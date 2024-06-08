#pragma once
#ifndef TETRIS_H_
#define TETRIS_H_

#include "Graph.h"
#include "Buffer.h"

#include <iostream>
#include <random>
#include <ctime>
#include <conio.h>

using namespace std;

const int WEIGHT = 20;
const int HEIGHT = 20;

// Refresh rate.
const int SHOWPERIOD = 50;
// Game speed. The larger the value, the slower the fall.
const int DOWNPERIOD = 300;

class Tetris
{
private:
	long currentTime = 0;
	long showMapTime = 0;
	long graphDownTime = 0;

	char map[HEIGHT][WEIGHT];
	int currentGraph;
	int currentX;
	int currentY;
	int score;
	bool GameOver;

	void initialParameter();
	void showMap();
	bool checkCollision(char);
	void graphMove(char);
	void graphRotate();
	void checkAndRemove();
	void uploadGraph();
	void clearCurrentGraph();
	void checkGameOver(int);
public:
	void run();
	// Constructor.
	Tetris();
};

#endif // !TETRIS_H_