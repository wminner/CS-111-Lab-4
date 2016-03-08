#!/bin/bash
# Logs data in .dat files according to graphs required in the spec.
# Call this script with numbers after it (separated by spaces) to collect data
# for only specific graphs.
# Use "./logdata.sh 2 3" gets data for graphs 2 and 3 only
# Use "./logdata.sh 0" to collect all data

# NOTE: data collection for 8, 9, 11, and 12 take a fairly long time to complete
# (may be over a minute for each)

make clean
make

MAX_THR=25
SUFF_HIGH_ADD_ITER=1000
SUFF_HIGH_SL_ITER=1000

WHITE_SPACE=$((MAX_THR-6)) # For progress bar (when data collection long)

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

################################################################################
# Part 1 Tests (addtest)
################################################################################

	# Part 1 - Data 1
	# x = iterations, y = average time per operation
	# threads = 1, yield off, sync off
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
		printf "Data Set 1 DONE: placed in p1d1.dat.\n"
	fi

	# Part 1 - Data 2
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync off
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
				TEMP=$(((./addtest --yield-1 --iter=$SUFF_HIGH_ADD_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d2.dat
		done
		printf "Data Set 2 DONE: placed in p1d2.dat.\n"
	fi

	# Part 1 - Data 3
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync mutex
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
				TEMP=$(((./addtest --sync=m --yield-1 --iter=$SUFF_HIGH_ADD_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d3.dat
		done
		printf "Data Set 3 DONE: placed in p1d3.dat.\n"
	fi

	# Part 1 - Data 4
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync spin-lock
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
				TEMP=$(((./addtest --sync=s --yield-1 --iter=$SUFF_HIGH_ADD_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d4.dat
		done
		printf "Data Set 4 DONE: placed in p1d4.dat.\n"
	fi

	# Part 1 - Data 5
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync compare and swap
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
				TEMP=$(((./addtest --sync=c --yield-1 --iter=$SUFF_HIGH_ADD_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p1d5.dat
		done
		printf "Data Set 5 DONE: placed in p1d5.dat.\n"
	fi

################################################################################
# Part 2 Tests (sltest)
################################################################################

	# Part 2 - Data 6
	# x = iterations, y = average time per operation
	# threads = 1, yield off, sync off
	if [ $REQUEST -eq 6 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d6.dat ]
		then
			rm p2d6.dat
		fi
		SUM=0

		for i in `seq 3 100`;
		do
			let NUM_ITER=i*10
			for j in `seq 1 5`;
			do
				TEMP=$((./sltest --iter=$NUM_ITER --threads=1) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_ITER\t$AVG\n"
			printf "$NUM_ITER\t$AVG\n" >> p2d6.dat
		done
		printf "Data Set 6 DONE: placed in p2d6.dat.\n"
	fi

	# Part 2 - Data 7
	# x = threads (1 because unprotected), y = average time per operation
	# iterations sufficiently high, yield off, sync off
	if [ $REQUEST -eq 7 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d7.dat ]
		then
			rm p2d7.dat
		fi
		SUM=0

		for i in `seq 1 1`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p2d7.dat
		done
		printf "Data Set 7 DONE: placed in p2d7.dat.\n"
	fi

	# Part 2 - Data 8
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield off, sync mutex
	if [ $REQUEST -eq 8 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d8.dat ]
		then
			rm p2d8.dat
		fi
		SUM=0

		# Progress bar
		printf "|0%%"
		for k in `seq 1 $WHITE_SPACE`;
		do
			printf " "
		done
		printf "100%%|\n|"

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --sync=m --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p2d8.dat
			printf "."
		done
		printf "|\nData Set 8 DONE: placed in p2d8.dat.\n"
	fi

	# Part 2 - Data 9
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield off, sync spin-lock
	if [ $REQUEST -eq 9 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d9.dat ]
		then
			rm p2d9.dat
		fi
		SUM=0
		
		# Progress bar
		printf "|0%%"
		for k in `seq 1 $WHITE_SPACE`;
		do
			printf " "
		done
		printf "100%%|\n|"

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=i
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --sync=s --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$NUM_THR\t$AVG\n"
			printf "$NUM_THR\t$AVG\n" >> p2d9.dat
			printf "."
		done
		printf "|\nData Set 9 DONE: placed in p2d9.dat.\n"
	fi

	# Part 2 - Data 10
	# x = ratio threads:lists (1 because unprotected), y = average time per operation
	# iterations sufficiently high , yield off, sync off
	if [ $REQUEST -eq 10 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d10.dat ]
		then
			rm p2d10.dat
		fi
		SUM=0

		for i in `seq 1 1`;
		do
			let NUM_THR=i
			let NUM_LIST=i
			let THR_LIST_RATIO=1
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR --lists=$NUM_LIST) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$THR_LIST_RATIO\t$AVG\n"
			printf "$THR_LIST_RATIO\t$AVG\n" >> p2d10.dat
		done
		printf "Data Set 10 DONE: placed in p2d10.dat.\n"
	fi

	# Part 2 - Data 11
	# x = ratio threads:lists, y = average time per operation
	# iterations sufficiently high, yield off, sync mutex
	if [ $REQUEST -eq 11 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d11.dat ]
		then
			rm p2d11.dat
		fi
		SUM=0

		# Progress bar
		printf "|0%%"
		for k in `seq 1 $WHITE_SPACE`;
		do
			printf " "
		done
		printf "100%%|\n|"

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=MAX_THR
			let NUM_LIST=i
			THR_LIST_RATIO=$(bc <<<"scale=2;$NUM_THR/$NUM_LIST")
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --sync=m --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR --lists=$NUM_LIST) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$THR_LIST_RATIO\t$AVG\n"
			printf "$THR_LIST_RATIO\t$AVG\n" >> p2d11.dat
			printf "."
		done
		printf "|\nData Set 11 DONE: placed in p2d11.dat.\n"
	fi

	# Part 2 - Data 12
	# x = ratio threads:list, y = average time per operation
	# iterations sufficiently high, yield off, sync spin-lock
	if [ $REQUEST -eq 12 -o $REQUEST -eq 0 ]
	then
		if [ -e p2d12.dat ]
		then
			rm p2d12.dat
		fi
		SUM=0

		# Progress bar
		printf "|0%%"
		for k in `seq 1 $WHITE_SPACE`;
		do
			printf " "
		done
		printf "100%%|\n|"

		for i in `seq 1 $MAX_THR`;
		do
			let NUM_THR=MAX_THR
			let NUM_LIST=i
			THR_LIST_RATIO=$(bc <<<"scale=2;$NUM_THR/$NUM_LIST")
			for j in `seq 1 5`;
			do
				TEMP=$(((./sltest --sync=s --iter=$SUFF_HIGH_SL_ITER --threads=$NUM_THR --lists=$NUM_LIST) | sed -n -E "s/per operation: ([0-9]+) ns/\\1/p")2>/dev/null)
				SUM=$((SUM+TEMP))
			done
			AVG=$((SUM/5))
			SUM=0
			#printf "$THR_LIST_RATIO\t$AVG\n"
			printf "$THR_LIST_RATIO\t$AVG\n" >> p2d12.dat
			printf "."
		done
		printf "|\nData Set 12 DONE: placed in p2d12.dat.\n"
	fi

done # Handle specific graph requests