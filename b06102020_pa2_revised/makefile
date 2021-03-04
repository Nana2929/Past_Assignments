# CC and CFLAGS are variables
# https://www.gnu.org/software/make/manual/make.html
CC = g++
CXXFLAGS = -std=c++11
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option asks g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
OPTFLAGS = -O3

all	: bin/mps
	@echo -n ""

# optimized version
bin/mps	: mps_opt.o main_opt.o lib
			$(CC) $(OPTFLAGS) mps_opt.o main_opt.o -ltm_usage -Llib -o bin/mps
main_opt.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CXXFLAGS) $(CFLAGS) $< -Ilib -o $@
mps_opt.o	: src/max_planar_subset.cpp src/max_planar_subset.h
			$(CC) $(CXXFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

