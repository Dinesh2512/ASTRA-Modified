set_db init_lib_search_path ./
set_db library ../libs/freepdk/gscl45nm.lib
set_db init_hdl_search_path ./
set_db write_vlog_unconnected_port_style none
set_db print_ports_nets_preserved_for_cb true
read_hdl ../benchmarks/baseline/abench01.v
elaborate abench01


create_clock -name clk -period 0.00 -waveform {0 0.00} [get_ports clk]
set_input_delay  0 -clock clk [all_inputs]
set_output_delay 0 -clock clk [all_outputs]
set_clock_uncertainty 0 clk

syn_generic
syn_map 
remove_cdn_loop_breaker
change_names -inst -restricted {"." "/" "\\" " "} -convert_string "_"
change_names -hnet -restricted {"." "/" "\\" " "} -convert_string "_"
write_hdl > ../benchmarks/synthesised/abench01_synth.v
report_area > ../reports/abench01/abench01_area.rpt
report_timing > ../reports/abench01/abench01_timing.rpt
report_power > ../reports/abench01/abench01_power.rpt
exit