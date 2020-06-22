module SimpleWriteOnExec(
    input wire clk,
    input wire reset,
    input wire [7:0] edges_input,
    output wire [5:0] cgra_exit
);

    // States
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

    // Orientation
    parameter right = 3;
    parameter left = 2;
    parameter top = 1;
    parameter bot = 0;

    // Constants
    parameter max_bypass = 2;
    parameter gridline_size = 4;

    // Memories
    reg [5:0] cgra [15:0];
    reg [5:0] stack [5:0];
    reg [7:0] edges [10:0];

    // Internals
    reg [7:0] current;
    reg [3:0] state;
    reg [3:0] index_input;
    reg [2:0] index_stack;
    reg modified, fe;

    assign cgra_exit = cgra[0]^cgra[1]^cgra[2]^cgra[3]^cgra[4]^cgra[5]^cgra[6]^cgra[7]^cgra[8]^cgra[9]^cgra[10]^cgra[11]^cgra[12]^cgra[13]^cgra[14]^cgra[15];

    always_ff @(posedge clk) begin

        if(reset) begin
            state <= state_init;
            edges[0] <= edges_input;
            edges[1] <= edges_input;
            edges[2] <= edges_input;
            edges[3] <= edges_input;
            edges[4] <= edges_input;
            edges[5] <= edges_input;
            edges[6] <= edges_input;
            edges[7] <= edges_input;
            edges[8] <= edges_input;
            edges[9] <= edges_input;
            edges[10] <= edges_input;
        end

        else begin
            case(state)

                state_init:
                begin
                    index_input <= 0;
                end

                state_nextedge:
                begin
                    if (cgra[index_input]==0)
                    begin
                        state <= state_end;
                    end
                    else
                    begin
                        current <= edges[index_input];
                        index_input <= index_input + 1;
                        index_stack <= 0;
                        fe <= 1;
                        modified <= 0;
                        state <= state_x_test;
                    end
                end

                state_x_test:
                begin
                    if ((current[7:4]%gridline_size - current[3:0]%gridline_size)==0)
                    begin
                        state = state_y_test;
                    end
                    else if ((current[7:4]%gridline_size - current[3:0]%gridline_size)>0)
                    begin
                        state = state_xdec_test;
                    end
                    else
                    begin
                        state = state_xinc_test;
                    end
                end

                state_xdec_test:
                begin
                    if (cgra[current[7:4]][right] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                    begin
                        state <= state_y_test;
                    end
                    else
                    begin
                        state <= state_xdec_set;
                    end
                end

                state_xdec_set:
                begin
                    cgra[current[7:4]][right] <= 1;

                    if (!fe) begin
                        cgra[current[7:4]][5:4] <= cgra[current[7:4]][5:4] + 1;
                    end

                    current[7:4] <= current[7:4] + 1;
                    modified <= 1;
                    fe <= 0;

                    stack[index_stack][1:0] <= right;
                    stack[index_stack][5:2] <= current[7:4];
                    index_stack <= index_stack + 1;

                    state = state_x_test;
                end

                state_xinc_test:
                begin
                    if (cgra[current[7:4]][left] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                    begin
                        state <= state_y_test;
                    end
                    else
                    begin
                        state <= state_xinc_set;
                    end
                end

                state_xinc_set:
                begin
                    cgra[current[7:4]][left] <= 1;

                    if (!fe) begin
                        cgra[current[7:4]][5:4] <= cgra[current[7:4]][5:4] + 1;
                    end

                    current[7:4] <= current[7:4] - 1;
                    modified <= 1;
                    fe <= 0;

                    stack[index_stack][1:0] <= left;
                    stack[index_stack][5:2] <= current[7:4];
                    index_stack <= index_stack + 1;

                    state = state_x_test;
                end

                state_y_test:
                begin
                    if ((current[7:4]/gridline_size - current[3:0]/gridline_size)==0)
                    begin
                        state = state_xy_test;
                    end
                    else if ((current[7:4]/gridline_size - current[3:0]/gridline_size)>0)
                    begin
                        state = state_ydec_test;
                    end
                    else
                    begin
                        state = state_yinc_test;
                    end
                end

                state_ydec_test:
                begin
                    if (cgra[current[7:4]][bot] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                    begin
                        state <= state_xy_test;
                    end
                    else
                    begin
                        state <= state_ydec_set;
                    end
                end

                state_ydec_set:
                begin
                    cgra[current[7:4]][bot] <= 1;

                    if (!fe) begin
                        cgra[current[7:4]][5:4] <= cgra[current[7:4]][5:4] + 1;
                    end

                    current[7:4] <= current[7:4] + gridline_size;
                    modified <= 1;
                    fe <= 0;

                    stack[index_stack][1:0] <= bot;
                    stack[index_stack][5:2] <= current[7:4];
                    index_stack <= index_stack + 1;

                    state = state_x_test;
                end

                state_yinc_test:
                begin
                    if (cgra[current[7:4]][bot] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                    begin
                        state <= state_xy_test;
                    end
                    else
                    begin
                        state <= state_yinc_set;
                    end
                end

                state_yinc_set:
                begin
                    cgra[current[7:4]][bot] <= 1;

                    if (!fe) begin
                        cgra[current[7:4]][5:4]++;
                    end

                    current[7:4] <= current[7:4] - gridline_size;
                    modified <= 1;
                    fe <= 0;

                    stack[index_stack][1:0] <= bot;
                    stack[index_stack][5:0] <= current[7:4];
                    index_stack <= index_stack + 1;

                    state = state_xy_test;
                end

                state_xy_test:
                begin
                    if (current[7:4] == current[3:0])
                    begin
                        state <= state_nextedge;    
                    end
                    else
                    begin
                        state <= state_modified_test;
                    end
                end

                state_modified_test:
                begin
                    if (modified) begin
                        modified <= 1;
                        state <= state_x_test;
                    end
                    else
                    begin
                        state <= state_blacklist;
                    end
                end

                state_blacklist:
                begin
                    cgra[stack[index_stack][5:2]][stack[index_stack][1:0]] <= 0;

                    if (index_stack == 0)
                    begin
                        state <= state_nextedge;
                    end
                    else
                    begin
                        index_stack <= index_stack - 1;
                        cgra[stack[index_stack][5:2]][5:4] <=cgra[stack[index_stack][5:2]][5:4] - 1;
                        state <= state_blacklist;
                    end
                end

                state_end:
                begin
                    state <= state_end;
                end

            endcase
        end

    end

endmodule