set_db library ../libs/freepdk/gscl45nm.lib
set_db write_vlog_unconnected_port_style none
set_db print_ports_nets_preserved_for_cb true
read_hdl ../benchmarks/baseline/b02.v
elaborate


create_clock -name clock -period 0.00 -waveform {0 0.00} [get_ports clock]
set_input_delay  0 -clock clock [all_inputs]
set_output_delay 0 -clock clock [all_outputs]
set_clock_uncertainty 0 clock

remove_cdn_loop_breaker
report_area > ../reports/b02/b02_area_1024.rpt
remove_cdn_loop_breaker
report_timing > ../reports/b02/b02_timing_1024.rpt
remove_cdn_loop_breaker
report_power > ../reports/b02/b02_power_1024.rpt
exit
