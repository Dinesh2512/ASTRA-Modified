set module_name "des3"
define_design_lib WORK -path ./WORK
set suppress_errors {IFS-001}
set sh_continue_on_error true
set verilogout_no_tri true

set_app_var target_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db

analyze -f verilog {/home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/des3.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/crp.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/key_sel3.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox1.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox2.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox3.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox4.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox5.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox6.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox7.v /home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/hdl_cores/des3/sbox8.v}

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
