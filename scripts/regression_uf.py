#!/usr/bin/env python
# Imports
import os
import csv
import re

# ripper -help (full non-release version)
'''
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                             RIPPER
            An IP logic removal and replacement tool
      Version : RIPPER-1.3.2 (Wed Aug  2 05:34:40 EDT 2023)
           Copyright : University of Florida, FL, USA
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Usage: ./ripper -hdl <Absolute/Path/To/Design/File/Or/Directory/> -module <Top Module> [option1 option2 ...]
Options:
	-clock <Clock Port List: clock_1, clock_2, ..., clock_n >
	-reset <Reset Port List: reset_1, reset_2, ..., reset_n >
	-reset_edge <Reset Edge Sensitivity List: 1, 0, ..., 1 >
	-lib_path <Absolute/Path/To/RIPPER/lib/Directory/>
	-model_path <Absolute/Path/To/RIPPER/models/Directory/>
	-link_library <Absolute/Path/To/Link/Library/Directory/>
	-target_library <Absolute/Path/To/Target/Library/Directory/>
	-pvt <Process_Voltage_Temperature String To Look For In The Target Library File Name(s)>
	-sdc <Absolute/Path/To/SDC/File.sdc>
	-bit_comp <Bitstream Compaction Percentage (floating point between 50 and 100)>
	-seed <Seed For Random Number Generator (integer)>
	-syn_tcl <Absolute/Path/To/Synthesis/Tool/TCL/File.tcl>
	-sim_directives <Absolute/Path/To/Simulation/Directives/File.v>
	-buffer_size <Bitstream Configuration Buffer Size (integer)>
	-x_param_csb <CLUT % (x) Parameter For CSB (floating point between 0 and 100)>
	-y_param_csb <Flip-Flop % (y) Parameter For CSB (floating point between 0 and 100)>
	-custom_library <Absolute/Path/To/Custom/Intel/Library/Directory/>
	-variant <Process Variant String To Look For In The Intel Library(s)>
	-timing <Timing Model String To Look For In The Intel Library(s)>
	-max_clut_size <Maximum CLUT Input Size (integer between 2 and 4)>
	-uf_1in_area <UF (LEDA) 1-Input Gate Area (floating point)>
	-uf_2in_area <UF (LEDA) 2-Input Gate Area (floating point)>
	-uf_3in_area <UF (LEDA) 3-Input Gate Area (floating point)>
	-uf_4in_area <UF (LEDA) 4-Input Gate Area (floating point)>
	-clut2_incr <CLUT2 Skew-Factor (floating point between 0 and 100)>
	-clut3_incr <CLUT3 Skew-Factor (floating point between 0 and 100)>
	-clut4_incr <CLUT4 Skew-Factor (floating point between 0 and 100)>
	-config_clk_period <Target Period for Configuration Clock(s) In SDC (integer value in library units)>
	-debug
	-flow_map
	-show_logs
	-cadence_flow
	-no_csb
	-disable_io_renaming
	-disable_all_renaming
	-no_overheads
	-fdbk_from_csbout_mux
	-use_org_sdc
	-use_random_seed
	-lec
	-custom
	-use_intel_sim_flags
	-analyze
	-verify
	-use_sim_model
	-enable_lbr_msgs
	-use_uf_mux
	-disable_y_csb_comp
	-use_org_as_golden
Exiting...
'''

# Benchmark class
class benchmark:
    name = ""
    hdl_name = ""
    module_name = ""
    clock_type = ""
    clock_name = ""
    reset_name = ""
    reset_edge = ""
    group = ""
    seed = ""
    clut1_incr = ""
    clut2_incr = ""
    clut3_incr = ""
    clut4_incr = ""
    clut5_incr = ""
    clut6_incr = ""
    clut7_incr = ""
    uf_1in_area = ""
    uf_2in_area = ""
    uf_3in_area = ""
    uf_4in_area = ""
    mem = ""
    ignore = ""
    org_gate_count = ""
    trans_gate_count = ""
    clut_count = ""
    bitstream = ""
    bit_comp = ""
    comp_bitstream = ""
    area_overhead = ""
    delay_overhead = ""
    power_overhead = ""
    mismatch = ""
    runtime = ""

# Configuration class


