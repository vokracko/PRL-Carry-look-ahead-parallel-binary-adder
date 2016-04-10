set xlabel 'Počet bitů'
set ylabel 'Čas'
set title 'Čas v závisloti na počtu bitů'
set terminal postscript eps enhanced color font 14
set autoscale xfix
set output 'time.eps'
set style fill solid
set boxwidth 0.75
plot 'time.dat' using 2:xtic(1) with boxes notitle
