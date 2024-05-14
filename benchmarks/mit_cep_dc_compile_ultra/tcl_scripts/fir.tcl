set module_name "FIR_filter"
define_design_lib WORK -path ./WORK
set suppress_errors {IFS-001}
set sh_continue_on_error true
set verilogout_no_tri true

set_app_var target_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db

analyze -f verilog {/home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/cep_full/fir.v}

elaborate ${module_name}

check_design
check_timing
set_max_area 0

ungroup -all -flatten -force
compile_ultra 
change_names -rules verilog -hierarchy

write -f verilog -o ../designs/${module_name}.v
report_area > ../reports/${module_name}_area.rpt
report_power > ../reports/${module_name}_power.rpt
report_timing > ../reports/${module_name}_timing.rpt
exit
