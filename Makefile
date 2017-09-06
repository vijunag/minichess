all: movegen chess chess2

ifeq ($(SELFGAME), )
CFLAGS +=
else
CFLAGS +=-DPLAYSELF
endif

movegen: movegen.cpp
	g++ -DCMDLINE=1  $(CFLAGS) -g -O0 -I. $^ -o $@

chess: chess.cpp
	g++ -DCMDLINE=1 $(CFLAGS) -g -O0 -I. $^ -o $@

chess2: chess2.cpp
	g++ -DCMDLINE=1 $(CFLAGS) -g -O0 -I. $^ -o $@

clean:
	@rm -rf movegen
	@rm -rf chess
	@rm -rf chess2
