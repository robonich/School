#ifndef AI2_H
#define AI2_H

#include "move.h"	
  
int eval_fanc2(const int);
void undo_board2(int boardtemp[][8]);
struct best max_node2(int, int, const int);

#endif

#ifndef STRUCT_BEST
#define STRUCT_BEST

struct best{
  int eval;
  IntPair move;
};

#endif
