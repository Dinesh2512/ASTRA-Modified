set module_name "idft_top"
define_design_lib WORK -path ./WORK
#set suppress_errors {IFS-001}
#set sh_continue_on_error true
#set verilogout_no_tri true

set_app_var target_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db

analyze -f verilog {/home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/cep_full/idft.v}

elaborate ${module_name}

#write -f verilog -o ../designs/${module_name}_gtech.v

check_design
check_timing
set_max_area 0

ungroup -all -flatten -force
#compile_ultra 
compile -ungroup_all

change_names -rules verilog -hierarchy

write -f verilog -o ../designs/${module_name}.v
report_area > ../reports/${module_name}_area.rpt
report_power > ../reports/${module_name}_power.rpt
report_timing > ../reports/${module_name}_timing.rpt
exit
