module up_down_counter_tb;

  reg clk;
  reg reset;
  reg up_down;
  wire [2:0] count;

  up_down_counter uut (
    .clk(clk),
    .reset(reset),
    .up_down(up_down),
    .count(count)
  );

  always begin
    #5 clk = ~clk;
  end

  initial begin
    clk = 0;
    reset = 1;
    up_down = 0;

    // Test for 8 clock cycles
    #10 reset = 0;

    // Output truth table to a file
    $dumpfile("up_down_counter.vcd");
    $dumpvars(0, up_down_counter_tb);
    $dumpon;

    // Run for additional clock cycles
    #40;
    $dumpoff;
    $finish;
  end

endmodule
