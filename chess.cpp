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
  {0x4842,0xA4A1,0x5258,0x2124},
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
{0xffff,0xffff,0xffff,0xffff,0xf7ff,0xffff,0xfdef,0xffff,0xffff,0xffff,0xffff,0xffef,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xfb7f,0xffff,0xfeff,0xff7f,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfdb7,0xffff,0xffff,0xffb7,0xffff,0xffff,0xfff7,0xffff,0xffff,0xffff},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffbd,0xffff,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0x7fff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xff7f,0xffff,0xffde,0xffff,0xffff,0xffff,0xffff,0xfffe},
{0xffff,0xbfff,0xffff,0xefff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffb7,0xffff,0xffef,0xfff7,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdb,0xffff,0xffff,0xfffb,0xffff,0xffff},
{0xffff,0xffff,0xdfff,0xffff,0xffff,0xdbff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffb,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xefff,0xf7ff,0xffff,0xedff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfff7,0xffff,0xfffd,0xffff},
{0x7fff,0xffff,0xffff,0xffff,0xffff,0x7bff,0xffff,0xfeff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffb,0xffff,0xfffe},
{0xffff,0xbfff,0xffff,0xffff,0xffff,0xffff,0xbdff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffd,0xffff},
{0xffff,0xffff,0xffff,0xefff,0xffff,0xffff,0xedff,0xffff,0xffff,0xedbf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfeff,0xff7f,0xffff,0xfedf,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xf7ff,0xffff,0xffff,0xffff,0xffff,0xf7bf,0xffff,0xffef,0xffff,0xffff,0xffff,0xffff},
{0x7fff,0xffff,0xffff,0xffff,0xffff,0x7bff,0xffff,0xffff,0xffff,0xffff,0x7bdf,0xffff,0xffff,0xffff,0xffff,0xffff},
};

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)
#define BSR(_val) ((__builtin_ffs(_val))-1)
#define RBSR(_val) (15-BSR(_val))
#define SERIALIZE(_val) (1<<(15-(_val)))

#define ROWIDX(r) (3-BSR(r)/4)
#define COLIDX(c) (3-(BSR(c))%4)

typedef enum ColorType {
  BLACK = 0,
  WHITE = 1,
} ColorType;

typedef enum PieceType {
  INVALID = 0,
  KNIGHT1 = 1,
  KNIGHT2 = 2,
  BISHOP1 = 3,
  BISHOP2 = 4,
  ROOK1 = 5,
  ROOK2 = 6,
  QUEEN = 7,
  MAX_PIECE_TYPE,
#define WINSCORE   100
#define LOSESCORE -WINSCORE
} PieceType;

uint16_t (*moves[])[4] = { NULL, KnightMoves, KnightMoves, BishopMoves, BishopMoves, RookMoves, RookMoves, QueenMoves};

typedef struct Board {
  union {
    struct _bmap {
      uint16_t _invalid;
      uint16_t _kn1;
      uint16_t _kn2;
      uint16_t _bi1;
      uint16_t _bi2;
      uint16_t _ro1;
      uint16_t _ro2;
      uint16_t _qu;
    } _bval;
    uint16_t bmap[MAX_PIECE_TYPE];
  } u;

#define kn1 u.bmap[1]
#define kn2 u.bmap[2]
#define bi1 u.bmap[3]
#define bi2 u.bmap[4]
#define ro1 u.bmap[5]
#define ro2 u.bmap[6]
#define qu u.bmap[7]
#define bm u.bmap

  Board() {
    clearbits();
  }

  uint16_t getbits(void) {
    return kn1 | kn2 | bi1 | bi2 | ro1 | ro2 | qu;
  }

  void set(PieceType p, int mask) {
    bm[p] = mask;
  }

  void clear(uint16_t mask) {
    kn1 = kn1&mask ? 0:kn1;
    kn2 = kn2&mask ? 0:kn2;
    ro1 = ro1&mask ? 0:ro1;
    ro2 = ro2&mask ? 0:ro2;
    bi1 = bi1&mask ? 0:bi1;
    bi2 = bi2&mask ? 0:bi2;
    qu = qu&mask ? 0:qu;
  }

  void clearbits(void) {
    u.bmap[0] = 0;
    kn1 = 0;
    kn2 = 0;
    bi1 = 0;
    bi2 = 0;
    ro1 = 0;
    ro2 = 0;
    qu = 0;
  }
} Board;

