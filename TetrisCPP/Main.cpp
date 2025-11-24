#include<iostream>
#include<Windows.h>
#include<stdio.h>
#include<thread>
#include<vector>

using namespace std;

wstring tetromino[7];

// Tetris field size
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

// Console screen size
int nScreenWidth = 80;
int nScreenHeight = 30;

static int Rotate(int px, int py, int rotation)
{
	switch (rotation % 4)
	{
	case 0: return py * 4 + px; // 0 degrees: i = (0,0) + y * w + x
	case 1: return 12 + py - (px * 4); // 90 degrees: i = (0,0) + y - (x * w)
	case 2: return 15 - (py * 4) - px; // 180 degrees: i = (0,0) - (y * w) - x 
	case 3: return 3 + py + (px * 4); // 270 degrees: i = (0,0) + y + (x * w)
	default: return py * 4 + px; // Default 0 degrees case
	}
}

int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Tetronimos 4x4
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	// Create play field buffer
	pField = new unsigned char[nFieldWidth * nFieldHeight];

	// Board Boundary
	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	bool gameOver = false;

	// Main game loop
	while (!gameOver)
	{
		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}