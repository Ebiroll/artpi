/*
 * LTDC LCD Emulation
 * This emulates the stm32 ltdc device
 * It draws the LCD in a skin and handles mouse clicks on the buttons  
 * To speed up emulation, this uses 32 bit spi transfers from the controller.
 * It wouldn't be hard to make it do 8 bit transfers instead.
 * 
 * Martin Johnson 2020 M.J.Johnson@massey.ac.nz
 * Olof Astrand 2022 olof.astrand@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or
 * (at your option) any later version.
 */
#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/ssi/ssi.h"
#include "ui/console.h"
#include "ltdc.h"
#include "ui/input.h"
#include "hw/irq.h"
#include "sysemu/runstate.h"

//#define PANEL_WIDTH 240
//#define PANEL_HEIGHT 320

#define PANEL_WIDTH 240
#define PANEL_HEIGHT 320


// only one console
static ConsoleState console_state;


#define PORTRAIT_X_OFFSET 52
#define PORTRAIT_Y_OFFSET 40
#define LANDSCAPE_X_OFFSET 40
#define LANDSCAPE_Y_OFFSET 53

#define SKIN_PORTRAIT_X_OFFSET (62/2)
#define SKIN_PORTRAIT_Y_OFFSET (126/2)
#define SKIN_LANDSCAPE_X_OFFSET (126/2)
#define SKIN_LANDSCAPE_Y_OFFSET (82/2)

typedef struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a;} pixel;

const struct {
  guint          width;
  guint          height;
  guint          bytes_per_pixel; 
  pixel          pixel_data[240 * 320];
} ltdc_board_skin = 
 {
  240, 474, 4};

static void draw_skin(ConsoleState *c) {
    volatile uint32_t *dest = c->data;
    for (int i = 0; i < ltdc_board_skin.height; i++)
        for (int j = 0; j < ltdc_board_skin.width; j++) {
            pixel p = ltdc_board_skin.pixel_data[i * ltdc_board_skin.width + j];
            uint32_t rgba= (p.a<<24) | (p.r<<16) | (p.g<<8) | p.b;
            if (p.a < 200)
                rgba=0xff000000;
            if (c->width < c->height)  // portrait
                dest[i * ltdc_board_skin.width + j] = rgba;
            else
                dest[(ltdc_board_skin.width - j - 1) * ltdc_board_skin.height +i] = rgba;
        }
}

static void set_portrait(ConsoleState *c) {
    qemu_console_resize(c->con, ltdc_board_skin.width,
                        ltdc_board_skin.height);
    c->data=surface_data(qemu_console_surface(c->con));
    c->width = PANEL_HEIGHT;
    c->height = PANEL_WIDTH;
    c->x_offset = PORTRAIT_X_OFFSET;
    c->y_offset = PORTRAIT_Y_OFFSET;
    c->skin_x_offset = SKIN_PORTRAIT_X_OFFSET;
    c->skin_y_offset = SKIN_PORTRAIT_Y_OFFSET;
    c->skin_width = ltdc_board_skin.width;
    draw_skin(c);
}

static void set_landscape(ConsoleState *c) {
    qemu_console_resize(c->con, ltdc_board_skin.height,
                    ltdc_board_skin.width);
    c->data=surface_data(qemu_console_surface(c->con));
    c->width = PANEL_WIDTH;
    c->height = PANEL_HEIGHT;
    c->x_offset = LANDSCAPE_X_OFFSET;
    c->y_offset = LANDSCAPE_Y_OFFSET;
    c->skin_x_offset = SKIN_LANDSCAPE_X_OFFSET;
    c->skin_y_offset = SKIN_LANDSCAPE_Y_OFFSET;
    c->skin_width = ltdc_board_skin.height;
    draw_skin(c);
}

