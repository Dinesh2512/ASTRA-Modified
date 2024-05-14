set_db library ../libs/freepdk/gscl45nm.lib
set_db write_vlog_unconnected_port_style none
set_db print_ports_nets_preserved_for_cb true
read_hdl ../benchmarks/baseline/dbench10.v
elaborate


create_clock -name clk -period 0.00 -waveform {0 0.00} [get_ports clk]
set_input_delay  0 -clock clk [all_inputs]
set_output_delay 0 -clock clk [all_outputs]
set_clock_uncertainty 0 clk

remove_cdn_loop_breaker
report_area > ../reports/dbench10/dbench10_area.rpt
remove_cdn_loop_breaker
report_timing > ../reports/dbench10/dbench10_timing.rpt
remove_cdn_loop_breaker
report_power > ../reports/dbench10/dbench10_power.rpt
exit
