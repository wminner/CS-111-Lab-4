#!/usr/local/cs/bin/python3.4
# Logs data in .dat files according to graphs required in the spec.
# Call this script with numbers after it (separated by spaces) to collect data
# for only specific graphs.
# Use "./logdata.sh 2 3" gets data for graphs 2 and 3 only
# Use "./logdata.sh" to collect all data

# NOTE: data collection for 8, 9, 11, and 12 take a fairly long time to complete
# (may be over a minute for each)

import os
import sys
import subprocess
import math
import re

subprocess.call("make clean;", shell=True)
subprocess.call("make;", shell=True)

MAX_THR = 25
SUFF_HIGH_ADD_ITER = 1000
SUFF_HIGH_SL_ITER = 1000
WHITE_SPACE = MAX_THR - 6

write_sum = open('temp_file', 'w')
read_sum = open('temp_file', 'r')

pattern = re.compile('per operation: ([0-9]+) ns')

# Handle specific graph requests
# If no args given, then compute all graph data
if len(sys.argv) == 1:
	sys.argv.append('0')

for request in sys.argv[1:]:
	if request == '0':
		print("Computing data for all graphs...")
	else:
		print("Computing data for graph {0}...".format(request))

################################################################################
# Part 1 Tests (addtest)
################################################################################
	
	part_num = 1
	data_num = 1

	# Part 1 - Data 1
	# x = iterations, y = average time per operation
	# threads = 1, yield off, sync off
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Iterations 30 - 1000 (steps of 10)
		for i in range(3, 101):
			NUM_ITER = i * 10
			for j in range(5):
				process_string = "./addtest --iter={0} --threads=1;".format(NUM_ITER)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_ITER, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 1 - Data 2
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync off
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./addtest --yield=1 --iter={0} --threads={1} 2> /dev/null;".format(SUFF_HIGH_ADD_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 1 - Data 3
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync mutex
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./addtest --sync=m --yield=1 --iter={0} --threads={1};".format(SUFF_HIGH_ADD_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 1 - Data 4
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync spin-lock
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./addtest --sync=s --yield=1 --iter={0} --threads={1};".format(SUFF_HIGH_ADD_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 1 - Data 5
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield on, sync compare and swap
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./addtest --sync=c --yield=1 --iter={0} --threads={1};".format(SUFF_HIGH_ADD_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

################################################################################
# Part 2 Tests (sltest)
################################################################################

	part_num = 2

	# Part 2 - Data 6
	# x = iterations, y = average time per operation
	# threads = 1, yield off, sync off
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Iterations 30 - 1000 (steps of 10)
		for i in range(3, 101):
			NUM_ITER = i * 10
			for j in range(5):
				process_string = "./sltest --iter={0} --threads=1;".format(NUM_ITER)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_ITER, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 7
	# x = threads (1 because unprotected), y = average time per operation
	# iterations sufficiently high, yield off, sync off
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Threads 1
		for i in range(1):
			NUM_THR = 1
			for j in range(5):
				process_string = "./sltest --iter={0} --threads={1};".format(SUFF_HIGH_SL_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 8
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield off, sync mutex
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Progress bar
		print("|0%", end="")
		for k in range(WHITE_SPACE):
			print(" ", end="")
		print("100%|\n|", end="")

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./sltest --sync=m --iter={0} --threads={1};".format(SUFF_HIGH_SL_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))
			print(".", end="", flush=True)

		print("|\nData Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 9
	# x = threads, y = average time per operation
	# iterations sufficiently high, yield off, sync spin-lock
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Progress bar
		print("|0%", end="")
		for k in range(WHITE_SPACE):
			print(" ", end="")
		print("100%|\n|", end="")

		# Threads 1 to max threads (25)
		for i in range(1, MAX_THR+1):
			NUM_THR = i
			for j in range(5):
				process_string = "./sltest --sync=s --iter={0} --threads={1};".format(SUFF_HIGH_SL_ITER, NUM_THR)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(NUM_THR, AVG))
			print(".", end="", flush=True)

		print("|\nData Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 10
	# x = ratio threads:lists (1 because unprotected), y = average time per operation
	# iterations sufficiently high, yield off, sync off
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Lists 1
		for i in range(1):
			NUM_THR = 1
			NUM_LIST = 1
			THR_LIST_RATIO = 1
			for j in range(5):
				process_string = "./sltest --iter={0} --threads={1} --lists={2};".format(SUFF_HIGH_SL_ITER, NUM_THR, NUM_LIST)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(THR_LIST_RATIO, AVG))

		print("Data Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 11
	# x = ratio threads:lists, y = average time per operation
	# iterations sufficiently high, yield off, sync mutex
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Progress bar
		print("|0%", end="")
		for k in range(WHITE_SPACE):
			print(" ", end="")
		print("100%|\n|", end="")

		# Lists/thread ratio 1 to 1/25
		for i in range(1, MAX_THR+1):
			NUM_THR = MAX_THR
			NUM_LIST = i
			THR_LIST_RATIO = round(NUM_THR/NUM_LIST, 2)
			for j in range(5):
				process_string = "./sltest --sync=m --iter={0} --threads={1} --lists={2};".format(SUFF_HIGH_SL_ITER, NUM_THR, NUM_LIST)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(THR_LIST_RATIO, AVG))
			print(".", end="", flush=True)

		print("|\nData Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Part 2 - Data 12
	# x = ratio threads:list, y = average time per operation
	# iterations sufficiently high, yield off, sync spin-lock
	if request == str(data_num) or request == '0':
		SUM = 0
		data_file = open("p{0}d{1}.dat".format(part_num, data_num), 'w')

		# Progress bar
		print("|0%", end="")
		for k in range(WHITE_SPACE):
			print(" ", end="")
		print("100%|\n|", end="")

		# Lists/thread ratio 1 to 1/25
		for i in range(1, MAX_THR+1):
			NUM_THR = MAX_THR
			NUM_LIST = i
			THR_LIST_RATIO = round(NUM_THR/NUM_LIST, 2)
			for j in range(5):
				process_string = "./sltest --sync=s --iter={0} --threads={1} --lists={2};".format(SUFF_HIGH_SL_ITER, NUM_THR, NUM_LIST)
				subprocess.call(process_string, shell=True, stdout=write_sum)
				match = pattern.search(read_sum.read())
				if match:
					SUM += int(match.group(1))

			AVG = math.floor(SUM/5)
			SUM = 0
			data_file.write("{0}\t{1}\n".format(THR_LIST_RATIO, AVG))
			print(".", end="", flush=True)

		print("|\nData Set {0} DONE: placed in p{1}d{2}.dat.".format(data_num, part_num, data_num))

	data_num += 1

	# Clean up temp files
	#subprocess.call("rm temp_file;", shell=True)
	os.remove("temp_file")