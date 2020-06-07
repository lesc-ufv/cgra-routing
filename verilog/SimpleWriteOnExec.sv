module SimpleWriteOnExec(
    input logic clk,
    input logic reset
);

    parameter state_init = 0;
    parameter state_nextedge = 1;
    parameter state_end = 2;
    parameter state_x_test = 3;
    parameter state_xinc_test = 4;
    parameter state_xinc_set = 5;
    parameter state_xdec_test = 6;
    parameter state_xdec_set = 7;
    parameter state_y_test = 8;
    parameter state_yinc_test = 9;
    parameter state_yinc_set = 10;
    parameter state_ydec_test = 11;
    parameter state_ydec_set = 12;
    parameter state_xy_test = 13;
    parameter state_modified_test = 14;
    parameter state_blacklist = 15;

    // Memories
    logic [5:0] cgra [15:0];
    logic [7:0] edges [10:0];
    logic [5:0] stack [7:0];

    // Internals
    logic [3:0] state, next_state;
    logic [3:0] index_input, next_index_input;
    logic [2:0] index_stack, next_index_stack;
    logic modified, next_modified;

    always_ff @(posedge clk) begin

        if(reset) begin
            $readmemb("input/cgra.bin", cgra);
            $readmemb("input/edges.bin", edges);
            state <= state_init;
        end

        else begin
            state <= next_state;
            modified <= next_modified;
            index_input <= next_index_input;
            index_stack <= next_index_stack;
        end

    end

    always_comb begin

        case(state)

            state_init: begin
                next_index_input <= 0;
                next_modified <= 0;
                next_state <= state_init;
                next_index_stack <= 0;
            end

        endcase

    end
endmodule