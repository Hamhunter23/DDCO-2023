`timescale 1ns / 1ps

module tb_up_down_counter;
  reg clk;
  reg rst;
  reg up_down;
  wire [2:0] count;

  // Instantiate the up_down_counter
  up_down_counter u1 (
    .clk(clk),
    .rst(rst),
    .up_down(up_down),
    .count(count)
  );

  initial begin
    // Initialize signals
    clk = 0;
    rst = 1;
    up_down = 0;

    // Dump waveforms to .vcd file
    $dumpfile("tb_up_down_counter.vcd");
    $dumpvars(0, tb_up_down_counter);

    // Apply reset
    #10 rst = 0;

    // Count up
    #10 up_down = 1;

    // Count down
    #100 up_down = 0;

    // Run for some time
    #100;

    // Finish simulation
    $finish;
  end

  // Generate clock
  always #5 clk = ~clk;

endmodule