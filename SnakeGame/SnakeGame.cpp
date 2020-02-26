// Project Name : SnakeGame
// Madeby : DragonSky(https://github.com/DragonSky2357)
// Date Of Preparation : 2020-02-26
// Reference : https://m.blog.naver.com/azure0777/220282982693

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<cstdlib>
#include<ctime>

#pragma warning(disable:4996)

// KEY_VALUE
enum KEY_VALUE {
	LEFT=75, RIGHT=77,UP=72,DOWN=80, PAUSE=112,ESC=27,ENTER=13
};

enum MAP_VALUE {
	MAP_X=0, MAP_Y=0, MAP_WIDTH=50, MAP_HEIGHT=50
};

enum COLLISION_VALUE {
	COLLISION_WALL = -1, COLLISION_SNAKE=0, COLLISION_FOOD=1
};
const int KEY_RESET = 0;
const int BASIC_SCORE = 10;

using namespace std;

class PlayerL{ //Player Location
public:
	int x[100];
	int y[100];
};

int key; //Input Key
int gameSpeed; // Game Spped
int direction;
int foodX, foodY;
int length;
int score = 0;
int foodColor;
int playerColor=10;
PlayerL player;

void gotoxy(int x, int y, const char* s) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);

}
void GameInit(void);
void GameStart(void);
void DrawMap(void);
void CheckKey(int key); // Direction Set or Pause etc... 
void MoveSnake();
void MakeTail(void);
int CheckCollision(void);
void DrawSnake(void);
void MoveTorso(void);
void MoveHead(void);
bool CheckDirection(int key);
void MakeFood(void);
void GameOver(void);
void GameScore(void);

int main(void) {
	GameStart();

	// Game Start
	while (true) {
		if (kbhit()) do { key = getch(); } while (key == 224); // Direction key gets 2 values 224 and x
		Sleep(gameSpeed);
		CheckKey(key);
	}
}

void GameStart(void) {
	system("title SnakeGame");
	system("mode con cols=50 lines=20");

	while (kbhit()) getch(); // Drop in buffer key value

	DrawMap();    //Draw Map
	for (int i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // inner map empty space full
		for (int j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) gotoxy(j, i, "  ");
	}

	while (true) {
		if (kbhit()) { 
			key = getch();
			if (key == ESC) exit(0); 
			else break; 
		}
		gotoxy((MAP_WIDTH / 2) - 20, MAP_Y + 9, " < PRESS ANY KEY TO START > ");
		Sleep(500);
		gotoxy((MAP_WIDTH / 2) - 20, MAP_Y + 9, "                            ");
		Sleep(500);

	}
	GameInit(); // Game Initialize
}

void GameInit(void){
	system("cls");
	system("mode con cols=100 lines=50");
	DrawMap();

	while (kbhit()) getch(); 

	direction = LEFT; // Direction Init
	gameSpeed = 100; // Speed Init 
	length = 5; // Snake Length Init 
	score = 0; // Score Init 
	for (int i = 0; i <length; i++) { // Input Snake Torso 
		player.x[i] = MAP_VALUE::MAP_WIDTH / 2 + i;
		player.y[i] = MAP_VALUE::MAP_HEIGHT / 2;
		gotoxy(MAP_X + player.x[i], MAP_Y + player.y[i], "ㅇ");
	}
	gotoxy(MAP_VALUE::MAP_X + player.x[0], MAP_VALUE::MAP_Y + player.y[0], "ㅎ"); //Snake head draw
	MakeFood(); // Make food 
}



void DrawMap(void){
	for (int i = 0; i < MAP_WIDTH; i++) {
		gotoxy(MAP_X + i, MAP_Y, "■");
	}
	for (int i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
		gotoxy(MAP_X, i, "■");
		gotoxy(MAP_X + MAP_WIDTH - 1, i, "■");
	}
	for (int i = 0; i < MAP_WIDTH; i++) {
		gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■");
	}
}

void CheckKey(int key){

	switch (key)
	{
	case KEY_VALUE::LEFT:
	case KEY_VALUE::RIGHT:
	case KEY_VALUE::UP:
	case KEY_VALUE::DOWN:
		if (CheckDirection(key)) direction = key;
		key = KEY_RESET;
		break;
	}
	MoveSnake();
}

