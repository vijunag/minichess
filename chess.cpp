#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <unistd.h>

using namespace std;
int board[4][4];

uint16_t QueenMoves[4][4] = {
  {0xFCA9, 0xFE54, 0XF7A2, 0xF359},
  {0xCFCA, 0xEFE5, 0x7F7A, 0x3F35},
  {0xACFC, 0x5EFE, 0xA7F7, 0X53F3},
  {0x9ACF, 0x45EF, 0x2A7F, 0x953F}
};

uint16_t RookMoves[4][4] = {
  {0xF888, 0xF444, 0xF222, 0xF111},
  {0x8F88, 0x4F44, 0x2F22, 0x1F11},
  {0x88F8, 0x44F4, 0x22F2, 0x11F1},
  {0x888F, 0x444F, 0x222F, 0x111F}
};

uint16_t BishopMoves[4][4] = {
  {0x8421,0x4A10,0x2580,0x1248},
  {0x4842,0xA4A1,0x5258,0x2128},
  {0x2484,0x1A4A,0x8525,0x4212},
  {0x1248,0x01A4,0x852,0x8421}
};

uint16_t KnightMoves[4][4]= {
  {0x8240,0x41A0,0x2850,0x1420},
  {0x2824,0x141A,0x8285,0x4142},
  {0x4282,0xA141,0x5828,0x2414},
  {0x428,0xA14,0x582,0x241}
};

uint16_t (*moves[])[4] = { NULL, KnightMoves, BishopMoves, RookMoves, QueenMoves};

#define BSR(_val) ((__builtin_ffs(_val))-1)

#define ROWIDX(r) (3-BSR(r)/4)
#define COLIDX(c) (3-(BSR(c))%4)

typedef enum ColorType {
  BLACK = 0,
  WHITE = 1,
} ColorType;

typedef enum PieceType {
  INVALID = 0,
  KNIGHT = 1,
  BISHOP = 2,
  ROOK = 3,
  QUEEN = 4,
  MAX=5,
#define WINSCORE  QUEEN
#define LOSESCORE -QUEEN
} PieceType;

typedef struct Board {
  union {
    struct _bmap {
      uint16_t _invalid;
      uint16_t _kn;
      uint16_t _bi;
      uint16_t _ro;
      uint16_t _qu;
    } _bval;
    uint16_t bmap[MAX];
  } u;

#define kn u.bmap[1]
#define bi u.bmap[2]
#define ro u.bmap[3]
#define qu u.bmap[4]
#define bm u.bmap

  Board() {
    clearbits();
  }

  uint16_t getbits(void) {
    return kn | bi | ro | qu;
  }

  uint16_t set(PieceType p, int mask) {
    bm[p] = mask;
  }

  uint16_t clearbits(void) {
    u.bmap[0] = 0;
    kn = 0;
    bi = 0;
    ro = 0;
    qu = 0;
  }

} Board;

int GetScore(Board &b, uint16_t mmask)
{
  if (b.qu & mmask) //highest score
    return QUEEN;
  if (b.ro & mmask)
    return ROOK;
  if (b.kn & mmask)
    return KNIGHT;
  if (b.bi & mmask) //lowest score
    return BISHOP;

  return 0; //some intermediate score
}

int GameScore(Board b, Board w, int minimize)
{
  int score = GetScore(b, w.getbits());
  return minimize ? -score : score;
}

uint16_t GetMoves(PieceType p, Board &b)
{
  uint16_t (*m)[4] = moves[p];
  int r = ROWIDX(b.bm[p]);
  int c = COLIDX(b.bm[p]);
  uint16_t legal_moves = m[r][c];

  return ~(legal_moves&b.getbits())&legal_moves;
}

Board fbb, fwb; //final black & white boards
#define MINIMAX(_score, _b, _w, _m, OP, _fb)                \
({                                                          \
  int _best = _score;                                       \
  for (PieceType p=KNIGHT; p<MAX;p=(PieceType)(((int)p)+1)){\
    if (!w.bm[p]) {                                         \
      continue;                                             \
    }                                                       \
    uint16_t moves = GetMoves(p, _w);                       \
    for(uint16_t mask=0x8000;mask;mask>>=1) {               \
      if (moves&mask) {                                     \
        Board nb=_w;                                        \
        nb.set(p, mask);                                    \
        score=minimax(_b, nb, depth-1, _m);                 \
        if (score OP _best) {                               \
          _best = score;                                    \
          _fb = nb;                                         \
        }                                                   \
      }                                                     \
    }                                                       \
  }                                                         \
 return (_best);                                            \
 })

