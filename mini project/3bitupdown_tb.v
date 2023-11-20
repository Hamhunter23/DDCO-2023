module testbench;

  reg clk;
  reg up_down;
  wire [2:0] count;

  // Instantiate the up_down_counter module
  up_down_counter dut (
    .clk(clk),
    .up_down(up_down),
    .count(count)
  );

  // Clock generation
  always #5 clk = ~clk;

  // Test scenario
  initial begin
    clk = 0;
    up_down = 1; // Start counting up

    // Reset and initialize
    #10;

    // Test sequence
    repeat(8) begin
      #10; // Change input after 10 time units
      up_down = ~up_down; // Toggle up_down signal
    end

    // End simulation after testing
    $finish;
  end

  // Display counter value
  always @(posedge clk)
    $display("Count: %b", count);

endmodule