void MoveSnake(){
	switch (CheckCollision())
	{
	case COLLISION_VALUE::COLLISION_FOOD: // if food with collision
		playerColor = foodColor;
		MakeFood();
		MakeTail();
		break;
	case COLLISION_VALUE::COLLISION_SNAKE:
	case COLLISION_VALUE::COLLISION_WALL:
		GameOver();
		return;
	}
	DrawSnake();
}
int CheckCollision(void){
	if (player.x[0] == foodX && player.y[0] == foodY) return COLLISION_VALUE::COLLISION_FOOD;
	if (player.x[0] == MAP_VALUE::MAP_X || player.x[0] == MAP_VALUE::MAP_WIDTH - 2 ||
		player.y[0] == MAP_VALUE::MAP_Y || player.y[0] == MAP_VALUE::MAP_WIDTH - 2) return COLLISION_VALUE::COLLISION_WALL;

	for (int i = 1; i < length; i++)
		if (player.x[0] == player.x[i] && player.y[0] == player.y[i]) return COLLISION_VALUE::COLLISION_SNAKE;
}
void DrawSnake(void){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), playerColor);
	MoveTorso();
	MoveHead();
}
void MoveTorso(void){
	if(player.x[length - 1] !=0 && player.y[length - 1]!=0)
		gotoxy(MAP_VALUE::MAP_X + player.x[length - 1], MAP_VALUE::MAP_Y + player.y[length - 1], "  ");

	for (int i = length - 1; i >0; i--) { // a movement of the torso coordinates
		player.x[i] = player.x[i - 1];
		player.y[i] = player.y[i - 1];
	}
	gotoxy(MAP_X + player.x[0], MAP_Y + player.y[0], "○"); // head location reset
}
void MoveHead(void){
	switch (direction)
	{
	case KEY_VALUE::LEFT: --player.x[0]; break;
	case KEY_VALUE::RIGHT: ++player.x[0]; break;
	case KEY_VALUE::UP: --player.y[0]; break;
	case KEY_VALUE::DOWN:++player.y[0]; break;
	}
	gotoxy(MAP_X + player.x[0], MAP_Y + player.y[0], "＠"); // draw head in new head location
}
void MakeFood(void) {
	srand((unsigned)time(NULL));
	foodX = (rand() % MAP_VALUE::MAP_WIDTH - 2) + 3;
	foodY = (rand() % MAP_VALUE::MAP_HEIGHT - 2) + 3;

	for (int i = 0; i < length; i++)
		if (foodX == player.x[i] && foodY == player.y[i]) i = 0;
	
	foodColor = (rand() % 10) + 5;
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foodColor);
	gotoxy(MAP_VALUE::MAP_X + foodX, MAP_VALUE::MAP_Y + foodY, "♬");
	gameSpeed -= 3;
}
void MakeTail(void) {
	gotoxy(MAP_VALUE::MAP_X + player.x[length - 1], MAP_VALUE::MAP_Y + player.y[length - 1], "  ");
	player.x[length - 1] = player.x[length - 2];
	player.y[length - 1] = player.y[length - 2];
	length++;
	score += BASIC_SCORE; // get game score
}

bool CheckDirection(int key) {
	return ((direction == KEY_VALUE::UP && key != KEY_VALUE::DOWN) || (direction == KEY_VALUE::DOWN && key != KEY_VALUE::UP) ||
		(direction == KEY_VALUE::LEFT && key != KEY_VALUE::RIGHT) || (direction == KEY_VALUE::RIGHT && key != KEY_VALUE::LEFT));
}

void GameOver(void){
	system("mode con cols=50 lines=20");
	system("cls");
	while (true) {
		if (kbhit()) {
			key = getch();
			if (key == ESC || key == ENTER) GameScore();
			else break; 
		}
		gotoxy((MAP_VALUE::MAP_WIDTH / 2) - 16, 9, " GAME OVER ");
		Sleep(500);
		gotoxy((MAP_VALUE::MAP_WIDTH / 2) - 16, 9, "           ");
		Sleep(500);
	}
}

void GameScore(void){
	system("cls");

	gotoxy((MAP_VALUE::MAP_WIDTH / 2) - 16, 9, " SCORE ");
	printf(" %d ", score);

	while (true) {
		if (kbhit()) {
			key = getch();
			if (key == ESC || key == ENTER) exit(0);
			else break; 
		}
	}
}

