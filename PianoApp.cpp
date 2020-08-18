/* 

C++ Windows Piano Application Project 

Description: The program below creates a basic windows-based piano application 
which plays piano note sounds when the appropriate region of the screen ('key') is
pressed. 

Author: Joanne Nock 
Date: 18/8/2020

Feel free to use this program for your own projects, but please be aware of the 
references I have highlighted throughout the code. 

✿ Joanne Nock ✿ MEng Computer Systems Engineering ✿ University of Warwick ✿

*/

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Wingdi.h>
#include <Winuser.h>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

// Global variables 
HRGN whiteKeysRgnArray[8];
HRGN blackKeysRgnArray[5];

#define WM_LBUTTONDOWN 0x0201

//----------------- HELPER METHODS ---------------------------------------------------------------

// Helper method for conversion from string to LPCWSTR type 
// Ref: https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
// *** Unused except for testing purposes - delete when project complete ***
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//--------------- START OF APPLICATION CODE ------------------------------------------------------

// Function OnPaint()
// Function: paint graphics on client window 
// Description: enacts the drawing of objects in the client window 
// Parameters: hdc - handler to device context associated with Graphics objects 
// Return: No return specified (called continuously as part of WindProc() )
// Reference: https://docs.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-drawing-a-string-use
VOID OnPaint(HDC hdc)
{
	// Instantiate required graphics objects for drawing 
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 0), 3);
	SolidBrush whiteKeyBrush(Color(255, 255, 255, 255));
	SolidBrush blackKeyBrush(Color(255, 0, 0, 0));

	// Create array of 8 RectF objects (white keys)
	RectF whiteKeysRectArray[8];
	// Starter rectangle coordinates: (first white key) 
	float rectx1 = 100.0f;
	float recty1 = 100.0f;
	float rectWidth = 100.0f;
	float rectHeight = 200.0f;
	// Create array of 5 RectF objects (black keys) 
	RectF blackKeysRectArray[5];
	// Dimensions and position of black keys: 
	float blackKey_offset = 75.0f; // position of black keys relative to white keys
	float blackKey_depth = 100.0f;
	float blackKey_width = 50.0f; 
	float blackKey_height = 100.0f; 

	// Black keys array counter 
	int blackKeyCounter = 0;

	// Populate arrays for key rectangles and regions: 
	for (int i = 0; i < 8; i++)
	{
		// Populate rectangle and region arrays of WHITE keys: 
		whiteKeysRectArray[i] = RectF(rectx1, recty1, rectWidth, rectHeight);
		whiteKeysRgnArray[i] = CreateRectRgn((int)whiteKeysRectArray[i].GetLeft(), 
			(int)whiteKeysRectArray[i].GetTop(),(int)whiteKeysRectArray[i].GetRight(), 
			(int)whiteKeysRectArray[i].GetBottom());

		// Draw white key 
		graphics.DrawRectangle(&pen, whiteKeysRectArray[i]);

		// Fill white key: 
		graphics.FillRectangle(&whiteKeyBrush, whiteKeysRectArray[i]);

		// Draw black keys relative to white keys (if black key valid): 
		if ((i != 2) && (i != 6) && (i != 7) && blackKeyCounter < 5)
		{
			blackKeysRectArray[blackKeyCounter] = RectF(rectx1 + blackKey_offset, 
				blackKey_depth, blackKey_width, blackKey_height);
			blackKeyCounter++;
		}

		// Update to next white rectangle: 
		rectx1 = rectx1 + 100.0f;
	}

	// Finally, draw the black rectangles and populate region array: 
	// (black rectangles and regions must be drawn AFTER white, so they are superimposed) 
	for (int i = 0; i < 5; i++)
	{
		// Black key regions
		blackKeysRgnArray[i] = CreateRectRgn((int)blackKeysRectArray[i].GetLeft(), 
			(int)blackKeysRectArray[i].GetTop(), (int)blackKeysRectArray[i].GetRight(), 
			(int)blackKeysRectArray[i].GetBottom());

		// Draw the black key rectangles: 
		graphics.DrawRectangle(&pen, blackKeysRectArray[i]);

		// Fill the black key rectangles: 
		graphics.FillRectangle(&blackKeyBrush, blackKeysRectArray[i]);
	}
} // OnPaint() End 

// Function declaration for WndProc() 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Function WinMain()
// Function: Entry point of windows application program 
// Reference: https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=vs-2019
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL; 
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),         // window class name
		TEXT("Windows Piano"),			// window caption
		WS_OVERLAPPEDWINDOW,            // window style
		CW_USEDEFAULT,                  // initial x position
		CW_USEDEFAULT,                  // initial y position
		CW_USEDEFAULT,                  // initial x size
		CW_USEDEFAULT,                  // initial y size
		NULL,                           // parent window handle
		NULL,                           // window menu handle
		hInstance,                      // program instance handle
		NULL);                          // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain() End 

// Function WndProc() 
// Function: Window-procedure function for event handling
// Description: Handles messages that Windows receives when events occur
// Reference: https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=vs-2019
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONDOWN: // executed on left mouse click 
	{
		// Extract x,y coordinates of the mouse click 
		int posx = GET_X_LPARAM(lParam);
		int posy = GET_Y_LPARAM(lParam);

		// Print coordinates to the console (test: TODO delete) 
		string xstr = to_string(posx);
		string ystr = to_string(posy);
		string coords = "[x: " + xstr + ", y: " + ystr + "]";
		std::wstring stemp = s2ws(coords);
		LPCWSTR coords_string = stemp.c_str();
		OutputDebugStringW(coords_string);

		// Arrays of sound file locations 
		LPCWSTR whiteNotes[8];
		LPCWSTR blackNotes[5];

		// Specify location of sound files in system: 
		whiteNotes[0] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[1] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[2] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[3] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[4] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[5] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[6] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		whiteNotes[7] = (LPCWSTR)L"<wav_file_location_in_local_system>";

		blackNotes[0] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		blackNotes[1] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		blackNotes[2] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		blackNotes[3] = (LPCWSTR)L"<wav_file_location_in_local_system>";
		blackNotes[4] = (LPCWSTR)L"<wav_file_location_in_local_system>";

		// Detect mouse clicks in regions and play appropriate sounds
		for (int i = 0; i < 8; i++)
		{
			if ((i < 5) && PtInRegion(blackKeysRgnArray[i], posx, posy))
			{
				PlaySound(blackNotes[i], NULL, SND_SYNC);
				break;
			}
			else if (PtInRegion(whiteKeysRgnArray[i], posx, posy) 
				&& PtInRegion(blackKeysRgnArray[i], posx, posy) == FALSE)
			{
				PlaySound(whiteNotes[i], NULL, SND_SYNC);
				break;
			}
		}

		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam); 
	}
	}
} // WndProc End 

//-------------- END OF APPLICATION CODE ------------------------------------------------------
