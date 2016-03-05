# CS 111 Lab 4 Makefile

CC = gcc
#CFLAGS = -O4 -pthread -g -Wall -Wextra -Werror -Wno-unused
CFLAGS = -pthread -g -Wall -Wextra
DIR = lab4-$(USER)

ADDTEST_SOURCES = \
  main.c

ADDTEST_OBJECTS = $(subst .c,.o,$(ADDTEST_SOURCES))

DIST_SOURCES = $(ADDTEST_SOURCES) Makefile README checkdist

addtest: $(ADDTEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(ADDTEST_OBJECTS)

dist: $(DIR).tar.gz

$(DIR).tar.gz: $(DIST_SOURCES)
	rm -rf $(DIR)
	tar -czf $@.tmp --transform='s,^,$(DIR)/,' $(DIST_SOURCES)
	./checkdist $(DIR)
	mv $@.tmp $@

check: test.sh
	./test.sh

clean:
	rm -rf *~ *.o *.tar.gz addtest $(DIR) *.tmp