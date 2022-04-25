# ART-Pi BT_WiFi module firmware download

The ART-Pi board has been burned with the BT_WiFi firmware when it leaves the factory and stored in the external Flash. If the firmware is accidentally erased, the function of the BT_WiFi module will not be able to be used normally, and an abnormal situation will occur, and the BT_WiFi firmware needs to be re-burned. The solution provided by RT-Thread: first burn the factory routine firmware `factory`, and then choose to use Ymodem to download the firmware.

## Burn the factory routine

[TODO]

## Download BT_WIFI firmware

The download method of wifi and bluetooth firmware is the same, and ymodem transmission can be performed through the terminal. The firmware path is located in the `/tools/firmware/` directory. `ap6212-bt-image-1.0.rbl` is the Bluetooth firmware, and `ap6212-wifi-image-1.0.rbl` is the WIFI firmware. Taking WIFI firmware upload as an example, two steps need to be performed:

1. Execute the following commands in sequence on the terminal command line to format the fal partition named `wifi_image`:
    ````
    fal probe wifi_image
    fal erase 0 0x80000
    ````
    ![FAL_ERASE_CMD](./figures/fal_erase_cmd.png)
2. Execute the `update` command in the terminal command line.
3. Use ymodem for file transfer; it should be noted that the terminal needs to support the ymodem protocol, Xshell is recommended.

After completion, the system will automatically identify the incoming firmware, and if the firmware is valid, it will be automatically moved to the corresponding firmware storage area.