module SimpleWriteOnExec(
    input logic clk,
    input logic reset
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
    logic [5:0] cgra [15:0];
    logic [7:0] edges [10:0];
    logic [5:0] stack [5:0];

    // Internals
    logic [7:0] current, next_current;
    logic [3:0] state, next_state;
    logic [3:0] index_input, next_index_input;
    logic [2:0] index_stack, next_index_stack;
    logic modified, next_modified, fe, next_fe;

    always_ff @(posedge clk) begin

        if(reset) begin
            $readmemb("input/cgra.bin", cgra);
            $readmemb("input/edges.bin", edges);
            state <= state_init;
        end

        else begin
            state <= next_state;
            modified <= next_modified;
            fe <= next_fe;
            index_input <= next_index_input;
            index_stack <= next_index_stack;
            current <= next_current;
        end

    end

    always_comb begin

        case(state)

            state_init:
            begin
                next_index_input <= 0;
                if(reset)
                next_state <= state_nextedge;
            end

            state_nextedge:
            begin
                if (cgra[index_input]==0)
                begin
                    next_state <= state_end;
                end
                else
                begin
                    next_current <= edges[index_input];
                    next_index_input <= index_input + 1;
                    next_fe <= 1;
                    next_modified <= 0;
                    next_state <= state_x_test;
                end
            end

            state_x_test:
            begin
                if ((current[7:4]%gridline_size - current[3:0]%gridline_size)==0)
                begin
                    next_state = state_y_test;
                end
                else if ((current[7:4]%gridline_size - current[3:0]%gridline_size)>0)
                begin
                    next_state = state_xdec_test;
                end
                else
                begin
                    next_state = state_xinc_test;
                end
            end

            state_xdec_test:
            begin
                if (cgra[current[7:4]][right] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                begin
                    next_state <= state_y_test;
                end
                else
                begin
                    next_state <= state_xdec_set;
                end
            end

            state_xdec_set:
            begin
                cgra[current[7:4]][right] <= 1;

                if (!fe) begin
                    cgra[current[7:4]][5:4]++;
                end

                next_current[7:4] <= current[7:4] + 1;
                next_modified <= 1;
                next_fe <= 0;

                stack[index_stack][1:0] <= right;
                stack[index_stack][5:2] <= current[7:4];
                next_index_stack <= index_stack + 1;

                next_state = state_x_test;
            end

            state_xinc_test:
            begin
                if (cgra[current[7:4]][left] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                begin
                    next_state <= state_y_test;
                end
                else
                begin
                    next_state <= state_xinc_set;
                end
            end

            state_xinc_set:
            begin
                cgra[current[7:4]][left] <= 1;

                if (!fe) begin
                    cgra[current[7:4]][5:4]++;
                end

                next_current[7:4] <= current[7:4] - 1;
                next_modified <= 1;
                next_fe <= 0;

                stack[index_stack][1:0] <= left;
                stack[index_stack][5:2] <= current[7:4];
                next_index_stack <= index_stack + 1;

                next_state = state_x_test;
            end

            state_y_test:
            begin
                if ((current[7:4]/gridline_size - current[3:0]/gridline_size)==0)
                begin
                    next_state = state_xy_test;
                end
                else if ((current[7:4]/gridline_size - current[3:0]/gridline_size)>0)
                begin
                    next_state = state_ydec_test;
                end
                else
                begin
                    next_state = state_yinc_test;
                end
            end

            state_ydec_test:
            begin
                if (cgra[current[7:4]][bot] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                begin
                    next_state <= state_xy_test;
                end
                else
                begin
                    next_state <= state_ydec_set;
                end
            end

            state_ydec_set:
            begin
                cgra[current[7:4]][bot] <= 1;

                if (!fe) begin
                    cgra[current[7:4]][5:4]++;
                end

                next_current[7:4] <= current[7:4] + gridline_size;
                next_modified <= 1;
                next_fe <= 0;

                stack[index_stack][1:0] <= bot;
                stack[index_stack][5:2] <= current[7:4];
                next_index_stack <= index_stack + 1;

                next_state = state_x_test;
            end

            state_yinc_test:
            begin
                if (cgra[current[7:4]][bot] || (cgra[current[7:4]][5:4] == max_bypass && !fe))
                begin
                    next_state <= state_xy_test;
                end
                else
                begin
                    next_state <= state_yinc_set;
                end
            end

            state_yinc_set:
            begin
                cgra[current[7:4]][bot] <= 1;

                if (!fe) begin
                    cgra[current[7:4]][5:4]++;
                end

                next_current[7:4] <= current[7:4] - gridline_size;
                next_modified <= 1;
                next_fe <= 0;

                stack[index_stack][1:0] <= bot;
                stack[index_stack][7:2] <= current[7:4];
                next_index_stack <= index_stack + 1;

                next_state = state_xy_test;
            end

            state_xy_test:
            begin
                if (current[7:4] == current[3:0])
                begin
                    next_state <= state_nextedge;    
                end
                else
                begin
                    next_state <= state_modified_test;
                end
            end

            state_modified_test:
            begin
                if (modified) begin
                    next_modified <= 1;
                    next_state <= state_x_test;
                end
                else
                begin
                    next_state <= state_blacklist;
                end
            end

            state_blacklist:
            begin
                cgra[stack[index_stack][5:2]][stack[index_stack][1:0]] <= 0;

                if (index_stack == 0)
                begin
                    next_state <= state_nextedge;
                end
                else
                begin
                    next_index_stack <= index_stack - 1;
                    cgra[stack[index_stack][5:2]][5:4]--;
                    next_state <= state_blacklist;
                end
            end

            state_end:
            begin
                next_state <= state_end;
            end

        endcase

    end
endmodule