#!/bin/bash
# Performs tests on both addtest and sltest

make clean
make

TESTNUM=9
PASSNUM=0

################################################################################
# Part 1 Tests
################################################################################

# Test 1 - addtest single thread (expect success)
(./addtest --iter=10000 --thread=1) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 1/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 1/$TESTNUM failed!?"
fi

# Test 2 - addtest multiple threads (expect failure)
(./addtest --yield=1 --iter=10000 --threads=20) 1> out 2> err
if [ $? -eq 0 -a -s err ]
then
	echo "Test 2/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 2/$TESTNUM failed!?"
fi

# Test 3 - addtest mutex synchronization
(./addtest --yield=1 --sync=m --iter=10000 --threads=20) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 3/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 3/$TESTNUM failed!?"
fi

# Test 4 - addtest spin lock synchronization
(./addtest --yield=1 --sync=s --iter=10000 --threads=20) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 4/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 4/$TESTNUM failed!?"
fi

# Test 5 - addtest compare and swap synchronization
(./addtest --yield=1 --sync=c --iter=10000 --threads=20) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 5/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 5/$TESTNUM failed!?"
fi

################################################################################
# Part 2 Tests
################################################################################

# Test 6 - sltest single thread (expect success)
(./sltest --iter=1000 --thread=1) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 6/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 6/$TESTNUM failed!?"
fi

# Test 7 - sltest multiple threads (expect failure)
(./sltest --yield=ids --iter=1000 --threads=10) 1> out 2> err
if [ $? -eq 1 -a -s err ]
then
	echo "Test 7/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 7/$TESTNUM failed!?"
fi

# Test 8 - sltest mutex synchronization
(./sltest --yield=ids --sync=m --iter=1000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 8/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 8/$TESTNUM failed!?"
fi

# Test 9 - sltest spin lock synchronization
(./sltest --yield=ids --sync=s --iter=1000 --threads=10) 1> out 2> err
if [ $? -eq 0 -a ! -s err ]
then
	echo "Test 9/$TESTNUM passed"
	PASSNUM=$((PASSNUM+1))
else
	echo "Test 9/$TESTNUM failed!?"
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