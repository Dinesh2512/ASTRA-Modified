set module_name "aes_192"
define_design_lib WORK -path ./WORK
set suppress_errors {IFS-001}
set sh_continue_on_error true
set verilogout_no_tri true

set_app_var target_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.db

analyze -f verilog {/home/UFAD/a.chatterjee/Desktop/lab_work/astra/benchmarks/mit_cep_dc_compile_ultra/designs_raw/aes_192.v}

elaborate ${module_name}

change_names -rules verilog -hierarchy

write -f verilog -o ../designs/${module_name}.v

exit
