CXX=clang++
CC=clang


APP=oneshot
APP_OBJS=FPGrowth.o 


OPTLEVEL = -O3


CFLAGS = -Ifpgrowth/ -Wall -std=c++11 $(OPTLEVEL)


oneshot: main.cc $(APP_OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: fpgrowth/%.cc
	$(CXX) -c $(CFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -f *.o $(APP)
