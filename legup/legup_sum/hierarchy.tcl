add wave -hex -group "main_top" -group "ports"  {*}[lsort [find nets -ports [lindex [find instances -bydu main_top] 0]/*]]
add wave -hex -group "main_top" -group "main" -group "ports"  {*}[lsort [find nets -ports [lindex [find instances -r /main_inst] 0]/*]]
