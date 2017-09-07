# minichess

A 4X4 chess engine for analyzing a game
================================================================

Game Rules:
-----------
This chess engine took birth while I was attempting to solve simplified chess problem on HackerRank(https://www.hackerrank.com/challenges/simplified-chess-engine/problem)

The game rules are as follows
1) Each player has at-least one Queen, at most 2 Rooks and at most 2 minor pieces i.e Bishop & Knight.
2) The game is always started by a white player.
3) The engine is supposed to answer "YES" if white can win(black queen can be captured) in "m" number of moves.
4) Even black plays optimal moves.

Source Tree structure:
---------------------
Source tree consists of the following
 1) movegen.cpp - An offline tool that can be used for generating move vectors, masks .
 2) chess.cpp - The crux of the chess engine algorithm is available in this file.
 3) Makefile - A simple makefile for building the engine
 4) input.<files> - Sample test cases for this engine.

How does ir it work ?
_____________________
 1) A bit board is maintained for representing positions of various black and white pieces.
 2) For a 4X4 board, using uint16_t should suffice. 
 3) White and black boards are seperately maintained. Game board at any point in time is bitwise "OR" of white & black board.
 4) The move vectors and occlusion masks table for sliding pieces (bishop&rook) are generated offline.
 5) Queen moves are determined by doing bitwise "OR" of rook and bishop moves.
 6) The crux of the engine is minimax. Minimax is run for a givem depth for maximizing the outcome and minimizing the loss.
 7) Alpha, beta pruning is inplemented for pruning the search space.
 8) Game score at any point in time is white board score minus the black board score.
 9) A board score is sum of weights of its pieces with Queen having the highest rank and Knight having the lowest rank.
 
How to build:
------------
 1) cd minichess/ - Get into source directory.
 2) make - Builds non debugable version of chess engine.
 3) CFLAGS='-DDEBUG=1' make - Builds debugable version of chess engine. It includes additional logs for game analysis.
 4) CFLAGS='-DPLAYSELF=1' make - Builds a chess engine that plays optimal moves for both black and white.

Input file format:
------------------
1) The first line consists of no of games G to be played
2) The following consists of three lines w,b &m where w represents no of white pieces, b represents no of black pieces and m represents no of moves
3) The subsequent w lines consists of white pieces in the format <piece> <file> <rank> where piece is one of ['Q', 'R', 'B', 'N'] and file is one of ['A', 'B', 'C', 'D'] and rank is one of [1,2,3,4]
4) The subsequent b lines consists of black pieces in the format <piece> <file> <rank> where piece is one of ['Q', 'R', 'B', 'N'] and file is one of ['A', 'B', 'C', 'D'] and rank is one of [1,2,3,4]

Eg input file:
--------------
```
1
2 1 1
N B 2
Q B 1
Q A 4

Starting program: /home/vijay/minichess/chess input.2
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.21-8.fc22.x86_64
YES
Game start

bQ -- -- --
-- -- -- --
-- wN -- --
-- wQ -- --
Player 1 white's move

wN -- -- --
-- -- -- --
-- -- -- --
-- wQ -- --
Black Queen has been captured
```