class config:
    bench_group = ""
    binary = ""
    bench_path = ""
    lib_path = ""
    model_path = ""
    link_lib = ""
    target_lib = ""
    pvt = ""
    sdc_path = ""
    bit_comp = ""
    seed = ""
    syn_tcl = ""
    sim_directives = ""
    buffer_size = ""
    x_param_csb = ""
    y_param_csb = ""
    max_clut_size = ""
    custom_lib = ""
    variant = ""
    timing = ""
    config_clk_period = ""
    tool_flags = ""
    uf_1in_area = ""
    uf_2in_area = ""
    uf_3in_area = ""
    uf_4in_area = ""
    clut2_incr = ""
    clut3_incr = ""
    clut4_incr = ""
    clean_runs = ""
    hide_RIPPER_logs = ""
    fire_runs = ""


# User input
regr_lib = str(
    input("\n>> Enter regression library [ Options: leda | intel | intel_custom ]: ")) # | sky130 | gf180 | saed32 ]: "))

#if regr_lib not in {"leda", "intel", "intel_custom", "sky130", "gf180", "saed32"}:
if regr_lib not in {"leda", "intel", "intel_custom"}:
    print(
        "\n-- The regression library specified [" + regr_lib + "] is not supported. Exiting...\n")
    quit()

# Path to configure.*.txt file
cfg_path = "./configure." + regr_lib + ".txt"
if os.path.isfile(cfg_path) == False:
    print("\n-- Cannot find " + cfg_path +
          " file with the configurations for regression. Exiting...\n")
    quit()

# Parse configure.*.txt file
CFG = config()
print("\n-- Parsing " + cfg_path + " file for regression configurations...")
with open(cfg_path, "r") as cfg_file:
    for line in cfg_file:
        if str(line[0]) == '#' or str(line[0]) == ' ' or '=""' in line:
            continue

        if "=" in line:
            pattern = re.search('=".+"', line)
            if pattern:
                val = pattern.group().replace('"', '').replace('=', '')

            if "bench_group" in line:
                CFG.bench_group = val
            if "binary" in line:
                CFG.binary = val
            if "bench_path" in line:
                CFG.bench_path = val
            if "lib_path" in line:
                CFG.lib_path = val
            if "model_path" in line:
                CFG.model_path = val
            if "link_lib" in line:
                CFG.link_lib = val
            if "target_lib" in line:
                CFG.target_lib = val
            if "pvt" in line:
                CFG.pvt = val
            if "sdc_path" in line:
                CFG.sdc_path = val
            if "bit_comp" in line:
                CFG.bit_comp = val
            if "seed" in line:
                if "flags" not in line:
                    CFG.seed = val
            if "syn_tcl" in line:
                CFG.syn_tcl = val
            if "sim_directives" in line:
                CFG.sim_directives = val
            if "buffer_size" in line:
                CFG.buffer_size = val
            if "x_param_csb" in line:
                CFG.x_param_csb = val
            if "y_param_csb" in line:
                CFG.y_param_csb = val
            if "max_clut_size" in line:
                CFG.max_clut_size = val
            if "uf_1in_area" in line:
                CFG.uf_1in_area = val
            if "uf_2in_area" in line:
                CFG.uf_2in_area = val
            if "uf_3in_area" in line:
                CFG.uf_3in_area = val
            if "uf_4in_area" in line:
                CFG.uf_4in_area = val
            if "clut2_incr" in line:
                CFG.clut2_incr = val
            if "clut3_incr" in line:
                CFG.clut3_incr = val
            if "clut4_incr" in line:
                CFG.clut4_incr = val
            if "custom_lib" in line:
                CFG.custom_lib = val
            if "variant" in line:
                CFG.variant = val
            if "timing" in line:
                CFG.timing = val
            if "config_clk_period" in line:
                CFG.config_clk_period = val
            if "tool_flags" in line:
                CFG.tool_flags = val
            if "clean_runs" in line:
                CFG.clean_runs = val
            if "hide_RIPPER_logs" in line:
                CFG.hide_RIPPER_logs = val
            if "fire_runs" in line:
                CFG.fire_runs = val


