# CS 111 Lab 4 Makefile

CC = gcc
#CFLAGS = -O4 -pthread -g -Wall -Wextra -Werror -Wno-unused
CFLAGS = -pthread -g -Wall -Wextra
DIR = lab4-$(USER)

ADDTEST_SOURCES = main_add.c
SLTEST_SOURCES = main_sl.c

ADDTEST_OBJECTS = $(subst .c,.o,$(ADDTEST_SOURCES))
SLTEST_OBJECTS = $(subst .c,.o,$(SLTEST_SOURCES))

DIST_SOURCES = $(ADDTEST_SOURCES) $(SLTEST_SOURCES) Makefile README checkdist \
	SortedList.h

all: addtest sltest

addtest: $(ADDTEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(ADDTEST_OBJECTS)

sltest: $(SLTEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(SLTEST_OBJECTS)

dist: $(DIR).tar.gz

$(DIR).tar.gz: $(DIST_SOURCES)
	rm -rf $(DIR)
	tar -czf $@.tmp --transform='s,^,$(DIR)/,' $(DIST_SOURCES)
	./checkdist $(DIR)
	mv $@.tmp $@

check: test.sh
	./test.sh

clean:
	rm -rf *~ *.o *.tar.gz addtest sltest $(DIR) *.tmp