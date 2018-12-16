CXX=clang++
CC=clang


APP=example
APP_OBJS=FPGrowth.o 


OPTLEVEL = -O3


CFLAGS = -Ifpgrowth/ -Wall -std=c++11 $(OPTLEVEL)


example: main.cc $(APP_OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: fpgrowth/%.cc
	$(CXX) -c $(CFLAGS) $< -o $@


.PHONY: clean run
clean:
	rm -f *.o $(APP)

run: example
	./example --db 2010.csv -r

