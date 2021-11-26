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

all	: bin/cb
	@echo -n ""

# optimized version
bin/cb	: main_opt.o strc_opt.o lib
			$(CC) $(OPTFLAGS) main_opt.o -ltm_usage -Llib -o bin/cb
main_opt.o : src/main.cpp lib/tm_usage.h
			$(CC) $(CXXFLAGS) $(CFLAGS) $(OPTFLAGS) $< -Ilib -o $@
strc_opt.o	: src/Structs.cpp 
			$(CC) $(CXXFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@
lib: lib/libtm_usage.a
# cb_opt.o	: src/cb.cpp 
# 			$(CC) $(CXXFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@
lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*