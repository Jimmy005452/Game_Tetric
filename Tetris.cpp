#include "Tetris.h"

void Tetris::run()
{
	for (;;)
	{
		this->currentTime = clock();
		// Read input.
		char input = ';';
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				// GO right.
			case 'd':
				if (!this->checkCollision('r'))
				{
					this->clearCurrentGraph();
					this->graphMove('r');
				}
				break;
				// Go left.
			case 'a':
				if (!this->checkCollision('l'))
				{
					this->clearCurrentGraph();
					this->graphMove('l');
				}
				break;
				// Go down.
			case 's':
				if (!this->checkCollision('d'))
				{
					this->clearCurrentGraph();
					this->graphMove('d');
				}
				break;
				// Rotate.
			case ' ':
				this->graphRotate();
				break;
			default:
				break;
			}
			this->uploadGraph();
		}

		// Show map.
		if (this->currentTime >= this->showMapTime or this->currentTime < this->showMapTime - 10000)
		{
			this->showMap();
			this->showMapTime = this->currentTime + SHOWPERIOD;
		}

		// Graph down.
		if ((this->currentTime >= this->graphDownTime or this->currentTime < this->graphDownTime - 10000) and !this->checkCollision('d'))
		{
			this->clearCurrentGraph();
			this->graphMove('d');
			this->uploadGraph();
			this->graphDownTime = this->currentTime + DOWNPERIOD;
		}

		// Check if GameOver.
		if (this->GameOver)
		{
			break;
		}
	}
}

Tetris::Tetris()
{
	// Initialize Map.
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WEIGHT; j++)
		{
			map[i][j] = '.';
		}
	}
	for (int j = 0; j <= WEIGHT; j += (WEIGHT - 1))
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			map[i][j] = '|';
		}
	}
	
	// Initialize the parameters.
	score = 0;
	GameOver = 0;
	this->initialParameter();
	initconsoleScreenBuffer();
}

void Tetris::initialParameter()
{
	srand(time(NULL));
	currentGraph = rand() % 7;
	currentX = 7;
	currentY = -4;
}

void Tetris::showMap()
{
	show(map, score);
}

bool Tetris::checkCollision(char direction)
{
	int nextY = currentY;
	int nextX = currentX;
	switch (direction)
	{
	// Go down.
	case 'd':
		nextY++;
		for (int j = 0; j < 8; j += 2)
		{
			for (int i = 3; i >= 0; i--)
			{
				if (graph[currentGraph][i][j] == '[')
				{
					if (graph[currentGraph][i][j] == map[nextY + i][nextX + j] or (nextY + i) == HEIGHT)
					{
						checkAndRemove();
						checkGameOver(i);
						initialParameter();
						return true;
					}
					break;
				}
			}
		}
		break;

	// Go right.
	case 'r':
		nextX += 2;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 7; j >= 0; j -= 2)
			{
				if (graph[currentGraph][i][j] == ']')
				{
					if (graph[currentGraph][i][j] == map[nextY + i][nextX + j] or (nextX + j) == WEIGHT)
						return true;
					break;
				}
			}
		}
		break;

	// Go left.
	case 'l':
		nextX -= 2;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				if (graph[currentGraph][i][j] == '[')
				{
					if (graph[currentGraph][i][j] == map[nextY + i][nextX + j] or (nextX + j) == -1)
						return true;
					break;
				}
			}
		}
		break;
	}
	return false;
}

void Tetris::graphMove(char direction)
{
	switch (direction)
	{
	case 'd':
		currentY++;
		break;
	case 'r':
		currentX += 2;
		break;
	case 'l':
		currentX -= 2;
		break;
	}
}

void Tetris::graphRotate()
{
	// Simplify the graph.
	char graphBeforeR[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (graph[currentGraph][i][j * 2] == '[')
				graphBeforeR[i][j] = '*';
			else
				graphBeforeR[i][j] = ' ';
		}
	}

	// Get the rotate matrix.
	char graphAfterR[4][4];
	for (int j = 3; j >= 0; j--)
	{
		for (int i = 0; i < 4; i++)
		{
			graphAfterR[i][j] = graphBeforeR[4 - j - 1][i];
		}
	}

	// Check if Rotate.
	bool flag = 1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (graphAfterR[i][j] == '*')
			{
				if ((map[currentY + i][currentX + j * 2] == '[' and graphBeforeR[i][j] != graphAfterR[i][j]) or (currentY + i) >= HEIGHT or (currentX + j * 2 + 1) >= WEIGHT or (currentX + j * 2) <= -1)
				{
					flag = 0;
					break;
				}
			}
		}
		if (flag == 0)
		{
			break;
		}
	}

	// If flag equal to 1, rotate graph.
	if (flag == 1)
	{
		clearCurrentGraph();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				graph[currentGraph][i][j] = ' ';
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (graphAfterR[i][j] == '*')
				{
					graph[currentGraph][i][j * 2] = '[';
					graph[currentGraph][i][j * 2 + 1] = ']';
				}
			}
		}
		uploadGraph();
	}
}

void Tetris::checkAndRemove()
{
	int times = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 1; j < WEIGHT - 1; j++)
		{
			if (map[i][j] == '.')
				break;
			if (j == WEIGHT - 2)
			{
				times += (times / 2 + 1);
				// The cube above this line goes one cube down.
				if (i - 1 > 0)
				{
					for (int y = i - 1; y >= 0; y--)
					{
						for (int x = 1; x < WEIGHT - 1; x++)
						{
							map[y + 1][x] = map[y][x];
						}
					}
					for (int x = 1; x < WEIGHT - 1; x++)
						map[0][x] = '.';
				}
			}
		}
	}
	score += times * 10;
}

void Tetris::uploadGraph()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (graph[currentGraph][i][j] != ' ' && (i + currentY) >= 0)
				map[i + currentY][j + currentX] = graph[currentGraph][i][j];
		}
	}
}

void Tetris::clearCurrentGraph()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (graph[currentGraph][i][j] != ' ' && (i + currentY) >= 0)
				map[i + currentY][j + currentX] = '.';
		}
	}
}

void Tetris::checkGameOver(int y)
{
	for (int j = 1; j < WEIGHT - 1; j++)
	{
		if (currentY + y < 0)
		{
			// Show the score.
			cout << "score:" << score;
			GameOver = 1;
			break;
		}
	}
}