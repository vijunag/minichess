/* Author: Vijay Nag
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
#include <string.h>
#include <movegen.h>

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

      uint16_t horiz_rank=(blocks>>(4*(3-ROWIDX(i))) & 0xF);
      int k=COLIDX(i),idx=bitidx;
      /*move in east direction*/
      uint8_t b=0x8>>k;
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
      k=COLIDX(i),idx=bitidx;
      b=0x8>>k;
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
        if (k==0 || k==7 || k==11 || k==15)
          break; //no anti-diagnols
        if (k==j) {
          while(k>=0) {
            ClearBit(bmap, 15-k);
            k=BISHOP_ANTIDIAG_UP(k);
            if (k==3 || k==7 || k==11 || k==15)
              break; //no anti-diagnols
          }
        }
        k=BISHOP_ANTIDIAG_UP(k);
      }


      /*anti-diagnol down*/
      k=i;
      while(k<16) {
        if (k==0 || k==4 || k==8 || k==12)
          break; //no anti-diagnols
        if (k==j) {
          while(k<16) {
            ClearBit(bmap, 15-k);
            k=BISHOP_ANTIDIAG_DOWN(k);
            if (k==0 || k==4 || k==8 || k==12)
              break; //no anti-diagnols
          }
        }
        k=BISHOP_ANTIDIAG_DOWN(k);
      }
      bishopMask[i][j] = bmap;
    }
  }
}

void PrintGameBoard(uint16_t b, uint16_t w)
{
  uint16_t mask=0x8000;

  printf("\n");
  for(int i=0;i<4;++i) {
    for(int j=0;j<4;++j) {
      if (mask&b) {
        printf("P1 ");
      } else if (mask&w) {
        printf("P2 ");
      }else {
        printf("-- ");
      }
      mask>>=1;
    }
    printf("\n");
  }
}

void TestRookMoves(uint16_t bb, uint16_t wb, uint16_t rook_pos)
{
  uint16_t blackboard = bb;
  uint16_t whiteboard = wb;
  uint16_t mask = 0x8000;
  uint16_t oc_mask = 0xFFFF;
  uint16_t row_idx = ROWIDX(rook_pos);
  uint16_t col_idx = COLIDX(rook_pos);
  uint16_t row_s_idx = rook_pos;
  uint16_t move_vector = RookMoves[row_idx][col_idx];
  uint16_t bvectors = move_vector;
  uint16_t black_attacks;
  uint16_t white_attacks;

  printf("-----------Gameboard----------------\n");
  PrintGameBoard(whiteboard, blackboard);

  for (int i=0; i<4; ++i) {
    for(int j=0;j<4; ++j) {
       if (mask & blackboard) {
         oc_mask &= RookMask[row_s_idx][i*4+j];
       }
       mask >>=1;
    }
  }
  bvectors &= blackboard;
  oc_mask &= move_vector;
  oc_mask |= bvectors;
  black_attacks = oc_mask;
  printf("-----------Black attack moves-------\n");
  PrintGameBoard(0,black_attacks);

  mask=0x8000;
  oc_mask=0xFFFF;
  for (int i=0; i<4; ++i) {
    for(int j=0;j<4; ++j) {
       if (mask & whiteboard) {
         oc_mask &= RookMask[row_s_idx][i*4+j];
       }
       mask >>=1;
    }
  }
  white_attacks = move_vector & oc_mask;
  printf("-----------White attack moves-------\n");
  PrintGameBoard(white_attacks, 0);

  printf("-----------Final Attack vector-------\n");
  PrintGameBoard(white_attacks&black_attacks, 0);
}

void TestCase1(void)
{
 /*
  *  0 0  R2  0
  *  0 Q1 R1  0
  *  0 0  B2  0
  *  0 0  B1  0
  */
  TestRookMoves(0b0010000000100000, 0b0000011000000010, 6);

 /*
  *  R1 0  R2  0
  *  K1 Q1 0   0
  *  Q2 0  B2  0
  *  0  0  B1  0
  */
  TestRookMoves(0b0010000010100000, 0b1000110000000010, 0);

 /*
  * 0  Q2  0  0
  * 0  Q1  0  0
  * R2 R1  B1 0
  * 0  K2  K1 0
  */
  TestRookMoves(0b0100000010000100, 0b0000010001100010, 9);

 /*
  * 0  Q2  0  0
  * 0  Q1  0  0
  * R1 R2  B1 0
  * 0  K2  K1 0
  */
  TestRookMoves(0b0100000001000100, 0b0000010010100010, 8);

 /*
  * 0  Q2  0  R1
  * 0  Q1  0  0
  * 0  R2  B1 0
  * 0  K2  K1 0
  */
  TestRookMoves(0b0100000001000100, 0b0001010000100010, 3);

 /*
  * 0  Q2  0  0
  * 0  Q1  0  0
  * 0  R2  0  B1
  * 0  K2  K1 R1
  */
  TestRookMoves(0b0100000001000100, 0b0000010000010011, 15);

 /*
  * 0  Q2  0  0
  * 0  Q1  0  0
  * 0  R2  0  R1
  * 0  K2  K1 B2
  */
  TestRookMoves(0b0100000001000101, 0b0000010000010010, 11);

 /*
  * 0  Q2  0  0
  * 0  Q1  0  0
  * R2  0  0  R1
  * 0  K2  K1 B2
  */
  TestRookMoves(0b0100000010000101, 0b0000010000010010, 11);

 /*
  * R1  0   0  0
  * Q1  B1  0  0
  * R2  0   0  0
  * 0   0   0  0
  */
  TestRookMoves(0b000000001000000, 0b1000110000000000, 0);

 /*
  * 0   0   0  0
  * Q2  B1  R1 R2
  * K2  0   0  0
  * 0   0   0  0
  */
  TestRookMoves(0b0000100110000000, 0b0000011000000000, 6);

 /*
  * 0   0   0  0
  * Q2  0  0   R1
  * K2  0   0  0
  * 0   0   0  0
  */
  TestRookMoves(0b0000100010000000, 0b0000000100000000, 7);

 /*
  * 0   R1  0  B2
  * Q2  K2  0  R2
  * K2  0   0  K2
  * 0   0   0  0
  */
  TestRookMoves(0b0001110110010000, 0b0100000000000000, 1);

 /*
  * 0   0   0  B1
  * Q2  K1  0  R1
  * K2  0   0  K2
  * 0   0   0  0
  */
  TestRookMoves(0b0000100010010000, 0b0001010100000000, 7);
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

  TestCase1();
}
