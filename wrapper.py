CSRC = "c/main.c c/fsm/common.c c/fsm/SimpleWriteOnExec.c"
CC= "gcc"
CFLAGS= "-O2 -march=native -Wall -Wextra -Wno-unused-result"
CLIB= ""
CBIN= "bin/c"

OUTPUT_FOLDER = "mkdir output"
BIN_FOLDER = "mkdir bin"

import os
import glob

os.system(BIN_FOLDER)
os.system(OUTPUT_FOLDER)
os.system(CC + " " + CSRC + " -o " + CBIN + " " + CFLAGS + " " + CLIB) # Compiling


for group in sorted(glob.glob("benchmarks/*")):
    output_folder = "output/" + group.split("/")[1] + "/"
    os.system("mkdir " + output_folder)

    for bench in sorted(glob.glob(group + "/edge_list/*")):
        name = bench.split("/")[3]
        grid = (group + "/grid/verilog_" + name + ".out")
        bench_out_folder = output_folder + "/" + name + "/"
        os.system("mkdir " + bench_out_folder)

        for edge_list in sorted(glob.glob(bench + "/*")):
            output_csv = bench_out_folder + edge_list.split("/")[4].split(".")[0] + ".csv"
            os.system("./" + CBIN + " " + grid + " " + edge_list + " " + output_csv)