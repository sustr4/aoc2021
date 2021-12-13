set terminal pdf
set output 'day13.pdf'
set xrange [-10:50]
set yrange [-10:15]
plot 'day13.dat' u 1:2 w p pt 5
