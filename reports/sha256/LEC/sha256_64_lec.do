set dofile abort exit
setenv module_name "sha256"
setenv verify_lec_path "../reports/sha256/LEC"
read library -liberty /home/UFAD/a.chatterjee/Desktop/lab_work/astra/freepdk/gscl45nm.lib
read design -golden original_flat_64.v
read design -revised revised_flat_64.v
set system mode setup 
add pin constraints 1 sel  -module sha256  -revised
add ignore outputs pufout* -module sha256  -revised
report primary outputs > "$module_name"_64.primary_outputs.rpt
set system mode lec > "$module_name"_64.system_mode_lec.rpt
add compared points -all -golden
compare
report verification -verbose > "$module_name"_64.verification.rpt
report statistics > "$module_name"_64.statistics.rpt
exit -f