# Create a list of benchmarks for regression from benchmarks_list.txt
benchmark_list = []
bench_list_path = "./benchmarks_list.txt"
if os.path.isfile(bench_list_path) == False:
    print("\n-- Cannot find " + bench_list_path +
          " file with list of benchmarks for regression. Exiting...\n")
    quit()

with open(bench_list_path, "r") as list_file:
    for line in list_file:
        parts = line.split()

        B = benchmark()
        B.hdl_name = parts[0].replace('#', '')
        B.name = B.hdl_name.split('.')[0]
        B.module_name = parts[1]
        B.clock_type = parts[2]
        B.clock_name = parts[3]
        B.reset_name = parts[4]
        B.reset_edge = parts[5]
        B.group = parts[6]

        if CFG.bench_group == "all":
            if str(line[0]) == "#":
                continue
            else:
                benchmark_list.append(B)

        elif B.group == CFG.bench_group:
            benchmark_list.append(B)


# Prepare regression scripts
if len(benchmark_list) == 0:
    print("\n-- " + str(len(benchmark_list)) +
          " benchmarks specified for regression. Exiting...\n")
    quit()
else:
    print("\n-- Starting RIPPER regression run for " +
          str(len(benchmark_list)) + " benchmarks...")

    group_path = "./runs_" + str(CFG.bench_group)
    if CFG.clean_runs == "true":
        cleanup_cmd = "rm -rf ." + group_path + ".bak"
        os.system(cleanup_cmd)
        cleanup_cmd = "rm -rf " + group_path
        os.system(cleanup_cmd)
    else:
        if os.path.isdir(group_path):
            backup_cmd = "rm -rf ." + group_path + ".bak"
            os.system(backup_cmd)
            backup_cmd = "mv " + group_path + " ." + group_path + ".bak"
            os.system(backup_cmd)

    group_dir_cmd = "mkdir " + group_path
    os.system(group_dir_cmd)

    with open(group_path + "/fire_test_runs.sh", "a") as sc:
        sc.write("#!/usr/bin/env bash\n")

    benchmark_list_passed = []
    for bench in benchmark_list:

        if CFG.fire_runs == "true":
            print("\n---- Generating RIPPER test run script for benchmark: " +
                  str(bench.name) + " (Module: " + str(bench.module_name) + ")...")

        dir_path = group_path + "/test_run_" + str(bench.name)
        dir_path_only_name = "test_run_" + str(bench.name)
        # if CFG.clean_runs == "true":
        #    cleanup_cmd = "rm -rf ." + dir_path + ".bak"
        #    os.system(cleanup_cmd)
        #    cleanup_cmd = "rm -rf " + dir_path
        #    os.system(cleanup_cmd)
        # else:
        #    if os.path.isdir(dir_path):
        #        backup_cmd = "rm -rf ." + dir_path + ".bak"
        #        os.system(backup_cmd)
        #        backup_cmd = "mv " + dir_path + " ." + dir_path + ".bak"
        #        os.system(backup_cmd)

        dir_cmd = "mkdir " + dir_path
        os.system(dir_cmd)

        # Option based vars
        if "analyze" in CFG.tool_flags:
            report_name = str(bench.name) + \
                "_analysis_report." + regr_lib + ".txt"
        else:
            report_name = str(bench.name) + \
                "_transformation_report." + regr_lib + ".txt"

        if CFG.hide_RIPPER_logs == "false":
            report_str = " | tee " + report_name
        else:
            report_str = " > " + report_name

        binary_str = str(CFG.binary) + " \\\n\t"
        hdl_str = " -hdl " + CFG.bench_path + \
            str(bench.group) + "/" + str(bench.hdl_name) + " \\\n\t"
        module_str = " -module " + str(bench.module_name) + " \\\n\t"
        clock_str = " -clock " + str(bench.clock_name) + " \\\n\t"
        reset_str = " -reset " + str(bench.reset_name) + " \\\n\t"
        reset_edge_str = " -reset_edge " + str(bench.reset_edge) + " \\\n\t"

        if CFG.lib_path != "":
            lib_path_str = " -lib_path " + str(CFG.lib_path) + " \\\n\t"
        else:
            lib_path_str = ""

        if CFG.model_path != "":
            model_path_str = " -model_path " + str(CFG.model_path) + " \\\n\t"
        else:
            model_path_str = ""

        if CFG.link_lib != "":
            link_lib_str = " -link_library " + str(CFG.link_lib) + " \\\n\t"
        else:
            link_lib_str = ""

        if CFG.target_lib != "":
            target_lib_str = " -target_library " + \
                str(CFG.target_lib) + " \\\n\t"
        else:
            target_lib_str = ""

        if CFG.pvt != "":
            pvt_str = " -pvt " + str(CFG.pvt) + " \\\n\t"
        else:
            pvt_str = ""

        if CFG.sdc_path != "":
            if bench.clock_type == "seq":
                sdc_str = " -sdc " + str(CFG.sdc_path) + "/" + str(bench.group) + "/" + str(bench.name) + ".sdc" + " \\\n\t"
            else:
                sdc_str = ""
        else:
            sdc_str = ""

        if CFG.bit_comp != "":
            bit_comp_str = " -bit_comp " + str(CFG.bit_comp) + " \\\n\t"
        else:
            bit_comp_str = ""

        if CFG.seed != "":
            seed_str = " -seed " + str(CFG.seed) + " \\\n\t"
        else:
            seed_str = ""

        if CFG.syn_tcl != "":
            syn_tcl_str = " -syn_tcl " + str(CFG.syn_tcl) + " \\\n\t"
        else:
            syn_tcl_str = ""

        if CFG.sim_directives != "":
            sim_dir_str = " -sim_directives " + \
                str(CFG.sim_directives) + " \\\n\t"
        else:
            sim_dir_str = ""

        if CFG.buffer_size != "":
            buffer_size_str = " -buffer_size " + \
                str(CFG.buffer_size) + " \\\n\t"
        else:
            buffer_size_str = ""

        if CFG.x_param_csb != "":
            x_param_csb_str = " -x_param_csb " + \
                str(CFG.x_param_csb) + " \\\n\t"
        else:
            x_param_csb_str = ""

        if CFG.y_param_csb != "":
            y_param_csb_str = " -y_param_csb " + \
                str(CFG.y_param_csb) + " \\\n\t"
        else:
            y_param_csb_str = ""

        if CFG.max_clut_size != "":
            max_clut_size_str = " -max_clut_size " + \
                str(CFG.max_clut_size) + " \\\n\t"
        else:
            max_clut_size_str = ""

        if CFG.uf_1in_area != "":
            uf_1in_area_str = " -uf_1in_area " + \
                str(CFG.uf_1in_area) + " \\\n\t"
        else:
            uf_1in_area_size_str = ""

        if CFG.uf_2in_area != "":
            uf_2in_area_str = " -uf_2in_area " + \
                str(CFG.uf_2in_area) + " \\\n\t"
        else:
            uf_2in_area_size_str = ""

        if CFG.uf_3in_area != "":
            uf_3in_area_str = " -uf_3in_area " + str(CFG.uf_3in_area) + " \\\n\t"
        else:
            uf_3in_area_size_str = ""

        if CFG.uf_4in_area != "":
            uf_4in_area_str = " -uf_4in_area " + str(CFG.uf_4in_area) + " \\\n\t"
        else:
            uf_4in_area_size_str = ""

        if CFG.clut2_incr != "":
            clut2_incr_str = " -clut2_incr " + str(CFG.clut2_incr) + " \\\n\t"
        else:
            clut2_incr_str = ""

        if CFG.clut3_incr != "":
            clut3_incr_str = " -clut3_incr " + str(CFG.clut3_incr) + " \\\n\t"
        else:
            clut3_incr_str = ""

        if CFG.clut4_incr != "":
            clut4_incr_str = " -clut4_incr " + str(CFG.clut4_incr) + " \\\n\t"
        else:
            clut4_incr_str = ""

        if CFG.custom_lib != "":
            custom_lib_str = " -custom_library " + \
                str(CFG.custom_lib) + " \\\n\t"
        else:
            custom_lib_str = ""

        if CFG.variant != "":
            variant_str = " -variant " + str(CFG.variant) + " \\\n\t"
        else:
            variant_str = ""

        if CFG.timing != "":
            timing_str = " -timing " + str(CFG.timing) + " \\\n\t"
        else:
            timing_str = ""

        if CFG.config_clk_period != "":
            config_clk_period_str = " -config_clk_period " + str(CFG.config_clk_period) + " \\\n\t"
        else:
            config_clk_period_str = ""

        if CFG.tool_flags != "":
            tool_flags_str = " " + str(CFG.tool_flags) + " \\\n\t"
        else:
            tool_flags_str = ""

        if bench.clock_type == "seq":
            tool_cmd = binary_str  \
                + hdl_str \
                + module_str \
                + clock_str \
                + reset_str \
                + reset_edge_str \
                + lib_path_str \
                + model_path_str \
                + link_lib_str \
                + target_lib_str \
                + pvt_str \
                + sdc_str \
                + bit_comp_str \
                + seed_str \
                + syn_tcl_str \
                + sim_dir_str \
                + buffer_size_str \
                + x_param_csb_str \
                + y_param_csb_str \
                + max_clut_size_str \
                + uf_1in_area_size_str \
                + uf_2in_area_size_str \
                + uf_3in_area_size_str \
                + uf_4in_area_size_str \
                + clut2_incr_str \
                + clut3_incr_str \
                + clut4_incr_str \
                + custom_lib_str \
                + variant_str \
                + timing_str \
                + config_clk_period_str \
                + tool_flags_str \
                + report_str

        else:
            tool_cmd = binary_str \
                + hdl_str \
                + module_str \
                + lib_path_str \
                + model_path_str \
                + link_lib_str \
                + target_lib_str \
                + pvt_str \
                + sdc_str \
                + bit_comp_str \
                + seed_str \
                + syn_tcl_str \
                + sim_dir_str \
                + buffer_size_str \
                + x_param_csb_str \
                + y_param_csb_str \
                + max_clut_size_str \
                + uf_1in_area_size_str \
                + uf_2in_area_size_str \
                + uf_3in_area_size_str \
                + uf_4in_area_size_str \
                + clut2_incr_str \
                + clut3_incr_str \
                + clut4_incr_str \
                + custom_lib_str \
                + variant_str \
                + timing_str \
                + tool_flags_str \
                + report_str

        with open(dir_path + "/test_code.sh", "w") as test_script:
            test_script.write("#!/usr/bin/env bash\n")
            test_script.write("cd \"$(dirname \"${BASH_SOURCE[0]}\")\"\n")
            test_script.write("chmod 777 "+str(CFG.binary)+"\n")
            test_script.write("\n"+str(tool_cmd)+"\n")

        #with open(group_path + "/fire_test_runs.sh", "a") as regr_script:
            #regr_script.write("\n## Calling RIPPER for: " +
                              #str(bench.name) + "\n")                            
            #regr_script.write("pushd " + dir_path + "\n")                           
            #regr_script.write("chmod 777 ./test_code.sh\n")
            #regr_script.write("source /apps/settings\n")
            #regr_script.write("sh ./test_code.sh &\n")
            #regr_script.write("echo $! >> ../pid_list_" +
             #                 str(CFG.bench_group) + ".txt\n")
            #regr_script.write("popd\n")
            #regr_script.write("###JOB_DELIMITER###\n")
    with open(group_path + "/fire_test_runs.sh", "a") as regr_script:
        regr_script.write("base_directory=\"./\"  # Replace with your actual base directory\n")
        regr_script.write("for subdir in \"$base_directory\"/test_run_*; do\n")
        regr_script.write("	sem -j 30 \"sh $subdir/test_code.sh\" &\n")
        regr_script.write("done\n")
        regr_script.write("wait\n")
                                     
            

    if CFG.fire_runs == "true":
        print("\n-- Running RIPPER on " +
              str(len(benchmark_list_passed)) + " benchmarks...")
        regr_cmd = "cat " + group_path + "/fire_test_runs.sh | parallel -j 20"
        #os.system(regr_cmd)
    else:
        print("\n-- Execute bash script goto: " + str(group_path) +
              "-> chmod +x fire_test_runs.sh && parallel -j 20 ::: ./fire_test_runs.sh to start RIPPER regression runs. Exiting...\n")
        quit()

# if CFG.fire_runs == "true":
#    print("\n-- RIPPER regression run passed for " + str(len(benchmark_list_passed)) + ". Exiting...\n")

# print("\n-- Exiting...\n")
