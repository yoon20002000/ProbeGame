#pragma once

// ���� ��ġ�� ���� �� �� �� ��ġ ��
#define WINDOW_START_POSX	300
#define WINDOW_START_POSY	300
// �ʱ� ������ ������
#define WINDOW_SIZEX		640
#define	WINDOW_SIZEY		480

// object��ġ���� �Ǻ� �ϱ� ����
// enum�� �̿��� bit �����Ͽ� ��ġ �Ǻ�
enum Obj
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_BALL,
	OBJ_PLAYER,
	OBJ_UNKNOWN,
	OBJ_GOAL = (1<<7)

};