.PHONY: tests TAGS

CONF    := -DBLOCK_SIZE=512 \
	-DNUM_BLOCKS=100

CC      := ~/optfs/gcc/bin/gcc
CXX     := ~/optfs/gcc/bin/g++
CXXFLAGS:= $(shell pkg-config --cflags fuse) -std=c++14  $(CONF)
LDFLAGS := $(shell pkg-config --libs   fuse)
#WARN    := -Wextra -pedantic -Wall -Wold-style-cast -Werror -Wc++11-compat
DEBUG   := -ggdb3
OPTIMIZE:= -O2

src=$(wildcard *.cc)
obj=$(patsubst %.cc,%.o,$(src))

BIN := sfuse

#all: $(BIN) TAGS

#TAGS:   
	#etags *.cc *.hpp /usr/include/fuse.h /usr/include/fuse/fuse.h

$(BIN): $(obj)
	$(CXX) $^ $(LDFLAGS)  -o $@

%.o : %.cc
	$(CXX) $(DEBUG) $< -c -MMD -MF $*.d -MT $*.o -MT $*.d $(CXXFLAGS) $(WARN) $(OPTIMIZE) -o $@

clean:
	-rm $(BIN)
	-rm *.o
	-rm *.d

-include $(wildcard *.d)

