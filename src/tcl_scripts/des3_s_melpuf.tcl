set_db library ../libs/freepdk/gscl45nm.lib
set_db write_vlog_unconnected_port_style none
set_db print_ports_nets_preserved_for_cb true
read_hdl ../benchmarks/baseline/des3_s.v
elaborate


create_clock -name clock -period 0.00 -waveform {0 0.00} [get_ports clock]
set_input_delay  0 -clock clock [all_inputs]
set_output_delay 0 -clock clock [all_outputs]
set_clock_uncertainty 0 clock

remove_cdn_loop_breaker
report_area > ../reports/des3_s/des3_s_area_1024.rpt
remove_cdn_loop_breaker
report_timing > ../reports/des3_s/des3_s_timing_1024.rpt
remove_cdn_loop_breaker
report_power > ../reports/des3_s/des3_s_power_1024.rpt
exit
