//モンテカルロ法
#ifndef AI3_H
#define AI3_H

#include <assert.h>
#include <stdlib.h>
#include "move.h"

IntPair montecarlo(const int, IntPair *,const int);
int random_play(const int, IntPair);

#endif
