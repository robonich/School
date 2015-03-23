#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "move.h"
#include "ai1.h"
#include "ai2.h"
#include "ai3.h"
#include "ai4.h"

#define MAX_DEPTH 4//自分の現状から一手先が深度１

int board[8][8];  // 8x8 squares: 0 -> empty; 1 -> black; -1 -> white

static void init_board()
{
  int x, y;
  for (y = 0; y < 8; y++) 
    for (x = 0; x < 8; x++)
      board[x][y] = 0;

  board[4][3] = board[3][4] =  1;  // black disks
  board[3][3] = board[4][4] = -1;  // white disks
}

static void print_board()
{
  int x, y;
  puts("\n  a b c d e f g h");
  for (y = 0; y < 8; y++) {
    printf("%d ", y + 1);
    for (x = 0; x < 8; x++) {
      const int d = board[x][y];
      char c = '-';
      if (d ==  1) c = 'B';
      if (d == -1) c = 'W';
      printf("%c ", c);
    }
    putchar('\n');
  }
  putchar('\n');
}

void print_result(int mode){;
  int i, j, black, white;
  black = 0, white = 0;
  static int blackwin = 0, whitewin = 0, draw = 0;

  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if(board[i][j] == 1) black++;
      if(board[i][j] == -1) white++;
    }
  }

  printf("black : %d, white : %d\n", black, white);
  if(black == white) draw++;
  if(black > white) blackwin++;
  if(black < white) whitewin++;
  if(blackwin + whitewin + draw == mode){//最終結果を表示
    printf("-------R E S U L T------- \n");
    printf("BLACK : WHITE : DRAW = %d : %d : %d\n", blackwin, whitewin, draw);
    if(blackwin == whitewin) printf("DRAW\n");
    if(blackwin > whitewin) printf("BLACK WIN\n");
    if(blackwin < whitewin) printf("WHITE WIN\n");
  }
}

int main(int argc, char **argv)//黒が先手
{
  char black, white;//black先手(blackならコマは1)、white後手(whiteならコマは-1)で選ばせる
  printf("BLACK, WHITE:");
  scanf("%c,%c", &black, &white);
  
  int mode = argc >= 2 ? atoi(argv[1]) : 1;//実行モードの選択。"1"以外ならその回数分回対戦したあとに、勝率を表示（ディスク表示なし）。"1"なら1回対戦（ディスク表示あり）。何も入力がないなら"1"。
  int turn;
  int npass = 0;//連続pass回数を記録する

  int k;

  srand((unsigned) time(NULL));

  for(k = 0; k < mode; k++){

    init_board();

    //プレイヤー選択。'p'はplayer, 数字はコンピュータ。数字が大きくなれば強くなる。最小'0'最大''
    

    for (turn = 1;; turn *= -1) {
      if(mode == 1) print_board();//mode1のときだけディスク表示

      IntPair legal_moves[60];
      const int nmoves = generate_all_legal_moves(turn, legal_moves);
      if (nmoves == -1) break;     // no empty square
      if (nmoves ==  0) { // pass
	if(mode == 1) printf("turn = %d, move = Pass\n", turn);
	npass++;
	if(npass == 2){
	  if(mode == 1) printf("Both can't place disk!!!\n");//勝率以外の表示はさせない
	  break;
	}
	continue;
      }
      npass = 0;//pass回数の初期化。ここまで読み込まれたということはpassしていないということだから。
      IntPair move;
      if (turn == 1) {//これは先攻の手番
	while(1){
	  switch(black){
	  case 'p':
	    while (1) {
	      printf("Where? ");
	      char buf[1000];
	      scanf("%s", buf);
	      move.first  = buf[0] - 'a';
	      move.second = buf[1] - '1';
	      if (is_legal_move(turn, move)) break;
	      printf("%s is not a legal move!\n", buf);
	    } break;
	  case '0':
	    move = legal_moves[rand() % nmoves];  // choose a legal move at random.
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '1':
	    move = max_node(0, MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '2':
	    move = max_node2(0, MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '3':
	    move = montecarlo(turn, legal_moves, nmoves);
	    if(mode == 1) printf("turn = %d, move = %c%c\n",
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '4':
	    move = max_node4(0, MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  default: 
	    printf("there is no such player. Please input again:");
	    scanf("%c", &black);
	    continue;//defaultにきたときはwhileに戻ってもらう。
	  }
	  break;//無事switchを抜けられたらwhileも抜ける。
	}
      } else {//これは後攻の手番
	while(1){
	  switch(white){
	  case 'p':
	    while (1) {
	      printf("Where? ");
	      char buf[1000];
	      scanf("%s", buf);
	      move.first  = buf[0] - 'a';
	      move.second = buf[1] - '1';
	      if (is_legal_move(turn, move)) break;
	      printf("%s is not a legal move!\n\n", buf);
	    } break;
	  case '0': 
	    move = legal_moves[rand() % nmoves];  // choose a legal move at random.
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '1':
	    move = max_node(0, MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '2':
	    move = max_node2(0,MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '3':
	    move = montecarlo(turn, legal_moves, nmoves);
	    if(mode == 1) printf("turn = %d, move = %c%c\n",
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  case '4':
	    move = max_node4(0, MAX_DEPTH, turn).move;
	    if(mode == 1) printf("turn = %d, move = %c%c\n", 
				 turn, 'a' + move.first, '1' + move.second);
	    break;
	  default: 
	    printf("there is no such player. Please input again:");
	    scanf("%c", &white);
	    continue;//defaultにきたときはwhileに戻ってもらう。
	  }
	  break;//無事switchを抜けられたらwhileも抜ける。
	}
      }
      place_disk(turn, move);
    }
  
    print_result(mode);
  }
  return 0;
}
