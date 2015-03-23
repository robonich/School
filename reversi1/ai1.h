#ifndef AI1_H
#define AI1_H

#include "move.h"
  
int eval_fanc(const int);
void undo_board(int boardtemp[][8]);
struct best max_node(int, int, const int);

#endif

#ifndef STRUCT_BEST
#define STRUCT_BEST

struct best{
  int eval;
  IntPair move;
};

#endif
