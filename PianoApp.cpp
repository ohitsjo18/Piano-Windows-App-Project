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

// Hi 

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")


// Global variables 
HRGN whiteKeysRgnArray[8];
HRGN blackKeysRgnArray[5];

#define WM_LBUTTONDOWN 0x0201

//----------------- HELPER METHODS ---------------------------------------------------------------
// Helper method for conversion from string to LPCWSTR
// Ref: https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
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

//--------------- START OF APPLICATION CODE -----------------------------------------------------

// OnPaint fn for draw white keys: 
VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);

	// Create a pen object: 
	Pen pen(Color(255, 0, 0, 0), 3);

	// Create a SolidBrush objects: (for filling shape) 
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
	// Populate array of coordinates manually (not uniform) 
	float blackKey0_x1 = 175.0f;
	float blackKey0_y1 = 100.0f;
	float blackKey0_width;
	float blackKey0_height;

	// Black keys array counter 
	int blackKeyCounter = 0;

	// Populate arrays for key rectangles and regions: 
	for (int i = 0; i < 8; i++)
	{
		// Populate rectangle and region arrays of WHITE keys: 
		whiteKeysRectArray[i] = RectF(rectx1, recty1, rectWidth, rectHeight);
		whiteKeysRgnArray[i] = CreateRectRgn((int)whiteKeysRectArray[i].GetLeft(), (int)whiteKeysRectArray[i].GetTop(),
			(int)whiteKeysRectArray[i].GetRight(), (int)whiteKeysRectArray[i].GetBottom());

		// Draw white key 
		graphics.DrawRectangle(&pen, whiteKeysRectArray[i]);

		// Fill the white key: 
		graphics.FillRectangle(&whiteKeyBrush, whiteKeysRectArray[i]);

		// Draw black keys relative to white keys: 
		if ((i != 2) && (i != 6) && (i != 7) && blackKeyCounter < 5) // If black key valid 
		{
			blackKeysRectArray[blackKeyCounter] = RectF(rectx1 + 75.0f, blackKey0_y1, 50.0f, 100.0f);
			// Increment counter
			blackKeyCounter++;
		}

		// Update to next rectangle: 
		rectx1 = rectx1 + 100.0f;
	}

	// Finally, draw the black rectangles and populate region array: 
	for (int i = 0; i < 5; i++)
	{
		// Black key regions
		blackKeysRgnArray[i] = CreateRectRgn((int)blackKeysRectArray[i].GetLeft(), (int)blackKeysRectArray[i].GetTop(),
			(int)blackKeysRectArray[i].GetRight(), (int)blackKeysRectArray[i].GetBottom());

		// Draw the black key rectangles: 
		graphics.DrawRectangle(&pen, blackKeysRectArray[i]);

		// Fill the black key rectangles: 
		graphics.FillRectangle(&blackKeyBrush, blackKeysRectArray[i]);
	}
} // OnPaint End 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
		TEXT("Playing with Shapes"),    // window caption
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
}  // WinMain End 

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

		// Display a message box on mouse click detected 
		//int msgBoxID = MessageBox(
		//	hWnd,
		//	(LPCWSTR)L"LMB Click Detected",
		//	(LPCWSTR)L"Message Box Test",
		//	MB_OK);

		// Print coordinates to the console (test, TODO delete) 
		string xstr = to_string(posx);
		string ystr = to_string(posy);
		string coords = "[x: " + xstr + ", y: " + ystr + "]";
		std::wstring stemp = s2ws(coords);
		LPCWSTR coords_string = stemp.c_str();
		OutputDebugStringW(coords_string);

		// Detect if mouse click is in key regions:  
		LPCWSTR whiteNotes[8];
		LPCWSTR blackNotes[5];

		whiteNotes[0] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.C4.wav";
		whiteNotes[1] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.D4.wav";
		whiteNotes[2] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.E4.wav";
		whiteNotes[3] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.F4.wav";
		whiteNotes[4] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.G4.wav";
		whiteNotes[5] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.A4.wav";
		whiteNotes[6] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.B4.wav";
		whiteNotes[7] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.C5.wav";

		blackNotes[0] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.Db4.wav";
		blackNotes[1] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.Eb4.wav";
		blackNotes[2] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.Gb4.wav";
		blackNotes[3] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.Ab4.wav";
		blackNotes[4] = (LPCWSTR)L"C:\\Users\\CStriker\\Documents\\C++ Projects Master Folder\\Piano Windows App Project\\Source Files\\Piano Sound Files\\WAV Files\\Piano.mf.Bb4.wav";

		for (int i = 0; i < 8; i++)
		{
			if ((i < 5) && PtInRegion(blackKeysRgnArray[i], posx, posy))
			{
				PlaySound(blackNotes[i], NULL, SND_SYNC);
				break;
			}
			else if (PtInRegion(whiteKeysRgnArray[i], posx, posy) && PtInRegion(blackKeysRgnArray[i], posx, posy) == FALSE)
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