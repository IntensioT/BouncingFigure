#include "Graphics.h"


Graphics* graphics;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

	/*case WM_PAINT:
	{
		graphics->BeginDraw();

		graphics->ClearScreen(255,255,255);
		graphics->DrawCircle(100, 100, 50, 1.0f, 0, 0, 1);

		graphics->EndDraw();
		return 0;
	}*/

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class"; //unsigned short wchar_t;


	WNDCLASS wc = { };

	/*typedef struct tagWNDCLASSW {
		UINT        style;
		WNDPROC     lpfnWndProc;
		int         cbClsExtra;
		int         cbWndExtra;
		HINSTANCE   hInstance;
		HICON       hIcon;
		HCURSOR     hCursor;
		HBRUSH      hbrBackground;
		LPCWSTR     lpszMenuName;
		LPCWSTR     lpszClassName;*/

	wc.lpfnWndProc = WindowProc; //Pointer to a procedure of Window
	wc.hInstance = hInstance; //Handle of an Instance of 
	wc.lpszClassName = CLASS_NAME; //Identificator
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);
	//--------------------------------------------------------------------------------


	// Create the window.
	//hwnd — это переменная, содержащая дескриптор
	HWND hwnd = CreateWindowEx(
		0,                              // DWORD     dwExStyle, Optional window styles.
		CLASS_NAME,                     // LPCSTR    lpClassName, Window class
		L"Learn to Program Windows",    // LPCSTR    lpWindowName, Window text
		WS_OVERLAPPEDWINDOW,            //DWORD     dwStyle, Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // HWND      hWndParent, Parent window    
		NULL,       // HMENU     hMenu, Menu
		hInstance,  // HINSTANCE hInstance, Instance handle
		NULL        // LPVOID    lpParam, Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	graphics = new Graphics();

	if (!(graphics->Init(hwnd))) 
	{
		delete graphics;
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	//-----------------------------------------------------------------------------------
	float x = 100.0f;
	float y = 100.0f;
	float xSpeed = 0;
	float ySpeed = 0;
	RECT rect;
	//----------------------------------------------------------------------------------


	// Run the message loop.

	MSG msg = { };
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);

			GetClientRect(hwnd, &rect);
		}
		else
		{
			//Update
			xSpeed -= 1.0f;
			x += ySpeed;
			if (y > rect.bottom)
			{
				y = rect.bottom;
				ySpeed = -30.0f;
			}
			if (y < rect.top)
			{
				y = rect.top;
				ySpeed = +30.0f;
			}
			if (x > rect.right)
			{
				x = rect.right;
				xSpeed = -30.0f;
			}
			if (x < rect.left)
			{
				x = rect.left;
				xSpeed = +30.0f;
			}

			//Render
			graphics->BeginDraw();
			graphics->ClearScreen(1.0f, 1.0f, 1.0f);
			graphics->DrawCircle(x, y, 50, 1.0f, 0, 0, 1);

			graphics->EndDraw();
		}
	}

	//while (GetMessage(&msg, NULL, 0, 0) > 0)
	//{
	//	TranslateMessage(&msg); // Перевод нажатия клавитуры, виртуального ключа в сообщение
	//	DispatchMessage(&msg);// Неявный вызов WindowProc
	//}

	delete graphics;

	return 0;
}