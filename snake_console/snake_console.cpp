#include "snake_console.h"

HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO CursorInfo = {1, FALSE}; //Hide cursor
COORD AppleCoord;
COORD SnakeBody[100];
COORD NextCoord;
int snake_length = 2;
//direction flags:
int current_dir = 8;  //up:1, left:2, down:4, right:8
int next_dir = 8;
//TODO:Linked list impementation of Snake

//Init game: canvas, snake, apple;
void GameInit(void) {
	//set canvas
	COORD Cursor{ 0, 0 };
	SetConsoleTextAttribute(StdHandle, BACKGROUND_RED);	//set boarder color
	for (int i = 0; i < 61; ++i) {
		for (int j = 0; j < 25; ++j) {
			if (j == 0 || j == 24) {
				SetCoord(i, j);
				printf(" ");
			}
			if (i == 0 || i == 60) {
				SetCoord(i, j);
				printf("  ");
			}
		}
	}
  //set snake
	SnakeBody[0].X = 20;	//snake head position
	SnakeBody[0].Y = 12;
	SetCoord(SnakeBody[0].X, SnakeBody[0].Y);
	SetConsoleTextAttribute(StdHandle, BACKGROUND_BLUE);	//color of snake
	SetConsoleCursorInfo(StdHandle, &CursorInfo);	//hide cursor
	printf("  ");
	SnakeBody[1].X = 18;
	SnakeBody[1].Y = 12;
	SetCoord(SnakeBody[1].X, SnakeBody[1].Y);
	printf("  ");
	//set apple
	CreatApple(&AppleCoord);
  GameOn();
}

//Game Logic
void GameOn(void) {
  bool snake_dead = FALSE;
  bool game_pause = FALSE;
  do {
    snake_dead = CheckDead();
    ReadKeyBoard(&game_pause);
    if (!game_pause) {
      SnakeMove();
      DisplaySnake();
    }
    else {}
  } while (!snake_dead);
  GameOver();
}

//Check if snake is dead
bool CheckDead(void) {
  if (SnakeBody[0].X <= 0 || SnakeBody[0].X > 60 ||	//check if dead
    SnakeBody[0].Y <= 0 || SnakeBody[0].Y >= 24) {
    //GameOver();
    return TRUE;
  }
  for (int i = 1; i < snake_length; ++i) {
    if (SnakeBody[i].X == SnakeBody[0].X &&
      SnakeBody[i].Y == SnakeBody[0].Y) {
      //GameOver();
      return TRUE;
    }
    else {}
  }
  return FALSE;
}

//Place an apple randomly
//Note that in x direction, 1 unit = 2 cursors
//So x must be a even number
void CreatApple(COORD *Coord) {
	bool apple_ok = FALSE;
	//COORD AppleCoord = { 0, 0 };
	while (Coord->X == 0 ||
				 Coord->Y == 0 ||
				 (!apple_ok)) {
		Coord->X = (rand() % 29 + 1) * 2;	// a random even number
		Coord->Y = rand() % 23 + 1;
		for (int i = 0; i < snake_length; ++i) { //check overlap with snake body
			if (Coord->X == SnakeBody[i].X &&
				  Coord->Y == SnakeBody[i].Y) {
				apple_ok = FALSE;
        break;
			} 
      else {
				apple_ok = TRUE;
			}
		}
	}
	apple_ok = FALSE;	//reset flag
	SetCoord(AppleCoord.X, AppleCoord.Y);
	SetConsoleTextAttribute(StdHandle, BACKGROUND_GREEN);	//color of apple
	printf("  ");
}

//TODO: Snake movements
void SnakeMove(void) {
	NextCoord.X = SnakeBody[0].X; //init
  NextCoord.Y = SnakeBody[0].Y;
  if ((current_dir + next_dir == 5) || 
      (current_dir + next_dir == 10)) {  //cannot reverse direction
    next_dir = current_dir;
  }
  else {
    current_dir = next_dir;
  }
	switch (next_dir) {
	  case 1: {  //dir up
      NextCoord.Y  = SnakeBody[0].Y - 1;
      break;
	  }
    case 2: {  //dir left
      NextCoord.X = SnakeBody[0].X - 2;
      break;
    }
    case 4: {  //dir down
      NextCoord.Y = SnakeBody[0].Y + 1;
      break;
    }
    case 8: {  //dir right
      NextCoord.X = SnakeBody[0].X + 2;
      break;
    }
    default: break;
	}
}

//Display Snake
void DisplaySnake() {
  COORD PrevCoord = SnakeBody[snake_length - 1];
  bool apple_get = FALSE;
  if (NextCoord.X == AppleCoord.X && 
      NextCoord.Y == AppleCoord.Y) {  //check if apple ahead
    snake_length++;
    apple_get = TRUE;
    CreatApple(&AppleCoord);
  }
  for (int i = snake_length; i > 1; --i) {
    SnakeBody[i - 1] = SnakeBody[i - 2];
    SetCoord(SnakeBody[i-1].X, SnakeBody[i-1].Y);
    SetConsoleTextAttribute(StdHandle, BACKGROUND_BLUE);
    printf("  ");
  }
  SnakeBody[0] = NextCoord;
  SetCoord(NextCoord.X, NextCoord.Y);
  SetConsoleTextAttribute(StdHandle, BACKGROUND_BLUE);
  printf("  ");
  SetCoord(PrevCoord.X, PrevCoord.Y);
  SetConsoleTextAttribute(StdHandle, 0x0000);
  printf("  ");
}

//read keyboard input
void ReadKeyBoard(bool *pause_flag) {
  while (!_kbhit()) {
    Sleep(400);
    break;
  }
  if (_kbhit()) {
    char key = _getch();
    if (key >= 'A' && key <= 'Z') {
      key -= ('A' - 'a');
    }
    switch (key) {
    case 'w':{  //up
               next_dir = 1;
               break;
    }
    case 'a':{  //left
               next_dir = 2;
               break;
    }
    case 's':{  //down
               next_dir = 4;
               break;
    }
    case 'd':{  //right
               next_dir = 8;
               break;
    }
    case 'p':{  //pause
               *pause_flag ^= TRUE;
    }
    default: break;
    }
    Sleep(10);
  }
  else {
    next_dir = current_dir;
  }
}

//Set cursor position
//Parameters:
//	coord: which coord to set
//	x, y: destination
void SetCoord(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(StdHandle, coord);
}

//Game over
void GameOver(void) {
	SetCoord(28, 12);
  SetConsoleTextAttribute(StdHandle, FOREGROUND_INTENSITY);
	printf("Game Over");
  while (1);
}

long RandomPosition(void) {
  long seed =0;
  seed = seed * 22695477L + 1; 
  return ((seed >> 16) & 0x7FFF);
}