CXX=g++
CC=gcc


APP=example
APP_OBJS=FPGrowth.o 
APP_LIBS=libfpgrowth.a


OPTLEVEL = -O3


CFLAGS = -Ifpgrowth/ -Wall -s -std=c++11 $(OPTLEVEL)

VPATH = fpgrowth

example: main.cc $(APP_LIBS)
	$(CXX) $(CFLAGS) $^ -o $@ -L. -lfpgrowth

$(APP_LIBS): $(APP_OBJS)
	ar cr libfpgrowth.a $^

.cc.o:
	$(CXX) -c $(CFLAGS) $< -o $@



.PHONY: clean run lib
clean:
	rm -f *.o $(APP) $(APP_LIBS)

run: example
	./$^ --db 2010.csv -r
	@echo ""
	./$^ --db 6459.csv -r

