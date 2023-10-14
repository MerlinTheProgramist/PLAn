#!/bin/bash
# Data='progress_report.txt'
gnuplot --persist -s -e "
set ylabel 'hours';
set yrange [0:*];

set xlabel 'date';
set xdata time;
set timefmt '%Y-%m-%d';

plot 'work_report.txt' using 1:2 w l lc rgb 'red' ti 'progress';
"
