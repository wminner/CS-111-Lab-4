#!/bin/bash

make clean
make

TESTNUM=5
PASSNUM=0

# Test 1 - single thread (expect success)
(./addtest --iter=10000 --thread=1) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 1/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 1/$TESTNUM failed!?"
fi

# Test 2 - multiple threads (expect failure)
(./addtest --yield=1 --iter=10000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a -s err ]
then
	echo "Test 2/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 2/$TESTNUM failed!?"
fi

# Test 3 - mutex synchronization
(./addtest --yield=1 --sync=m --iter=10000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 3/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 3/$TESTNUM failed!?"
fi

# Test 4 - spin lock synchronization
(./addtest --yield=1 --sync=s --iter=10000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 4/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 4/$TESTNUM failed!?"
fi

# Test 5 - compare and swap synchronization
(./addtest --yield=1 --sync=c --iter=10000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 5/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 5/$TESTNUM failed!?"
fi

# Clean up files
rm out err

# Check if all tests passed
if [ $PASSNUM -eq $TESTNUM ]
then
    echo "ALL TESTS PASSED!!!"
else
    echo "SOME TESTS FAILED???"
fi