// transfer 32 bits at a time to speed things up.
// this needs the spi controller to do the same thing.
static uint32_t ltdc_transfer(SSIPeripheral *dev, uint32_t data)
{
    ConsoleState *c=&console_state;
    uint8_t *bytes;
    if(c->cmd_mode) {
        c->current_command=data;
    } else {
        switch (c->current_command) {
            case ST7789_MADCTL:
                if (data == 0 || data == 8) {  // portrait
                    set_portrait(c);
                } else {  // landscape
                    set_landscape(c);
                }
                break;
            case ST7789_CASET:
                bytes=(uint8_t *)&data;
                c->x_start = bytes[1]+bytes[0]*256;
                c->x_end = bytes[3]+bytes[2]*256;
                c->x = c->x_start;
                break;
            case ST7789_RASET:
                bytes=(uint8_t *)&data;
                c->y_start = bytes[1]+bytes[0]*256;
                c->y_end = bytes[3]+bytes[2]*256;
                c->y = c->y_start;
                break;
            case ST7789_RAMCTRL:
                if(data & 0x800)
                    c->little_endian = 1;
                else
                    c->little_endian = 0;
                break;
            case ST7789_RAMWR:
                for(int i=0;i<2;i++) {
                    uint16_t d16=data;
                    if(!c->little_endian) {
                        d16=(d16>>8) | (d16<<8);
                    }
                    uint32_t d32=((d16 & 0xf800) << 8) |
                            ((d16 & 0x7e0) << 5) | 
                            ((d16 & 0x1f) << 3);
                    if(!c->backlight) d32=(d32>>2)&0x3f3f3f;
                        
                    uint32_t offset = (c->y - c->y_offset + c->skin_y_offset) * 
                        c->skin_width + c->x - c->x_offset + c->skin_x_offset;
                    if(offset<ltdc_board_skin.height*ltdc_board_skin.width)
                        c->data[offset] = d32;
                    c->x++;
                    if (c->x > c->x_end) {
                        c->x = c->x_start;
                        c->y++;
                    }
                    if ((c->y > c->y_end)) {
                        c->y = c->y_start;
                        c->x = c->x_start;
                        c->redraw=1;
                        break;
                    }
                    data=data>>16;
                }
                break;
            }
    }
    return 0;
}

/* Command/data input.  */
static void ltdc_cd(void *opaque, int n, int level)
{
    ConsoleState *c=&console_state;
    c->cmd_mode = !level;
}

static void ltdc_backlight(void *opaque, int n, int level)
{
    ConsoleState *c=&console_state;
    if(c->backlight != level) {
        volatile unsigned *dest = c->data;
        uint32_t px=level?(64<<16)|(64<<8)|(64):0;
        for(int y=0;y<c->height;y++)
            for(int x=0;x<c->width;x++)
                dest[(y+c->skin_y_offset)*c->skin_width+x+c->skin_x_offset]=px^(rand()&0x0f0f0f);
        dpy_gfx_update(c->con, c->skin_x_offset, c->skin_y_offset, c->width, c->height);
    }
    c->backlight = level;
}

static void st7789_update_display(void *opaque) {
    ConsoleState *c = &console_state;
    if (!c->redraw) return;
    c->redraw = 0;
    //dpy_gfx_update(c->con, c->skin_x_offset, c->skin_y_offset, c->width, c->height);
    dpy_gfx_update(c->con, 0, 0, 240, 320);

}

static void st7789_invalidate_display(void *opaque) {
    ConsoleState *c = &console_state;
    c->redraw = 1;
}

static const GraphicHwOps ltdc_ops = {
    .invalidate = st7789_invalidate_display,
    .gfx_update = st7789_update_display,
};


static uint64_t ltdc_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
    LtdcState *s = (LtdcState *)opaque;
    uint32_t res = 0;

    switch (offset) {
          case CFBAR:

            qemu_log_mask(LOG_UNIMP,
                "ltdc_read: cfg offset 0x%08"HWADDR_PRIx
                " %d\n", offset,res);
                // RCC_FLAG_HSERDY
        break;
        case 0x00000048:
            res = 0xffffffff;
            break;

    default:
        qemu_log_mask(LOG_UNIMP,
                      "ltdc_read: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        res = 0;
        break;
    }

    return res;
}

uint16_t buff[240*320];

