set_app_var target_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db 
set_app_var link_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db 
read_file -format verilog {../reports/aes_192/aes_192_puf_8192.v}
compile -map_effort high -area_effort high 
ungroup -all -flatten -all_instances
write -f verilog -o ../reports/aes_192/aes_192_puf_8192_resynth.v
exit
