set_db library ../libs/freepdk/gscl45nm.lib
set_db write_vlog_unconnected_port_style none
set_db print_ports_nets_preserved_for_cb true
read_hdl ../benchmarks/baseline/s832.v
elaborate


create_clock -name clk -period 0.00 -waveform {0 0.00} [get_ports clk]
set_input_delay  0 -clock clk [all_inputs]
set_output_delay 0 -clock clk [all_outputs]
set_clock_uncertainty 0 clk

remove_cdn_loop_breaker
report_area > ../reports/s832/s832_area.rpt
remove_cdn_loop_breaker
report_timing > ../reports/s832/s832_timing.rpt
remove_cdn_loop_breaker
report_power > ../reports/s832/s832_power.rpt
exit
