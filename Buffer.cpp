#include "Buffer.h"

HANDLE hOutput = NULL, hOutBuf = NULL;
COORD coord = { 0, 0 };
DWORD bytes = 0;
bool flag = 0;

void initconsoleScreenBuffer()
{
	hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	hOutput = CreateConsoleScreenBuffer(
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);
	SetConsoleCursorInfo(hOutBuf, &cci);
}

void show(char map[20][20], int score)
{
	// String processing.
	char scoreStr[13] = { "score : 0000" };
	for (int i = 0; i < 4; i++)
	{
		int temp = score;
		for (int j = 0; j < (3 - i); j++)
		{
			temp /= 10;
		}
		temp %= 10;
		scoreStr[8 + i] = '0' + temp;
	}
	// Show the game screen.
	if (flag == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			coord.Y = i;
			WriteConsoleOutputCharacterA(hOutBuf, map[i], 20, coord, &bytes);
		}
		// Show the score.
		coord.Y++;
		WriteConsoleOutputCharacterA(hOutBuf, scoreStr, 12, coord, &bytes);
		SetConsoleActiveScreenBuffer(hOutBuf);
	}
	else if (flag == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			coord.Y = i;
			WriteConsoleOutputCharacterA(hOutput, map[i], 20, coord, &bytes);
		}
		// Show the score.
		coord.Y++;
		WriteConsoleOutputCharacterA(hOutput, scoreStr, 12, coord, &bytes);
		SetConsoleActiveScreenBuffer(hOutput);
	}

	// Exchange the screen.
	flag = flag ? 0 : 1;
}