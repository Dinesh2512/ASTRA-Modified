set module_name "aes_192"
define_design_lib WORK -path ./WORK
set suppress_errors {IFS-001}
set sh_continue_on_error true
set verilogout_no_tri true

set_app_var target_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db

analyze -f verilog {/home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/aes/aes_192.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/aes/round.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/aes/table.v}

elaborate ${module_name}

check_design
check_timing
set_max_area 0

ungroup -all
compile_ultra 

write -f verilog -o ${module_name}.v
report_area > ./reports/${module_name}_area.rpt
report_power > ./reports/${module_name}_power.rpt
report_timing > ./reports/${module_name}_timing.rpt
exit
