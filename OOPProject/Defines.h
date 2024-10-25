#pragma once

// 시작 위치점 변경 할 때 쓸 위치 값
#define WINDOW_START_POSX	300
#define WINDOW_START_POSY	300
// 초기 윈도우 사이즈
#define WINDOW_SIZEX		640
#define	WINDOW_SIZEY		480

// object위치들을 판별 하기 위해
// enum을 이용해 bit 연산하여 위치 판별
enum Obj
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_BALL,
	OBJ_PLAYER,
	OBJ_UNKNOWN,
	OBJ_GOAL = (1<<7)

};