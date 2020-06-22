module Testbench();

    logic clk, reset, outputonly;
    logic [7:0] edges;

    wire [5:0] outputx;
    assign outputonly = ^outputx;

    initial begin
        $dumpfile("wave.vcd");
        $dumpvars(0, Testbench);
        reset<=1; edges <= 0; #1;
        reset<=0;
    end

    always begin
        clk<= 1; #1;
        clk<=0; #1;
        edges <= edges + 1;
    end

    SimpleWriteOnExec simpleWriteOnExec(clk, reset, edges, outputx);

endmodule