module up_down_counter(
    input wire clk,
    input wire up_down,
    output reg [2:0] count // Using reg instead of wire for count
);

  always @(posedge clk) begin
    if (up_down) // Count up
      count <= (count == 3'b111) ? 3'b000 : count + 1;
    else // Count down
      count <= (count == 3'b000) ? 3'b111 : count - 1;
  end

endmodule

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