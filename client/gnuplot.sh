#!/bin/bash
# Data='progress_report.txt'
gnuplot --persist -s -e "
set ylabel 'hours';
set yrange [0:*];

set xlabel 'date';
set xdata time;
set xtics format '';
set timefmt '%Y-%m-%d';

# average over N values
N = 250;
array Avg[N];
array X[N];

MovAvg(col) = (Avg[(t-1)%N+1]=column(col), n = t<N ? t : N, t=t+1, (sum [i=1:n] Avg[i])/n);
MovAvgCenterX(col) = (X[(t-1)%N+1]=column(col), n = t<N ? t%2 ? NaN : (t+1)/2 : ((t+1)-N/2)%N+1, n==n ? X[n] : NaN)   # be aware: gnuplot does integer division here;

# set datafile missing NaN;

plot 'progress_report.txt' using 1:2 w linespoints, t=1 '' u 1:(MovAvg(2)) w l lc rgb 'red' ti sprintf('Moving average over %d', N);
"
