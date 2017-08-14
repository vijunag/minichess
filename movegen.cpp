/*
 * Rook move generation
 *
 * The possible line N of attacks for a Rook is as shown below
 * *R**              |
 * *RRR       <--W---C---E--->
 * *R**              S
 *
 * Bishop move generation
 * The possible line  of attacks for bishop is as shown below
 * B*B*
 * *B**
 * B*B*
 * ***B
 */
#include <stdio.h>
#include <stdint.h>

#define ROOK_MOVE_UP(_i)   (_i-4)
#define ROOK_MOVE_DOWN(_i) (_i+4)
#define BISHOP_DIAG_UP(_i) (_i-5)
#define BISHOP_DIAG_DOWN(_i) (_i+5)
#define BISHOP_ANTIDIAG_UP(_i) (_i-3)
#define BISHOP_ANTIDIAG_DOWN(_i) (_i+3)

#define BSR(_val) ((__builtin_ffs(_val))-1)
#define ROWIDX(r) (r>>2)
#define COLIDX(r) (r&0x3)
#define SERIALIZE(j) (1<<(15-j))

uint16_t rookMask[16][16];
uint16_t bishopMask[16][16];

/*Move vector for Rook*/
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

uint16_t ClearBit(uint16_t &bits, int no)
{
    bits = bits^(1 << no);
}

uint16_t GenRookBlockMasks(void)
{
  uint16_t mask = 0xFFFF;

  for(int i=0;i<16;++i) {
    uint16_t move_mask = RookMoves[ROWIDX(i)][COLIDX(i)];
    for(int j=0;j<16;++j) {
      uint16_t bmap = mask;
      rookMask[i][j] = bmap;
      int bitidx = 15-j;
      uint16_t blocks = SERIALIZE(j);
      uint16_t set_bits = move_mask & blocks;

      if (i==j || !set_bits) {
        continue;
      }

      uint16_t horiz_rank=(blocks>>(4*(3-i)) & 0xF);
      int k=i,idx=bitidx;
      /*move in east direction*/
      uint8_t b=0x8>>ROWIDX(i);
      while (k<=3) {
        if (b&horiz_rank) {
          while(k<=3) {
            ClearBit(bmap, idx);
            k++;
            idx--;
          }
        }
        b=b>>1;
        k++;
      }

      /*move in west direction*/
      k=i,idx=bitidx;
      b=0x8>>ROWIDX(i);
      while (k>=0) {
        if (b&horiz_rank) {
          while(k>=0) {
            ClearBit(bmap, idx);
            idx++;
            k--;
          }
        }
        b<<=1;
        k--;
      }

      /*move north*/
      k=ROOK_MOVE_UP(i);
      while(k>=0) {
        if (k==j) {
          while(k>=0) {
            ClearBit(bmap, 15-k);
            k=ROOK_MOVE_UP(k);
          }
        }
        k=ROOK_MOVE_UP(k);
      }

      /*move south*/
      k=ROOK_MOVE_DOWN(i);
      while(k<16) {
        if (k==j) {
          while(k<16) {
            ClearBit(bmap, 15-k);
            k=ROOK_MOVE_DOWN(k);
          }
        }
        k=ROOK_MOVE_DOWN(k);
      }
      rookMask[i][j]=bmap;
    }
  }
}

void GenBishopBlockMasks()
{
  uint16_t mask = 0xFFFF;
  int k;

  for(int i=0;i<16;++i) {
    uint16_t move_mask = BishopMoves[ROWIDX(i)][COLIDX(i)];
    for(int j=0;j<16;++j) {
      uint16_t bmap = mask;
      bishopMask[i][j] = bmap;
      int bitidx = 15-j;
      uint16_t blocks = SERIALIZE(j);
      uint16_t set_bits = move_mask & blocks;

      if (i==j || !set_bits) {
        continue;
      }

#define TRAVERSE(OP, val, _dir, up_or_down)   \
  k=i;                                        \
  while(k OP val) {                           \
   if (k==j) {                                \
     while(k OP val) {                        \
       ClearBit(bmap, 15-k);                  \
       k=BISHOP_##_dir##_##up_or_down(k);     \
     }                                        \
   }                                          \
   k=BISHOP_DIAG_UP(k);                       \
 }

      /*diagnol up*/
      k=i;
      while(k>=0) {
        if (k==j) {
          while(k>=0) {
            ClearBit(bmap, 15-k);
            k=BISHOP_DIAG_UP(k);
          }
        }
        k=BISHOP_DIAG_UP(k);
      }

      /*diagnol down*/
      k=i;
      while(k<16) {
        if (k==j) {
          while(k<16) {
            ClearBit(bmap, 15-k);
            k=BISHOP_DIAG_DOWN(k);
          }
        }
        k=BISHOP_DIAG_DOWN(k);
      }

      /*anti-diagnol up*/
      k=i;
      while(k>=0) {
        if (k==j) {
          while(k>=0) {
            ClearBit(bmap, 15-k);
            k=BISHOP_ANTIDIAG_UP(k);
          }
        }
        k=BISHOP_ANTIDIAG_UP(k);
      }


      /*anti-diagnol down*/
      k=i;
      while(k<16) {
        if (k==3 || k==7 || k==11 || k==15)
          break; //no anti-diagnols
        if (k==j) {
          while(k<16) {
            ClearBit(bmap, 15-k);
            k=BISHOP_ANTIDIAG_DOWN(k);
            if (k==3 || k==7 || k==11 || k==15)
              break; //no anti-diagnols
          }
        }
        k=BISHOP_ANTIDIAG_DOWN(k);
      }
      bishopMask[i][j] = bmap;
    }
  }
}

int main()
{
  GenRookBlockMasks();
  GenBishopBlockMasks();

  printf("----Rook moves----\n");

  printf("uint16_t RookMask[16][16] = {\n");
  for(int i=0;i<16;++i) {
    printf("{");
    for(int j=0;j<16;++j) {
      if (j==15) {
        printf("0x%x},\n", rookMask[i][j]);
      } else {
        printf("0x%x,", rookMask[i][j]);
      }
    }
  }
  printf("}\n");
  printf("uint16_t BishopMask[16][16] = {\n");
  for(int i=0;i<16;++i) {
    printf("{");
    for(int j=0;j<16;++j) {
      if (j==15) {
        printf("0x%x},\n", bishopMask[i][j]);
      } else {
        printf("0x%x,", bishopMask[i][j]);
      }
    }
  }
  printf("}\n");
}
