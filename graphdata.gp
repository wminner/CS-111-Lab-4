# To use, execute "gnuplot graphdata.gp" from the shell
# or "load 'graphdata.gp'" from within gnuplot

################################################################################
# Part 1 Graphs (addtest)
################################################################################

# Part 1 - Graph 1 (data set 1)
set title "Average Time per Operation vs Number of Iterations\n{/*.9 addtest - \
Graph 1, one thread, yield off}"
set terminal pdf
set style data linespoints
set xlabel "Number of Iterations"
set ylabel "Average Time per Operation (ns)"
set output "p1d1.pdf"
plot "p1d1.dat" using 1:2 title "Sync Off"

# Part 1 - Graph 2.1 (data set 2-5)
set title "Average Time per Operation vs Number of Threads\n{/*.9 addtest - \
Graph 2.1, iterations sufficiently high (1000), yield on}"
set xlabel "Number of Threads"
set output "p1d2-5.pdf"
plot "p1d2.dat" using 1:2 title "Sync Off", \
	 "p1d3.dat" using 1:2 title "Sync Mutex", \
     "p1d4.dat" using 1:2 title "Sync Spin-Lock", \
     "p1d5.dat" using 1:2 title "Sync Compare Swap"

# Part 1 - Graph 2.2 (data set 2)
set title "Average Time per Operation vs Number of Threads\n{/*.9 addtest - \
Graph 2.2, iterations sufficiently high (1000), yield on}"
set output "p1d2.pdf"
plot "p1d2.dat" using 1:2 title "Sync Off"

# Part 1 - Graph 2.3 (data set 3)
set title "Average Time per Operation vs Number of Threads\n{/*.9 addtest - \
Graph 2.3, iterations sufficiently high (1000), yield on}"
set output "p1d3.pdf"
plot "p1d3.dat" using 1:2 title "Sync Mutex"

# Part 1 - Graph 2.4 (data set 4)
set title "Average Time per Operation vs Number of Threads\n{/*.9 addtest - \
Graph 2.4, iterations sufficiently high (1000), yield on}"
set output "p1d4.pdf"
plot "p1d4.dat" using 1:2 title "Sync Spin-Lock"

# Part 1 - Graph 2.5 (data set 5)
set title "Average Time per Operation vs Number of Threads\n{/*.9 addtest - \
Graph 2.5, iterations sufficiently high (1000), yield on}"
set output "p1d5.pdf"
plot "p1d5.dat" using 1:2 title "Sync Compare Swap"

################################################################################
# Part 2 Graphs (sltest)
################################################################################

# Part 2 - Graph 3 (data set 6)
set title "Average Time per Operation vs Number of Iterations\n{/*.9 sltest - \
Graph 3, one thread, yield off}"
set xlabel "Number of Iterations"
set output "p2d6.pdf"
plot "p2d6.dat" using 1:2 title "Sync Off"

# Part 2 - Graph 4.1 (data set 7-9)
set title "Average Time per Operation vs Number of Threads\n{/*.9 sltest - \
Graph 4.1, iterations sufficiently high (1000), yield off}"
set xlabel "Number of Threads"
set output "p2d7-9.pdf"
plot "p2d7.dat" using 1:2 title "Sync Off", \
	 "p2d8.dat" using 1:2 title "Sync Mutex", \
	 "p2d9.dat" using 1:2 title "Sync Spin-Lock"

# Part 2 - Graph 4.4 (data set 7)
#set title "Average Time per Operation vs Number of Threads\n{/*.9 sltest - \
#Graph 4.4, iterations sufficiently high (1000), one thread (due to no sync), \
yield off}"
#set output "p2d7.pdf"
#plot "p2d7.dat" using 1:2 title "Sync Off"

# Part 2 - Graph 4.2 (data set 8)
set title "Average Time per Operation vs Number of Threads\n{/*.9 sltest - \
Graph 4.2, iterations sufficiently high (1000), yield off}"
set output "p2d8.pdf"
plot "p2d8.dat" using 1:2 title "Sync Mutex"

# Part 2 - Graph 4.3 (data set 9)
set title "Average Time per Operation vs Number of Threads\n{/*.9 sltest - \
Graph 4.3, iterations sufficiently high (1000), yield off}"
set output "p2d9.pdf"
plot "p2d9.dat" using 1:2 title "Sync Spin-Lock"

# Part 2 - Graph 5.1 (data set 10-12)
set title "Average Time per Operation vs Ratio of Threads per List\n \
{/*.9 sltest - Graph 5.1, iterations sufficiently high (1000), yield off}"
set xlabel "Ratio of Threads per List"
set output "p2d10-12.pdf"
plot "p2d10.dat" using 1:2 title "Sync Off", \
	 "p2d11.dat" using 1:2 title "Sync Mutex", \
	 "p2d12.dat" using 1:2 title "Sync Spin-Lock"

# Part 2 - Graph 5.4 (data set 10)
#set title "Average Time per Operation vs Ratio of Threads per List\n \
#{/*.9 sltest - Graph 5.4, iterations sufficiently high (1000), one thread \
#(due to no sync), yield off}"
#set output "p2d10.pdf"
#plot "p2d10.dat" using 1:2 title "Sync Off"

# Part 2 - Graph 5.2 (data set 11)
set title "Average Time per Operation vs Ratio of Threads per List\n \
{/*.9 sltest - Graph 5.2, iterations sufficiently high (1000), yield off}"
set output "p2d11.pdf"
plot "p2d11.dat" using 1:2 title "Sync Mutex"

# Part 2 - Graph 5.3 (data set 12)
set title "Average Time per Operation vs Ratio of Threads per List\n \
{/*.9 sltest - Graph 5.3, iterations sufficiently high (1000), yield off}"
set output "p2d12.pdf"
plot "p2d12.dat" using 1:2 title "Sync Spin-Lock"

# Necessary to push write buffer to pdf file
set output