int GetBoardScore(Board &b)
{
  int score = 0;
  score += b.qu ? (QUEEN * 1) : 0;
  score += b.kn1 ? (KNIGHT1 * 1) : 0;
  score += b.kn2 ? (KNIGHT1 * 1) : 0;
  score += b.bi1 ? (BISHOP1 * 1) : 0;
  score += b.bi2 ? (BISHOP1 * 1) : 0;
  score += b.ro1 ? (ROOK1 * 1) : 0;
  score += b.ro2 ? (ROOK1 * 1) : 0;
  return score;
}

int GameScore(Board &p1, Board &p2)
{
  if (!p2.qu) {
    return LOSESCORE;
  } else if (!p1.qu) {
    return WINSCORE;
  } else {
    return GetBoardScore(p2) - GetBoardScore(p1);
  }
}

#if 1

PieceType GetPieceType(char c)
{
  switch(c) {
   case 'Q': return QUEEN;
   case 'B': return BISHOP1;
   case 'R': return ROOK1;
   case 'N': return KNIGHT1;
  }
  return MAX_PIECE_TYPE;
}

const char GetPieceName(PieceType p)
{
  switch(p) {
   case QUEEN:  return 'Q';
   case BISHOP1:
   case BISHOP2: return 'B';
   case ROOK1: /*fall through*/
   case ROOK2:   return 'R';
   case KNIGHT1:
   case KNIGHT2: return 'N';
  }
  return 'Z';
}

