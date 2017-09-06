all: movegen chess

ifeq ($(PLAYSELF), )
CFLAGS +=
else
CFLAGS +=-DPLAYSELF
endif

movegen: movegen.cpp
	g++ -DCMDLINE=1  $(CFLAGS) -g -O0 -I. $^ -o $@

chess: chess.cpp
	g++ -DCMDLINE=1 $(CFLAGS) -g -O0 -I. $^ -o $@

clean:
	@rm -rf movegen
	@rm -rf chess
