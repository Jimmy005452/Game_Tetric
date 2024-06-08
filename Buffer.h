#pragma once
#ifndef BUFFER_H_
#define BUFFER_H_

#include <Windows.h>

extern HANDLE hOutput, hOutBuf;
extern COORD coord;
extern DWORD bytes;
extern bool flag;

void initconsoleScreenBuffer();
void show(char map[20][20], int score);

#endif // !BUFFER_H_