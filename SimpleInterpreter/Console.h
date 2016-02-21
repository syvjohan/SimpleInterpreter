#pragma once

#include <iostream>
#include <stdexcept>
#include <windows.h>

static void SetConsoleSize(int x, int y) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Interpreter for Programming language Q");

	COORD coordinates;
	coordinates.X = x;
	coordinates.Y = y;

	SMALL_RECT windowRect;
	windowRect.Top = 0;
	windowRect.Left = 0;
	windowRect.Bottom = x - 1;
	windowRect.Right = y - 1;

	// Adjust buffer size:
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleScreenBufferSize(handle, coordinates))
		throw std::runtime_error("Unable to resize screen buffer.");

	// display as a maximized window
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}


static void ConsoleBackgroundAndTextColors(int ForgC, int BackC) {
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD coordinates;
	coordinates.X = 0;
	coordinates.Y = 0;

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