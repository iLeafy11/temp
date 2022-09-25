CC = gcc
CFLAGS = -O1 -g -Wall -I.

#GIT_HOOKS := .git/hooks/applied
#all: $(GIT_HOOKS) main

all: main

OBJS := main.o xs.o 

deps := $(OBJS:%.o=.%.o.d)

main: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MF .$@.d $<

valgrind_existence:
	@which valgrind 2>&1 > /dev/null || (echo "FATAL: valgrind not found"; exit 1)

valgrind: valgrind_existence
	# Explicitly disable sanitizer(s)
	$(MAKE) clean SANITIZER=0 main
	valgrind --leak-check=full ./main
	@echo

clean:
	rm -f $(OBJS) $(deps) *~ main /tmp/main.*
	rm -rf *.dSYM
	(cd traces; rm -f *~)

-include $(deps)