void ltdc_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
    LtdcState *s = (LtdcState *)opaque;
    ConsoleState *c=&console_state;


    printf("Address: 0x%" HWADDR_PRIx ", Value: 0x%x\n", offset, value);


    switch (offset) {
        case CFBAR:
        {
            qemu_log_mask(LOG_UNIMP,
                    "ltdc_write: CFBAR\n");

/*
void cpu_physical_memory_rw(hwaddr addr, void *buf,
    cpu_physical_memory_rw(addr, buf, len, false);
}

static inline void cpu_physical_memory_write(hwaddr addr,
                                             const void *buf, hwaddr len)
{
    cpu_physical_memory_rw(addr, (void *)buf, len, true);
}
*/
                    hwaddr addr=value;
                    void *buf;
                    int len=240*320*2;
                    cpu_physical_memory_rw(addr, (void *)buff, len, false);
                    uint32_t *ptr=(uint32_t *) buff;
                    uint32_t buff_offset=0;


                    for (int my=0;my<320;my++) {
                        for (int j=0;j<240;j++) {
                            //uint32_t offset = (my/2 - c->y_offset + c->skin_y_offset) * 
                            //    c->skin_width + j/2 - c->x_offset + c->skin_x_offset;

                            uint32_t offset=j+my*(c->skin_width);
                                pixel p;
                                p.r=(buff[buff_offset] & 0xf800) >> 8;  // 11 bits (should be 9)
                                p.g=(buff[buff_offset] & 0x07e0) >> 3;  // 5 bits  (should be 5)
                                p.b=(buff[buff_offset] & 0x1f) << 3;    // should be 2, but it looks a bit pale
                                p.a=255;

                                uint32_t rgba= (p.a<<24) | (p.r<<16) | (p.g<<8) | p.b;
                                //if (p.a < 200)
                                //    rgba=0xff000000;


                            if(offset<240*320 )  
                                c->data[offset] =   rgba;

                            buff_offset++;

                        }
                    }

            c->redraw=1;
            //dpy_gfx_update(c->con, 0, 0, 320, 240);
        }
        break;

        case PFCR:
            // Pixel format
            c->redraw=1;

            //set_portrait();
        break;

    default:
        qemu_log_mask(LOG_UNIMP,
                      "ltdc_write: Unknown offset 0x%08"HWADDR_PRIx
                      "\n", offset);
        break;
    }
}

