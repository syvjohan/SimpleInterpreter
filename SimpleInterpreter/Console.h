#pragma once

#include <windows.h>
#include <cassert>

static void SetConsoleSize() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Interpreter for Programming language Q");

	CONSOLE_SCREEN_BUFFER_INFO sbInfo;
	GetConsoleScreenBufferInfo(hStdOut, &sbInfo);

	COORD coordinates;
	coordinates.X = 80;
	coordinates.Y = 1000;

	SMALL_RECT windowRect;
	windowRect.Top = 0;
	windowRect.Left = 0;
	windowRect.Bottom = 48;
	windowRect.Right = 79;

	//set textbuffer size
	assert(SetConsoleScreenBufferSize(hStdOut, coordinates));

	//change window size.
	assert(SetConsoleWindowInfo(hStdOut, true, &windowRect));
	
	HWND hwnd = GetConsoleWindow();

	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
}

static void ConsoleBackgroundAndTextColors(int ForgC, int BackC) {
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD coordinates;
	coordinates.X = 80;
	coordinates.Y = 1000;

	DWORD count;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//set the current color
	SetConsoleTextAttribute(hStdOut, wColor);
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		FillConsoleOutputCharacter(hStdOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coordinates, &count);

		FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coordinates, &count);
		SetConsoleCursorPosition(hStdOut, coordinates);
	}
	return;
}

/*
Name         | Value
             |
Black        |   0
Blue         |   1
Green        |   2
Cyan         |   3
Red          |   4
Magenta      |   5
Brown        |   6
Light Gray   |   7
Dark Gray    |   8
Light Blue   |   9
Light Green  |   10
Light Cyan   |   11
Light Red    |   12
Light Magenta|   13
Yellow       |   14
White        |   15
*/