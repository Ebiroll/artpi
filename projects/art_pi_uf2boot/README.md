# brief introduction

1. The function provided by this example is a boot program, which can virtualize a U disk 
(**press and hold the USER button on the board and then power on**),

   Users can directly copy the applications packaged in uf2 format to the virtual U disk, so as to achieve the purpose of flashing the firmware.

2. The virtual root directory of the U disk contains four files: **CODE.BIN** firmware mapping, **DATA.BIN** data mapping (function code has been added), **INFO_UF2TXT** records the basic information of the board Information, **INDEX.HTM** Opening this file will take you directly to the ART-Pi related page.

3. Please refer to the relevant part of main.c for the clock configuration of this example (**The app application clock written when this example is used as boot should match this example**), please pay attention to prevent it from running correctly.

4. The uf2tool in the tools directory is used to package pure binary programs into uf2 format.

5. uf2 format reference https://github.com/microsoft/uf2

   uf2.c uf2.h in the source code refer to https://github.com/adafruit/tinyuf2



Maintenance: lizimu2020