int minimax(Board b, Board w, int depth, int maximize)
{
  int score = GameScore(b, w, maximize);

  if ((WINSCORE==abs(score))||(LOSESCORE==abs(score))||!depth) {
    return score;
  }

  if (maximize) {
#if 0
    int _best = LOSESCORE;
    for (PieceType p=KNIGHT; p<MAX;p=(PieceType)(((int)p)+1)) {
      uint16_t moves = GetMoves(p, w);
      for(uint16_t mask=0x8000;mask;mask>>=1) {
        if (moves&mask) {
          Board nb=w;
          nb.set(p, mask);
          score=minimax(b, nb, depth-1, 0);
          if (score >= _best) {
            _best = score;
            nextboard = nb;
          }
        }
      }
    }
    return (_best);
#endif
    MINIMAX(LOSESCORE,b,w,0,>=,fwb);
  } else {
    MINIMAX(WINSCORE,w,b,1,<,fbb);
  }
}

#if 1

PieceType GetPieceType(char c)
{
  switch(c) {
   case 'Q': return QUEEN;
   case 'B': return BISHOP;
   case 'R': return ROOK;
   case 'N': return KNIGHT;
  }
  return MAX;
}

const char GetPieceName(PieceType p)
{
  switch(p) {
   case QUEEN:  return 'Q';
   case BISHOP: return 'B';
   case ROOK:   return 'R';
   case KNIGHT: return 'N';
  }
  return 'Z';
}

void PrintMoveBoard(PieceType p, int r, int c)
{
  uint16_t (*m)[4] = moves[p];
  uint16_t mask = 0x8000;

  printf("----PieceType=%c, pos=(%d,%d)-----\n",
      GetPieceName(p), r,c);
  int move = m[r][c];
  for(int i=0;i<4;++i) {
    for(int j=0;j<4;++j) {
      if (mask & move) {
        printf("%c", GetPieceName(p));
      } else {
        printf("-");
      }
      mask>>=1;
    }
    printf("\n");
  }
}

void PrintPossibleMoves(PieceType p)
{
  for(int i=0;i<4;++i)
    for(int j=0;j<4;++j)
      PrintMoveBoard(p, i, j);
}

void PrintGameBoard(Board b, Board w)
{
  uint16_t mask=0x8000;
  uint16_t bw=b.getbits();
  uint16_t wh=w.getbits();

  printf("\n");
  for(int i=0;i<4;++i) {
    for(int j=0;j<4;++j) {
      int bscore = GetScore(b, mask);
      int wscore = GetScore(w, mask);
      if(bscore) {
        printf("b%c ", GetPieceName((PieceType)bscore));
      } else if (wscore) {
        printf("w%c ", GetPieceName((PieceType)wscore));
      } else {
        printf("-- ");
      }
      mask>>=1;
    }
    printf("\n");
  }
}

#endif

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT. */
  int g, w, b, m, i=0, j=0;

#if 0
  while (i<MAX) {
    PrintPossibleMoves((PieceType)i);
    ++i;
  }
  exit(-1);
#endif
  freopen("input", "r", stdin);
  scanf("%d", &g);

  while (g--) {
    Board black, white;
    int i=0;
    scanf("%d%d%d\n", &w, &b, &m);
    char p, c;
    int r;
    while (i<w) {
      scanf("%c %c %d\n", &p, &c, &r);
      uint16_t b=1<<(4*(r-1)+3-(c-'A'));
      white.u.bmap[GetPieceType(p)] = b;
      i++;
    }
    i=0;
    while(i<b) {
      scanf("%c %c %d\n", &p, &c, &r);
      uint16_t b=1<<(4*(r-1)+3-(c-'A'));
      black.u.bmap[GetPieceType(p)] = b;
      i++;
    }
    fbb = black;
    fwb = white;
    int score = minimax(black, white, m, 1);
    if (WINSCORE==score) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }
  return 0;
}

