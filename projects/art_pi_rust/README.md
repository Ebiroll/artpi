 RUST driver and application support

## Introduction

This routine provides support for rust applications and drivers.
The application requires additional compilation.
The code repository is located at: [driver_for_rtt](https://github.com/vito-chl/driver_for_rtt.git)



## Software Description

The program compiled by rust is a static library, which is linked to this project for compilation.
The static library is stored in the board directory and named librust_rtt.a.

## run
### Compile & Download

After compiling, connect the ST-Link USB port of the development board to the PC, and then download the firmware to the development board.


### running result

After normal operation, the driver framework will register the driver and print the log.
The rust entry application runs automatically.

## Precautions

For more information, please refer to the repository: [rtt_rs](https://github.com/vito-chl/rtt_rs.git)
