# STM32 Dma2d

https://en.chowdera.com/2022/04/202204020406556000.html

Also compare to this,

https://github.com/lvgl/lv_port_stm32h745i_disco


And for free_rtos support, check out this.

https://github.com/lvgl/lv_port_esp32

# LittlevGL2RTT #
[! [Building Status] (https://travis-ci.org/liu2guang/LittlevGL2RTT.svg?branch=master)] (https://travis-ci.org/liu2guang/LittlevGL2RTT)
[! [release] (https://img.shields.io/badge/Release-v0.0.1-orange.svg)] (https://github.com/liu2guang/LittlevGL2RTT/releases)

LittlevGL2RTT is an intermediate frame layer of LittlevGL built on RTThread 3.0 and higher. The purpose is to seamlessly embed the LCD frame for RTT, without developers having to transplant. If you think the library is pleasing to the eye, please donate a little star. The little star is Update motivation !!!

Thanks to the author of LittlevGL for the GUI library with open source. Move the original library to [https://github.com/littlevgl/lvgl](https://github.com/littlevgl/lvgl "lvgl").

----
[Littlevgl2rtt exchange group QR code] (https://i.imgur.com/CONvJ4A.png)
----
## 1. Rendering

![Rendering](https://i.imgur.com/hdq3RhV.gif)

## 2. Install LittlevGL2RTT

At present, the littlevgl2rtt library has been added to the rtt official pkgs package, which can be directly enabled in the menuconfig online package.

1. Run menuconfig in env.
2. Go to RT-Thread online packages -> system packages directory.
3. Start LittlevGL2RTT, select version as latest version, configure resolution as your display resolution, then start demo and save and exit menuconfig.
4. Run pkgs --update to update the LittlevGL2RTT package to your bsp.
5. Execute scons/scons --target=xxx, compile and generate the project, download and run.
 
![Installation process](https://i.imgur.com/fojc8Ie.gif)

## 3. Uninstall LittlevGL2RTT

1. Run menuconfig in env.
2. Go to RT-Thread online packages -> system packages directory.
3. Close LittlevGL2RTT.
4. Execute pkgs --update, and enter `Y` to agree to delete the pkg package file.
5. Execute scons/scons --target=xxx, compile and generate the project, download and run.

![Uninstallation process](https://i.imgur.com/yAeXejV.gif)

## 4. Precautions

Because LittlevGL uses anonymous structures, you need to add compilation parameters to MDK: **--gnu -g -W**.

## 5. Development progress

1. The development environment is mainly RT1050 fire board (lcd is 800*480, input device is GT911), and qemu environment (800*480, input device is mouse).
2. The input interface has been adapted, and it has passed the test on the Wildfire 1050 board, and the effect is stable [update].
3. Currently only 16bit mode has been tested.
4. For more lvgl configuration to be exported to menuconfig, please submit it in issues.