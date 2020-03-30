# uart
An implementation of UART RS-232 that can synthesize of an FPGA.

# Source
You can find the source code for transmission and receiving in `src/modules`. There are tests and a makefile in `src`. Run `make <tx or rx>` to get the Gtkwave output. The `open` command on Mac opens .vcd files with Gtkwave, but on other OS's this line will have to be changed in the Makefile.

The default settings are configured for a 100 Mhz clock operating at 9600 baud. Set the `CLKS_PER_BIT` paramater to your chip's clock frequency divided by the desired baud rate (max 115200).

# FPGA Example
You can find an example FPGA project in `fpga-bounceback`. It will simply bounce back anything it receives and light up the first 8 LED's with the last byte received. You only need the micro-USB cable that you use to program the FPGA, as it has a USB-UART bridge built-in. You can flip switch 0 to run an asynchronous reset current throughout the entire circuit. Compile and run the C program in `comms/bounceback.c` to communicate with the FPGA. You may need to modify the source to open a different portname. You can identify the port by plugging in the USB cable to the laptop and seeing what was added to the `/dev` folder.

I used a Basys3 board. Other boards require easy reconfiguration.
