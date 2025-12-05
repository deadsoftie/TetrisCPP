#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <string>
#include "Buffer.h"
#include "FieldBuffer.h"
#include "ScreenBuffer.h"
#include "TetrominoManager.h"

using namespace std;

// Tetris field size
static int nFieldWidth = 12;
static int nFieldHeight = 18;

// Console screen size
static int nScreenWidth = 80;
static int nScreenHeight = 30;

static int Rotate(int px, int py, int rotation)
{
	switch (rotation % 4)
	{
	case 0: return py * 4 + px; // 0 degrees: i = (0,0) + y * w + x
	case 1: return 12 + py - (px * 4); // 90 degrees: i = (0,0) + y - (x * w)
	case 2: return 15 - (py * 4) - px; // 180 degrees: i = (0,0) - (y * w) - x
	case 3: return 3 - py + (px * 4); // 270 degrees: i = (0,0) + y + (x * w)
	default: return py * 4 + px; // Default 0 degrees case
	}
}

bool DoesPieceFit(TetrominoManager<wstring, 7>& tetromino, FieldBuffer& pField,
	int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; ++px)
		for (int py = 0; py < 4; ++py)
		{
			int pi = Rotate(px, py, nRotation);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTetromino][pi] != L'.' && pField.isCellOccupied(nPosX + px, nPosY + py))
						return false;
				}
			}
		}
	return true;
}

int main()
{
	TetrominoManager<wstring, 7> tetromino;

	// Tetrominos 4x4
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	// Create play field buffer
	FieldBuffer pField(nFieldWidth, nFieldHeight);
	ScreenBuffer<wchar_t> screen(nScreenWidth, nScreenHeight);

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	COORD bufferSize = { static_cast<SHORT>(nScreenWidth), static_cast<SHORT>(nScreenHeight) };
	SMALL_RECT windowSize = {
		0,
		0,
		static_cast<SHORT>(nScreenWidth - 1),
		static_cast<SHORT>(nScreenHeight - 1)
	};
	SetConsoleActiveScreenBuffer(hConsole);
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
	DWORD dwBytesWritten = 0;

	bool bKey[4];

	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	int nSpeed = 20;
	int nSpeedCount = 0;

	bool bRotateHold = true;

	int nPieceCount = 0;
	int nScore = 0;

	vector<int> vLines;

	bool bGameOver = false;

	// Main game loop
	while (!bGameOver)
	{
		// GAME TIMING ========================

		std::this_thread::sleep_for(std::chrono::milliseconds(50)); // This will be one game tick
		nSpeedCount++;
		bool bForceDown = (nSpeedCount == nSpeed);

		// INPUT ==============================

		for (int k = 0; k < 4; k++)											// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState(static_cast<unsigned char>("\x27\x25\x28Z"[k]))) != 0;

		// GAME LOGIC =========================

		// Handle player movement

		// Right key
		nCurrentX += (bKey[0] && DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		// Left key
		nCurrentX -= (bKey[1] && DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		// Down key
		nCurrentY += (bKey[2] && DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		// Rotate, but latch to stop wild spinning
		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold = true;

		if (bForceDown)
		{
			// Increase the difficulty of the game
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 50 == 0)
				if (nSpeed >= 10) nSpeed--;

			// Can the piece move down?
			if (DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // Just do it!!!
			else
			{
				// Glue the current piece in that position
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
							pField.setCellValue(nCurrentX + px, nCurrentY + py, nCurrentPiece + 1);

				// Do we have full sync lines
				for (int py = 0; py < 4; py++)
				{
					if (nCurrentY + py < nFieldHeight - 1)
					{
						if (pField.isLineComplete(nCurrentY + py))
						{
							pField.clearLine(nCurrentY + py);
							vLines.push_back(nCurrentY + py);
						}
					}
				}

				nScore += 25;
				if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;

				// Choose next piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit at all
				bGameOver = !DoesPieceFit(tetromino, pField, nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
		}


		// RENDER OUTPUT ======================

		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen.drawCharacter(x + 2, y + 2, L" ABCDEFG=#"[pField.getCellValue(x, y)]);

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
					screen.drawCharacter(nCurrentX + px + 2, nCurrentY + py + 2, nCurrentPiece + 65);

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		// Animate Line Completion
		if (!vLines.empty())
		{
			WriteConsoleOutputCharacter(hConsole, screen.get(), nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			std::this_thread::sleep_for(std::chrono::milliseconds(400));

			for (auto& v : vLines)
				pField.collapseLine(v);

			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen.get(), nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);
	cout << "Game Over!! Score:" << nScore << '\n';
	system("pause");
	return 0;
}