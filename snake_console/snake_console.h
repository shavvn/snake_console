
#ifndef _SNAKE_CONSOLE_H_
#define _SNAKE_CONSOLE_H_
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
//About Initializations
void GameInit(void);
void CreatApple(COORD *Coord);
void RemoveApple(void);
//Game Logic
void SnakeMove(void);
void ReadKeyBoard(bool *pause_flag);
void SetCoord(int x, int y);
void GameOn(void);
void GameOver(void);
bool CheckDead(void);
void DisplaySnake(void);
long RandomPosition(void);
#endif 