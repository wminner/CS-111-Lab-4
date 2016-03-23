#!/usr/local/cs/bin/python3.4
# Performs tests on both addtest and sltest

import sys, os
import subprocess

subprocess.call("make clean;", shell=True)
subprocess.call("make;", shell=True)

total_test = 11
pass_num = 0
test_num = 1

out = open("out_file", 'w')
err = open("err_file", 'w')

################################################################################
# Part 1 Tests
################################################################################

# Test 1 - addtest single thread (expect success)
exitval = subprocess.call("./addtest --iter=10000 --thread=1;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 2 - addtest multiple threads (expect failure)
exitval = subprocess.call("./addtest --yield=1 --iter=10000 --threads=20;", shell=True, stdout=out, stderr=err)
if exitval == 1 and os.stat("err_file").st_size != 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 3 - addtest mutex synchronization
exitval = subprocess.call("./addtest --yield=1 --sync=m --iter=10000 --threads=20;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 4 - addtest spin lock synchronization
exitval = subprocess.call("./addtest --yield=1 --sync=s --iter=10000 --threads=20;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 5 - addtest compare and swap synchronization
exitval = subprocess.call("./addtest --yield=1 --sync=c --iter=10000 --threads=20;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

################################################################################
# Part 2 Tests
################################################################################

# Test 6 - sltest single thread (expect success)
exitval = subprocess.call("./sltest --iter=1000 --thread=1;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 7 - sltest multiple threads (expect failure)
exitval = subprocess.call("./sltest --yield=ids --iter=1000 --threads=10;", shell=True, stdout=out, stderr=err)
if exitval == 1 and os.stat("err_file").st_size != 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 8 - sltest mutex synchronization
exitval = subprocess.call("./sltest --yield=ids --sync=m --iter=1000 --threads=10;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 9 - sltest spin lock synchronization
exitval = subprocess.call("./sltest --yield=ids --sync=s --iter=1000 --threads=10;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 10 - sltest multiple lists, mutex synchronization
exitval = subprocess.call("./sltest --yield=ids --sync=m --iter=1000 --threads=10 --lists=10;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Test 11 - sltest multiple lists, spin-lock synchronization
exitval = subprocess.call("./sltest --yield=ids --sync=s --iter=1000 --threads=10 --lists=10;", shell=True, stdout=out, stderr=err)
if exitval == 0 and os.stat("err_file").st_size == 0:
	result = 'passed'
	pass_num += 1
else:
	result = 'failed!?'
print("Test {0}/{1} {2}".format(test_num, total_test, result))
# Clear error file
err.seek(0)
err.truncate()
test_num += 1

# Clean up files
os.remove("out_file")
os.remove("err_file")

# Check if all tests passed
if pass_num == total_test:
	print("ALL TESTS PASSED!!!")
else:
	print("SOME TESTS FAILED???")