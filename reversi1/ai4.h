#ifndef AI4_H
#define AI4_H

#include "move.h"	
  
int eval_fanc4(const int);
void undo_board4(int boardtemp[][8]);
struct best max_node4(int, int, const int);

#endif

#ifndef STRUCT_BEST
#define STRUCT_BEST

struct best{
  int eval;
  IntPair move;
};

#endif
