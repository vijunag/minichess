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

uint16_t RookMask[16][16] = {
{0xffff,0x8fff,0xcfff,0xefff,0xf777,0xffff,0xffff,0xffff,0xff77,0xffff,0xffff,0xffff,0xfff7,0xffff,0xffff,0xffff},
{0x7fff,0xffff,0xcfff,0xefff,0xffff,0xfbbb,0xffff,0xffff,0xffff,0xffbb,0xffff,0xffff,0xffff,0xfffb,0xffff,0xffff},
{0x7fff,0x3fff,0xffff,0xefff,0xffff,0xffff,0xfddd,0xffff,0xffff,0xffff,0xffdd,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0x7fff,0x3fff,0x1fff,0xffff,0xffff,0xffff,0xffff,0xfeee,0xffff,0xffff,0xffff,0xffee,0xffff,0xffff,0xffff,0xfffe},
{0x7fff,0xffff,0xffff,0xffff,0xffff,0xf8ff,0xfcff,0xfeff,0xff77,0xffff,0xffff,0xffff,0xfff7,0xffff,0xffff,0xffff},
{0xffff,0xbfff,0xffff,0xffff,0xf7ff,0xffff,0xfcff,0xfeff,0xffff,0xffbb,0xffff,0xffff,0xffff,0xfffb,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xf7ff,0xf3ff,0xffff,0xfeff,0xffff,0xffff,0xffdd,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0xffff,0xffff,0xffff,0xefff,0xf7ff,0xf3ff,0xf1ff,0xffff,0xffff,0xffff,0xffff,0xffee,0xffff,0xffff,0xffff,0xfffe},
{0x7fff,0xffff,0xffff,0xffff,0x77ff,0xffff,0xffff,0xffff,0xffff,0xff8f,0xffcf,0xffef,0xfff7,0xffff,0xffff,0xffff},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xbbff,0xffff,0xffff,0xff7f,0xffff,0xffcf,0xffef,0xffff,0xfffb,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xddff,0xffff,0xff7f,0xff3f,0xffff,0xffef,0xffff,0xffff,0xfffd,0xffff},
{0xffff,0xffff,0xffff,0xefff,0xffff,0xffff,0xffff,0xeeff,0xff7f,0xff3f,0xff1f,0xffff,0xffff,0xffff,0xffff,0xfffe},
{0x7fff,0xffff,0xffff,0xffff,0x77ff,0xffff,0xffff,0xffff,0x777f,0xffff,0xffff,0xffff,0xffff,0xfff8,0xfffc,0xfffe},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xbbff,0xffff,0xffff,0xffff,0xbbbf,0xffff,0xffff,0xfff7,0xffff,0xfffc,0xfffe},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xddff,0xffff,0xffff,0xffff,0xdddf,0xffff,0xfff7,0xfff3,0xffff,0xfffe},
{0xffff,0xffff,0xffff,0xefff,0xffff,0xffff,0xffff,0xeeff,0xffff,0xffff,0xffff,0xeeef,0xfff7,0xfff3,0xfff1,0xffff},
};

uint16_t BishopMask[16][16] = {
{0xffff,0xffff,0xffff,0xffff,0xffff,0xfbde,0xffff,0xffff,0xffff,0xffff,0xffde,0xffff,0xffff,0xffff,0xffff,0xfffe},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfdef,0xffff,0xffff,0xffff,0xffff,0xffef,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xfbff,0xffff,0xfef7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfdbf,0xffff,0xffff,0xffbf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffbd,0xffff,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0x7fff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffde,0xffff,0xffff,0xffff,0xffff,0xfffe},
{0xffff,0xbfff,0xffff,0x6fff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffbf,0xffff,0xffef,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdb,0xffff,0xfff7,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xdbff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffb,0xffff,0xffff},
{0xffff,0xffff,0xffff,0x6fff,0xf7ff,0xffff,0xfdff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0x7fff,0xffff,0xffff,0xffff,0xffff,0x7bff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffb,0xffff,0xfffe},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xffff,0xbdff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0xffff,0xffff,0xffff,0x6fff,0xffff,0xffff,0xfdff,0xffff,0xffff,0xfdbf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7f,0xffff,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xf7ff,0xffff,0xffff,0xffff,0xffff,0xf7bf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0x7fff,0xffff,0xffff,0xffff,0xffff,0x7bff,0xffff,0xffff,0xffff,0xffff,0x7bdf,0xffff,0xffff,0xffff,0xffff,0xffff},
};

uint16_t (*moves[])[4] = { NULL, KnightMoves, BishopMoves, RookMoves, QueenMoves};

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)
#define BSR(_val) ((__builtin_ffs(_val))-1)
#define RBSR(_val) (15-BSR(_val))

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

uint16_t GenSlidingMoves(PieceType p, Board &mb, Board &ob,
                         uint16_t rank, uint16_t qmask=0)
{
  uint16_t (*m)[4]=moves[p];
  uint16_t (*move_mask)[16] = ROOK == p ? RookMask : BishopMask;
  uint16_t r, c;
  uint16_t mv, mv_mask=0xFFFF, mv_w_mask=0xFFFF;
  uint16_t blackboard=ob.getbits();
  uint16_t whiteboard=mb.getbits();
  uint16_t mask=0x8000;

  if (qmask) {
    r=ROWIDX(qmask);
    c=COLIDX(qmask);
  } else {
    r=ROWIDX(mb.bm[p]);
    c=COLIDX(mb.bm[p]);
  }
  mv=m[r][c];

  for(int i=0; i<4; ++i) {
    for(int j=0; j<4; ++j) {
      if (mask&blackboard) {
          mv_mask&=move_mask[rank][i*4+j];
      }
      if (mask&whiteboard) {
          mv_w_mask&=move_mask[rank][i*4+j];
      }
      mask>>=1;
    }
  }
  mv_mask=(mv_mask & mv)|(mv & blackboard);
  mv_w_mask &= mv;
  return mv_mask & mv_w_mask;
}

