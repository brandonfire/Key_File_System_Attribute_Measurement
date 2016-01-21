c = gcc
CFLAGS = -D_GNU_SOURCE	          
LDFLAGS =  -lpthread

TARGETS = filesystem

all: bench
bench: $(TARGETS)

clean:
		rm -f $(TARGETS)

.PHONY: all bench
