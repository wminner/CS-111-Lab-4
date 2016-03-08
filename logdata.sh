#!/bin/bash
# Logs data in .dat files according to graphs required in the spec.
# Call this script with numbers after it (separated by spaces) to collect data
# for only specific graphs.
# Use "./logdata.sh 2 3" gets data for graphs 2 and 3 only
# Use "./logdata.sh 0" to collect all data

make clean
make

MAX_THR=25

# Handle specific graph requests
for i in "$@"
do
	REQUEST=$i
	if [ $REQUEST -eq 0 ]
	then
		printf "Computing data for all graphs...\n"
	else
		printf "Computing data for graph $REQUEST...\n"
	fi

	# Part 1 - Graph 1
	# x = number of iterations, y = average time per operation
	# threads = 1, yield off
	if [ $REQUEST -eq 1 -o $REQUEST -eq 0 ]
	then
		if [ -e p1d1.dat ]
		then
			rm p1d1.dat
		fi
		SUM=0

		for i in `seq 3 100`;
		do
			let NUM_ITER=i*10
			for j in `seq 1 5`;
			do
				TEMP=$((./addtest --iter=$NUM_ITER --threads=1) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_ITER\t$AVG\n"
			printf "$NUM_ITER\t$AVG\n" >> p1d1.dat
		done
		printf "DONE: Data placed in p1d1.dat.\n"
	fi

	# Part 1 - Graph 2
	# x = number of threads, y = average time per operation
	# iterations = 1000, yield on, sync off
	if [ $REQUEST -eq 2 -o $REQUEST -eq 0 ]
	then
		if [ -e p1d2.dat ]
		then
			rm p1d2.dat
		fi
		SUM=0

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./addtest --yield=1 --iter=1000 --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d2.dat
		done
		printf "DONE: Data placed in p1d2.dat.\n"
	fi

	# Part 1 - Graph 3
	# x = number of threads, y = average time per operation
	# iterations = 1000, yield on, sync mutex
	if [ $REQUEST -eq 3 -o $REQUEST -eq 0 ]
	then
		if [ -e p1d3.dat ]
		then
			rm p1d3.dat
		fi
		SUM=0

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./addtest --yield=1 --sync=m --iter=1000 --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d3.dat
		done
		printf "DONE: Data placed in p1d3.dat.\n"
	fi

	# Part 1 - Graph 4
	# x = number of threads, y = average time per operation
	# iterations = 1000, yield on, sync spin-lock
	if [ $REQUEST -eq 4 -o $REQUEST -eq 0 ]
	then
		if [ -e p1d4.dat ]
		then
			rm p1d4.dat
		fi
		SUM=0

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./addtest --yield=1 --sync=s --iter=1000 --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d4.dat
		done
		printf "DONE: Data placed in p1d4.dat.\n"
	fi

	# Part 1 - Graph 5
	# x = number of threads, y = average time per operation
	# iterations = 1000, yield on, sync compare swap
	if [ $REQUEST -eq 5 -o $REQUEST -eq 0 ]
	then
		if [ -e p1d5.dat ]
		then
			rm p1d5.dat
		fi
		SUM=0

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./addtest --yield=1 --sync=c --iter=1000 --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d5.dat
		done
		printf "DONE: Data placed in p1d5.dat.\n"
	fi

	# Part 2 - Graph 6
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 7
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 8
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 9
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 10
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 11
	# x = , y = 
	# iterations = , yield , sync 

	# Part 2 - Graph 12
	# x = , y = 
	# iterations = , yield , sync 

done # Handling specific graph requests