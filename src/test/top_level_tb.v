//
// Module: test_top_level
// 
// Notes:
// - Top level simulation testbench.
// - Uses a working tx to test

// module top_level(
//     input clk,
//     input RxD,
//     input sw_reset,
//     output TxD,
//     output [15:0] led
// );

module test_top_level;
    
  reg        clk      = 0; // Top level system clock input.
  reg        reset   = 1;

  wire       uart_rx_done; // done receiving byte
  wire [7:0] uart_rx_byte; // full byte received
  
  reg uart_tx_en = 0;
  reg [7:0] uart_tx_data; // full data to be sent on CPU side
  wire [7:0] uart_rx_data; // received data on CPU side
  wire uart_txd_cpu; // UART transmit pin from CPU
  wire uart_txd_fpga; // UART transmit pin from FPGA
  wire [15:0] led;

  // Period and frequency of the system clock.
  always #1 clk = !clk;

  initial begin
          $dumpfile("test_top_level.vcd");
          $dumpvars(0, test_top_level);
          # 2  reset = 0;      // stop reseting the register 

          # 2
            // send 'a'
            uart_tx_data = 8'b01100001;
            uart_tx_en = 1;
            
          #400000 // send, receive (on FPGA), and send back 'a'

          // send 'b'
            uart_tx_data = 8'b01100010;
            uart_tx_en = 1;
          
          #400000 // send, receive (on FPGA), and send back 'b'
          $finish;
  end
  
  uart_tx cpu_tx(clk, reset, uart_tx_en, uart_tx_data, , uart_txd_cpu, ); // mimics cpu sending data to FPGA
  top_level fpga(clk, uart_txd_cpu, reset, uart_txd_fpga, led);
  uart_rx cpu_rx(clk, reset, 1'b1, uart_txd_fpga, , uart_rx_data); // mimics cpu receiving data from FPGA

endmodule
