set_app_var target_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/sha256/sha256_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/rsa/rsa_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/md5/md5_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/iir/iir_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/idft/idft_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_128.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_256.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_512.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_1024.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_2048.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_3072.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_4096.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_5120.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_6144.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_7168.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_puf_8192.v}
create_clock -name sys_clk_50 -period 7 {sys_clk_50}
set_input_delay 0.28 -clock sys_clk_50 [all_inputs]
set_output_delay 0.28 -clock sys_clk_50 [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/gps/gps_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/fir/fir_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/dft/dft_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/des3/des3_8192.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_128.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_128_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_128_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_128_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_128.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_256.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_256_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_256_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_256_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_256.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_512.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_512_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_512_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_512_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_512.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_1024.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_1024_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_1024_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_1024_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_1024.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_2048.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_2048_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_2048_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_2048_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_2048.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_3072.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_3072_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_3072_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_3072_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_3072.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_4096.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_4096_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_4096_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_4096_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_4096.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_5120.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_5120_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_5120_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_5120_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_5120.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_6144.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_6144_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_6144_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_6144_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_6144.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_7168.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_7168_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_7168_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_7168_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_7168.v
remove_design -designs
read_file -format verilog {/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_puf_8192.v}
create_clock -name clk -period 7 {clk}
set_input_delay 0.28 -clock clk [all_inputs]
set_output_delay 0.28 -clock clk [all_outputs]
report_area > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_8192_area.rpt
report_power > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_8192_power.rpt
report_timing > /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_8192_timing.rpt
write -f verilog -o /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/aes_192/aes_192_8192.v
remove_design -designs
exit