static const MemoryRegionOps ltdc_mem_ops = {
    .read = ltdc_read,
    .write = ltdc_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

int touch_sensor[10];
#define PW 1200
static void keyboard_event(DeviceState *dev, QemuConsole *src,
                                InputEvent *evt) {
    LtdcState *s = LTDC(dev);
    ConsoleState *c = s->con;
    int qcode, up;
    InputMoveEvent *move;
    InputBtnEvent *btn;
    static int xpos = 0, ypos = 0;
    switch (evt->type) {
        case INPUT_EVENT_KIND_KEY:
            qcode = qemu_input_key_value_to_qcode(evt->u.key.data->key);
            up = 1 - evt->u.key.data->down;
            if (qcode == Q_KEY_CODE_1) {
                qemu_set_irq(s->button[0], up);
            }
            if (qcode == Q_KEY_CODE_2) {
                qemu_set_irq(s->button[1], up);
            }
            if (qcode == Q_KEY_CODE_R) {
                qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
            }
            int touch_codes[] = {Q_KEY_CODE_7, Q_KEY_CODE_8, Q_KEY_CODE_9,
                                 Q_KEY_CODE_0};
            int tsens[] = {2, 3, 8, 9};
            for (int i = 0; i < 4; i++)
                if (qcode == touch_codes[i])
                    touch_sensor[tsens[i]] = 1000 * (1 - up);
            break;

        case INPUT_EVENT_KIND_ABS:
            move = evt->u.abs.data;
            if (move->axis == 0) xpos = move->value;
            if (move->axis == 1) ypos = move->value;
            break;
        case INPUT_EVENT_KIND_BTN:
            btn = evt->u.btn.data;
            int portrait = c->height > c->width;
            up = (1 - btn->down);
            if (up) {
                qemu_set_irq(s->button[0], up);
                qemu_set_irq(s->button[1], up);
                for (int i = 2; i < 10; i++) 
                    touch_sensor[i] = 0;
                break;
            }
            if (portrait) {
                if (xpos > 24996 && xpos < 27962 && ypos > 28481 &&
                    ypos < 30347) {
                    qemu_set_irq(s->button[1], up);
                }
                if (xpos > 3071 && xpos < 6616 && ypos > 28481 &&
                    ypos < 30347) {
                    qemu_set_irq(s->button[0], up);
                }
                if (xpos > 30876 && xpos < 32530 && ypos > 23503 &&
                    ypos < 24713 && up == 0)
                    qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
                int xs[] = {0,    0, 1417,  1417,  1417,
                            1417, 0, 30010, 30010, 30010};
                int ys[] = {0,     0, 12132, 13791, 15312,
                            16694, 0, 18388, 12201, 13860};
                for (int i = 2; i < 10; i++) {
                    if (i != 6) {
                        if (xpos > (xs[i] - PW) && xpos < (xs[i] + PW) &&
                            ypos > (ys[i] - PW) && ypos < (ys[i] + PW))
                            touch_sensor[i] = 1000;
                    }
                }
            } else {
                if (xpos > 28308 && xpos < 30451 && ypos > 5199 &&
                    ypos < 8428) {
                    qemu_set_irq(s->button[1], up);
                }
                if (xpos > 28308 && xpos < 30451 && ypos > 26386 &&
                    ypos < 29852) {
                    qemu_set_irq(s->button[0], up);
                }
                if (xpos > 23607 && xpos < 24540 && ypos > 551 && ypos < 1732 &&
                    up == 0)
                    qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
                int xs[] = {0,     0, 12166, 13618, 15277,
                            16798, 0, 18388, 12166, 13791};
                int ys[] = {0,     0, 31743, 31743, 31743,
                            31743, 0, 2993,  2993,  2993};
                for (int i = 2; i < 10; i++) {
                    if (i != 6) {
                        if (xpos > (xs[i] - PW) && xpos < (xs[i] + PW) &&
                            ypos > (ys[i] - PW) && ypos < (ys[i] + PW))
                            touch_sensor[i] = 1000;
                    }
                }
            }
            break;
        default:
            break;
    }
}

static QemuInputHandler keyboard_handler = {
    .name  = "TDisplay Keys",
    .mask  = INPUT_EVENT_MASK_KEY | INPUT_EVENT_MASK_BTN | INPUT_EVENT_MASK_ABS,
    .event = keyboard_event,
};




static void ltdc_realize(DeviceState *dev, Error **errp) {
    LtdcState *s = LTDC(dev);

    qemu_input_handler_register(dev, &keyboard_handler);
    qdev_init_gpio_in_named(dev, ltdc_cd, "cmd",1);
    qdev_init_gpio_in_named(dev, ltdc_backlight, "backlight", 1);
    qdev_init_gpio_out_named(dev,s->button,"buttons",2);

}

static void ltdc_instance_init(Object *obj)
{
    LtdcState *s = LTDC(obj);
    DeviceState *dev = DEVICE(s);


    memory_region_init_io(&s->iomem, obj, &ltdc_mem_ops, s, TYPE_LTDC,
                          0x1000);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
    sysbus_init_irq(SYS_BUS_DEVICE(s), &s->ltcd_irq);

    if (console_state.con == 0) {
        ConsoleState *c=&console_state;
        s->con=c;
        console_state.con = graphic_console_init(dev, 0, &ltdc_ops, s);
        set_portrait(c);
    } else {
        s->con = &console_state;
    }

}




static void ltdc_class_init(ObjectClass *klass, void *data) {
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = ltdc_realize;


   /*
    SSIPeripheralClass *k = SSI_PERIPHERAL_CLASS(klass);

    k->realize = ltdc_realize;
    k->transfer = ltdc_transfer;
    k->cs_polarity = SSI_CS_NONE;
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
    */
}

static const TypeInfo ltdc_info = {
    .name = TYPE_LTDC,
    .parent  = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(LtdcState),

    .instance_init = ltdc_instance_init,
    .class_init = ltdc_class_init
};

static void ltdc_register_types(void) {
    type_register_static(&ltdc_info);
}

type_init(ltdc_register_types)
