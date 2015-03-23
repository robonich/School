//モンテカルロ法
#include "ai3.h"

#define RANDNUM 10

extern int board[8][8];

IntPair montecarlo(const int side, IntPair legal_moves[], const int nmoves)
{
  assert(nmoves > 0);
  int i, j, k, l;
  int n_win, max_n_win = -1;//勝った回数とその最大
  IntPair best;

  int boardtemp[8][8];
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      boardtemp[i][j] = board[i][j];
    }
  }

  for(i = 0; i < nmoves; i++){
    n_win = 0;
    for(j = 0; j < RANDNUM; j++){
      if(side == random_play(side, legal_moves[i])) n_win++; 
      for(k = 0; k < 8; k++){
	for(l = 0; l < 8; l++){
	  board[k][l] = boardtemp[k][l];
	}
      }
    }
    if(n_win > max_n_win){
      max_n_win = n_win;
      best = legal_moves[i];
    }
    for(k = 0; k < 8; k++){
      for(l = 0; l < 8; l++){
	board[k][l] = boardtemp[k][l];
      }
    }
  }
  return best;
}

int random_play(const int side, IntPair firstmove)//どちらがかったかを判定する。sideが勝てばsideを返し、負ければ-side、引き分けなら0
{
  int i, j, sum = 0;
  int npass = 0;
  int turn = side;
  IntPair legal_moves[60];

  place_disk(turn, firstmove);
  turn *= -1;

  while(1){
    const int nmoves = generate_all_legal_moves(turn, legal_moves);
    if(nmoves == -1 || npass == 2){
      for(i = 0; i < 8; i++){
	for(j = 0; j < 8; j++){
	  sum += side * board[i][j];
	}
      }
      break;
    }
    if(nmoves == 0){
      npass++;
      continue;
    }
    else npass = 0;
    place_disk(turn, legal_moves[rand() % nmoves]);
    turn *= -1;
  }
  if(sum > 0) return side;
  if(sum < 0) return -side;
  if(sum == 0) return 0;
  return 0;
}
