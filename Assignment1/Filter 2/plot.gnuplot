set datafile separator ","
set title "Filtered ECG"
set xlabel "time"
set ylabel "data"
set xrange [0:1000]
set yrange [400:600]

plot "FilteredSamples.csv" with l
pause 50

#cmd to plot:  gnuplot plot.gnuplot

