module up_down_counter_3bit_tb;

  reg clk;
  reg reset;
  reg up;
  reg down;
  wire [2:0] count;

  up_down_counter_3bit dut (
    .clk(clk),
    .reset(reset),
    .up(up),
    .down(down),
    .count(count)
  );

  // Clock generator
  always #5 clk = ~clk;

  // Initialize signals
  initial begin
    clk = 0;
    reset = 1;
    up = 0;
    down = 0;
    #10 reset = 0;
  end

  // Test scenario 1: Count up
  initial begin
    #20 up = 1;
    #30 up = 0;
    #40 up = 1;
    #50 up = 0;
    #60 up = 1;
    #70 up = 0;
    #80 up = 1;
    #90 up = 0;
    #100 $finish;
  end

  // Test scenario 2: Count down
  initial begin
    #20 down = 1;
    #30 down = 0;
    #40 down = 1;
    #50 down = 0;
    #60 down = 1;
    #70 down = 0;
    #80 down = 1;
    #90 down = 0;
    #100 $finish;
  end

  // Test scenario 3: Reset
  initial begin
    #20 reset = 1;
    #30 reset = 0;
    #40 reset = 1;
    #50 reset = 0;
    #60 reset = 1;
    #70 reset = 0;
    #80 reset = 1;
    #90 reset = 0;
    #100 $finish;
  end

  // Monitor the count output
  always @(posedge clk) begin
    $display("Count: %b", count);
  end

endmodule