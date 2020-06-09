add wave -hex -group "top" -group "ports"  {*}[lsort [find nets -ports [lindex [find instances -bydu top] 0]/*]]
add wave -hex -group "top" -group "main" -group "ports"  {*}[lsort [find nets -ports [lindex [find instances -r /main_inst] 0]/*]]