uint16_t GetMoves(Board &mb, Board &ob, uint16_t &kmoves,
    uint16_t &rmoves, uint16_t &bmoves, uint16_t &qmoves)
{
  uint16_t (*m)[4]=moves[KNIGHT];
  uint16_t r, c, legal_moves, rook_rank, bishop_rank;
  uint16_t rook_mv, bishop_mv, rook_mask, bishop_mask;
  uint16_t rook_w_mask = 0xFFFF, bishop_w_mask = 0xFFFF;
  uint16_t blackboard=ob.getbits();
  uint16_t whiteboard=mb.getbits();
  uint16_t mask=0xFFFF;

  /*Knight moves*/
  if (mb.bm[KNIGHT]) {
    m=moves[KNIGHT];
    r=ROWIDX(mb.bm[KNIGHT]);
    c=COLIDX(mb.bm[KNIGHT]);
    legal_moves=m[r][c];
    kmoves = ~(legal_moves&mb.getbits())&legal_moves;
  }

  /*Rook moves*/
  if (mb.bm[ROOK]) {
    rmoves = GenSlidingMoves(ROOK, mb, ob, RBSR(mb.bm[ROOK])) ^ mb.bm[ROOK];
  }

  /*Bishop moves*/
  if (mb.bm[BISHOP]) {
    bmoves = GenSlidingMoves(BISHOP, mb, ob, RBSR(mb.bm[BISHOP])) ^ mb.bm[BISHOP];
  }

  /*Queen moves*/
  if (mb.bm[QUEEN]) {
    qmoves = (GenSlidingMoves(ROOK, mb, ob, RBSR(mb.bm[QUEEN]), mb.bm[QUEEN]) |
              GenSlidingMoves(BISHOP, mb, ob, RBSR(mb.bm[QUEEN]),mb.bm[QUEEN])) ^ mb.bm[QUEEN];
  }
}

Board fbb, fwb; //final black & white boards
#define FOREARCHMOVES(p,_moves,_wb,_bb,max)                           \
  if (_moves) {                                                       \
    for(uint16_t mask=0x8000;mask;mask>>=1) {                         \
      if (_moves&mask) {                                              \
        if (max) {                                                    \
          Board nb=_wb;                                               \
          nb.set(p, mask);                                            \
          score=MAX(score, minimax(nb,_bb,alpha,beta,depth-1, max));  \
          alpha=MAX(alpha,score);                                     \
          if (beta<=alpha)                                            \
            goto done1;                                               \
        } else {                                                      \
          Board nb=_bb;                                               \
          nb.set(p, mask);                                            \
          score=MIN(score,minimax(_wb, nb,alpha,beta,depth-1, max));  \
          beta=MIN(beta,score);                                       \
          if (beta<=alpha)                                            \
            goto done2;                                               \
        }                                                             \
      }                                                               \
    }                                                                 \
  }                                                                   \

#define MINIMAX(_wb,_bb,max)              \
 FOREARCHMOVES(QUEEN, qmoves,_wb, _bb,max); \
 FOREARCHMOVES(KNIGHT,kmoves,_wb, _bb,max); \
 FOREARCHMOVES(ROOK,  rmoves,_wb, _bb,max); \
 FOREARCHMOVES(BISHOP,bmoves,_wb, _bb,max);

void PrintGameBoard(Board b, Board w, int =-1);
int minimax(Board b, Board w, int alpha, int beta, int depth, int maximize)
{
  int score = GameScore(b, w, maximize);

#if DEBUG
  printf("Ply: %d\n", depth);
  PrintGameBoard(b, w, maximize);
#endif /*DEBUG*/
  if ((WINSCORE==score)||(LOSESCORE==score)||!depth) {
    return score;
  }

  if (maximize) {
    int score=LOSESCORE;
    uint16_t qmoves=0, kmoves=0, bmoves=0, rmoves=0;
    GetMoves(w, b, kmoves, rmoves, bmoves, qmoves);
    MINIMAX(w,b,0);
done1:
    return (score);
  } else {
    int score = WINSCORE;
    uint16_t qmoves=0, kmoves=0, bmoves=0, rmoves=0;
    GetMoves(b,w, kmoves, rmoves, bmoves, qmoves);
    MINIMAX(w,b,1);
done2:
    return (score);
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

void PrintGameBoard(Board b, Board w, int maximize)
{
  uint16_t mask=0x8000;
  uint16_t bw=b.getbits();
  uint16_t wh=w.getbits();

  printf("\n");
  for(int i=0;i<4;++i) {
    for(int j=0;j<4;++j) {
      int bscore = GetScore(b, mask);
      int wscore = GetScore(w, mask);
      if (maximize>-1 && bscore && wscore) {
        printf("%c%c ", maximize == 1 ? 'b' : 'w', GetPieceName((PieceType)wscore));
      } else if(bscore) {
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
    int score = minimax(black, white, LOSESCORE, WINSCORE, m, 1);
    if (WINSCORE==score) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }
  return 0;
}
