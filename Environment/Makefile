CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -fPIC
LDFLAGS=-shared

SRCS=anteater_sw.cpp
OBJS=$(SRCS:.cpp=.o)
SHARED_LIB=libanteater_sw.so

.PHONY: all shared object clean

all: shared object

shared: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

object: $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SHARED_LIB) $(OBJS)
