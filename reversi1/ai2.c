#include "ai2.h"

#define INFINITY 1000000

extern int board[8][8];

int eval_func2(const int side)
{
  int i, j; 
  int eval = 0;
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if((i == 0 || i == 7) && (j == 0 || j == 7)) eval += 4 * board[i][j];//角は重率4
      else if((i == 1 || i == 6) && (j == 1 || j == 6)) eval += (-3) * board[i][j];//この４ヶ所は重率-3
      else eval += board[i][j];
    }
  }
  return side * eval;//良ければ正、悪ければ負で返す
}

void undo_board2(int board_temp[][8])
{
  int i, j;
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      board[i][j] = board_temp[i][j];
    }
  }
}


struct best max_node2(int depth, int max_depth, const int side)
{//自分の手番で最善の手を考える。だから評価関数は大きければ大きい程よい。1手後は相手の番で、-sideになる。このときも相手が最善の手を打ってくると仮定しているから、この関数でループさせればよい。またdepthが偶数のときは自分の手番で、depthが奇数のときは相手の手番となるので、評価をことなるものとしておいた。ーをつけたり。
  int i, j;
  IntPair legal_moves[60];
  int nmoves = generate_all_legal_moves(side, legal_moves);

  if(nmoves == 0 || nmoves == -1 || depth == max_depth) return (struct best){(depth%2 == 0) ? eval_func2(side) : -eval_func2(side), {0,0}};//ここではIntPairはなんでもよい

  struct best b;
  b.eval = (depth%2 == 0) ? -INFINITY : INFINITY;
  
  int boardtemp[8][8];//次の手に移る前の状態を記録する。
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      boardtemp[i][j] = board[i][j];
    }
  }

  for(i = 0; i < nmoves; i++){
    place_disk(side, legal_moves[i]);
    struct best v = max_node2(depth+1, max_depth, -side); //逆サイド
    if(depth%2 == 0 && v.eval > b.eval){
      b.eval = v.eval;
      b.move = legal_moves[i];
    }
    else if(depth%2 != 0 && -v.eval < b.eval){
      b.eval = -v.eval;
      b.move = legal_moves[i];
    }
    undo_board2(boardtemp);
  }
  return b;//best構造体
}