int GetScore(Board &b, uint16_t mmask)
{
  if (b.qu & mmask)//highest score
    return QUEEN;
  if (b.ro1&mmask)
    return ROOK1;
  if (b.ro2&mmask)//same score
    return ROOK1;
  if (b.kn1 & mmask)
    return KNIGHT1;
  if (b.kn2 & mmask)
    return KNIGHT1; //same score
  if (b.bi1 & mmask)//lowest score
    return BISHOP1;
  if (b.bi2 & mmask)//same score
    return BISHOP1;
  return 0; //some intermediate score
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
#endif

void PrintGameBoard(Board b, Board w, int max=-1)
{
  uint16_t mask=0x8000;
  uint16_t bw=b.getbits();
  uint16_t wh=w.getbits();

  printf("\n");
  for(int i=0;i<4;++i) {
    for(int j=0;j<4;++j) {
      int bscore = GetScore(b, mask);
      int wscore = GetScore(w, mask);
      if ((max>-1)&&bscore&&wscore) {
        int score= max==1 ? bscore : wscore;
        printf("%c%c ", max==1 ? 'b' :'w', GetPieceName((PieceType)score));
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

uint16_t GetSlidingPieceMoves(PieceType p, Board &mb, Board &ob,
                         uint16_t rank, uint16_t qmask=0)
{
  uint16_t (*m)[4]=moves[p];
  uint16_t (*move_mask)[16];
  uint16_t r, c;
  uint16_t mv, mv_mask=0xFFFF, mv_w_mask=0xFFFF;
  uint16_t blackboard=ob.getbits();
  uint16_t whiteboard=mb.getbits();
  uint16_t mask=0x8000;

  if ((ROOK1 == p) || (ROOK2 == p)) {
     move_mask=RookMask;
  } else {
    move_mask=BishopMask;
  }

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
          mv_mask&=move_mask[rank][i*4+j] | SERIALIZE(i*4+j);
      }
      if (mask&whiteboard) {
          mv_w_mask&=move_mask[rank][i*4+j];
      }
      mask>>=1;
    }
  }
  mv_mask&=mv;
  mv_w_mask &= m[r][c];
  return mv_mask & mv_w_mask;
}

typedef struct Moves {
  uint16_t moves[MAX_PIECE_TYPE];
} Moves;

void  GetMoves(Board &mb, Board &ob, Moves &mm){

  uint16_t (*m)[4]=moves[KNIGHT1];
  uint16_t r, c, legal_moves, rook_rank, bishop_rank;
  uint16_t rook_mv, bishop_mv, rook_mask, bishop_mask;
  uint16_t rook_w_mask = 0xFFFF, bishop_w_mask = 0xFFFF;
  uint16_t blackboard=ob.getbits();
  uint16_t whiteboard=mb.getbits();
  uint16_t mask=0xFFFF;

  mm.moves[KNIGHT1] = 0;
  mm.moves[KNIGHT2] = 0;
  mm.moves[ROOK1] = 0;
  mm.moves[ROOK2] = 0;
  mm.moves[BISHOP1] = 0;
  mm.moves[BISHOP2] = 0;
  mm.moves[QUEEN] = 0;

  /*Knight moves*/
  if (mb.bm[KNIGHT1]) {
    m=moves[KNIGHT1];
    r=ROWIDX(mb.bm[KNIGHT1]);
    c=COLIDX(mb.bm[KNIGHT1]);
    legal_moves=m[r][c];
    mm.moves[KNIGHT1] = ~(legal_moves&mb.getbits())&legal_moves;
  }

  /*Knight moves*/
  if (mb.bm[KNIGHT2]) {
    m=moves[KNIGHT2];
    r=ROWIDX(mb.bm[KNIGHT2]);
    c=COLIDX(mb.bm[KNIGHT2]);
    legal_moves=m[r][c];
    mm.moves[KNIGHT2] = ~(legal_moves&mb.getbits())&legal_moves;
  }

  /*Rook moves*/
  if (mb.bm[ROOK1]) {
    mm.moves[ROOK1]=GetSlidingPieceMoves(ROOK1, mb, ob, RBSR(mb.bm[ROOK1])) ^ mb.bm[ROOK1];
  }

  /*Rook moves*/
  if (mb.bm[ROOK2]) {
    mm.moves[ROOK2]=GetSlidingPieceMoves(ROOK2, mb, ob, RBSR(mb.bm[ROOK2])) ^ mb.bm[ROOK2];
  }

  /*Bishop moves*/
  if (mb.bm[BISHOP1]) {
    mm.moves[BISHOP1]=GetSlidingPieceMoves(BISHOP1, mb, ob, RBSR(mb.bm[BISHOP1])) ^ mb.bm[BISHOP1];
  }

  /*Bishop moves*/
  if (mb.bm[BISHOP2]) {
    mm.moves[BISHOP2]=GetSlidingPieceMoves(BISHOP2, mb, ob, RBSR(mb.bm[BISHOP2])) ^ mb.bm[BISHOP2];
  }

  /*Queen moves*/
  if (mb.bm[QUEEN]) {
    mm.moves[QUEEN]=(GetSlidingPieceMoves(ROOK1, mb, ob, RBSR(mb.bm[QUEEN]), mb.bm[QUEEN]) |
              GetSlidingPieceMoves(BISHOP1, mb, ob, RBSR(mb.bm[QUEEN]),mb.bm[QUEEN])) ^ mb.bm[QUEEN];
  }
}

#ifdef DEBUG
#define STOREBESTMOVE(_b, _nb, op) \
({                                 \
 if (score op v) {                 \
    _b = _nb;                      \
 }                                 \
})
#else
#define STOREBESTMOVE(_b,_nb, op)
#endif /*DEBUG*/
Board fbb, fwb; //final black & white boards

#define MINIMAX(max,func)                                              \
    int score, done=0;                                                 \
    int _max = max ? 0 : 1;                                            \
    Moves moves;                                                       \
    if (max) {                                                         \
      score = LOSESCORE;                                               \
      GetMoves(w,b,moves);                                             \
    } else {                                                           \
      score = WINSCORE;                                                \
      GetMoves(b,w, moves);                                            \
    }                                                                  \
    for(PieceType p=QUEEN; p>INVALID; p=(PieceType)(((int)p)-1)) {     \
      uint16_t mv=moves.moves[p];                                      \
      if (mv&&!done) {                                                 \
        for(uint16_t mask=0x8000;mask;mask>>=1) {                      \
          if (mask&mv) {                                               \
            int val;                                                   \
            Board nwb=w;                                               \
            Board nbb=b;                                               \
            if (max) {                                                 \
              nwb.set(p, mask);                                        \
              nbb.clear(nwb.getbits()); /*white captures*/             \
            } else {                                                   \
              nbb.set(p, mask);                                        \
              nwb.clear(nbb.getbits()); /*black captures*/             \
            }                                                          \
            val=minimax(nbb,nwb,alpha,beta,depth-1,_max);              \
            score=func(score,val);                                     \
            if (max) {                                                 \
              alpha=func(alpha,score);                                 \
            } else {                                                   \
              beta=func(beta,score);                                   \
            }                                                          \
            if (beta<=alpha) {                                         \
              done=1;                                                  \
              break;                                                   \
            }                                                          \
          }                                                            \
        }                                                              \
      }                                                                \
    }                                                                  \
    return (score);                                                    \

#ifdef DEBUG
int g_debug = 0;
#endif /*DEBUG*/
int minimax(Board b, Board w, int alpha, int beta, int depth, int maximize)
{
  int score = GameScore(b, w);

#if DEBUG
  PrintGameBoard(b, w, maximize);
  printf("Count=%d:Ply:%d, Score:%d, Alpha=%d, Beta=%d\n", g_debug++, depth, score, alpha, beta);
#endif /*DEBUG*/
  if ((WINSCORE==score)||(LOSESCORE==score)||!depth) {
#if DEBUG
    if (WINSCORE==score || LOSESCORE==score) {
      uint16_t posn = maximize ? BSR(b.qu) : BSR(w.qu);
      printf("Max=%d, Queen captured by %c at posn=%d\n", maximize, maximize ? 'B' : 'W', posn);
    }
#endif /*DEBUG*/
    return score;
  }

  if (maximize) {
    MINIMAX(maximize,MAX);
  } else {
    MINIMAX(maximize,MIN);
  }
}

int main(int argc, char **argv) {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT. */
  int g, w, b, m, i=0, j=0;

#ifdef CMDLINE
  freopen(argv[1], "r", stdin);
#endif /*CMDLINE*/
  scanf("%d", &g);

  while (g--) {
    Board black, white;
    int i=0;
    scanf("%d%d%d\n", &w, &b, &m);
    char p, c;
    int r;
    while (i<w) {
      scanf("%c %c %d\n", &p, &c, &r);
      uint16_t bit=1<<(4*(r-1)+3-(c-'A'));
      PieceType piece = GetPieceType(p);
      if (white.u.bmap[piece]) {
        piece= (PieceType)((int)piece + 1);
      }
      white.u.bmap[piece] = bit;
      i++;
    }
    i=0;
    while(i<b) {
      scanf("%c %c %d\n", &p, &c, &r);
      uint16_t bit=1<<(4*(r-1)+3-(c-'A'));
      PieceType piece = GetPieceType(p);
      if (black.u.bmap[piece]) {
        piece= (PieceType)((int)piece + 1);
      }
      black.u.bmap[piece] = bit;
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
#ifdef SELFGAME
    int k=10;
    score=0;
    printf("Game start\n");
    PrintGameBoard(black, white, 1); //First is maximizing player
    Board pb = black;
    while (k || (LOSESCORE==score) || (WINSCORE==score)) {
        printf("Player 1 white's move\n");
        PrintGameBoard(pb, fwb, 1);
        printf("Player 2 black's move\n");
        PrintGameBoard(fbb, fwb, 0);
        pb = fbb;
        score = minimax(fbb, fwb, LOSESCORE, WINSCORE, m, 1);
      k--;
    }
#endif /*DEBUG*/
  }
  return 0;
}
