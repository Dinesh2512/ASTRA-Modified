set_app_var target_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/aes_192.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]

set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> aes_192_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/des3.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> des3_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/dft.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> dft_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/fir.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> fir_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/gps.v}
create_clock -name sys_clk_50 -period 2 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> gps_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/idft.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> idft_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/iir.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> iir_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/md5.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> md5_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/rsa.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> rsa_dump.txt
}
remove_design

read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/sha256.v}
create_clock -name clk -period 2 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
set allNets [get_nets]
set mylist [get_object_name $allNets]   
foreach {net} $mylist {
    report_timing -to $net >> sha256_dump.txt
}
remove_design

quit