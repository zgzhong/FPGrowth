CXX=clang++
CC=clang


APP=oneshot
APP_OBJS=FPGrowth.o FPTable.o FPNode.o


CFLAGS = -Ifpgrowth/ -Wall -std=c++11  -g


oneshot: main.cc $(APP_OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: fpgrowth/%.cc
	$(CXX) -c $(CFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -f *.o $(APP)
