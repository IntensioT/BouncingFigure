#include "Graphics.h"
#include "Resource.h"
#include <math.h>


Graphics* graphics;
int direction = 0;
RECT rectOwner;
float screenR;
float screenG;
float screenB;
float brushR;
float brushG;
float brushB;

void UpdatePos(float* x, float* y, float* xSpeed, float* ySpeed, RECT rect);
void ChangeSpeed(float* xSpeed, float* ySpeed);


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			// Process the LEFT ARROW key. 
			direction = 1;
			return 0;

		case VK_RIGHT:
			// Process the RIGHT ARROW key. 
			direction = 2;
			return 0;

		case VK_UP:
			// Process the UP ARROW key. 
			direction = 3;
			return 0;

		case VK_DOWN:
			// Process the DOWN ARROW key. 
			direction = 4;
			return 0;

		}

	case WM_KEYUP:
		direction = 0;
		return 0;

	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (!(GetKeyState(VK_SHIFT) < 0))
		{
			
			if (delta > 0)
			{
				//SetWindowPos(hwnd)
				SetWindowPos(hwnd,
					HWND_TOP,
					rectOwner.left,
					rectOwner.top + 50,
					0, 0,          // Ignores size arguments. 
					SWP_NOSIZE);
			}
			else if (delta < 0)
			{
				SetWindowPos(hwnd,
					HWND_TOP,
					rectOwner.left,
					rectOwner.top - 50,
					0, 0,          // Ignores size arguments. 
					SWP_NOSIZE);
			}
		}
		else
		{
			if (delta > 0)
			{
				SetWindowPos(hwnd,
					HWND_TOP,
					rectOwner.left + 50,
					rectOwner.top,
					0, 0,          // Ignores size arguments. 
					SWP_NOSIZE);
			}
			else if (delta < 0)
			{
				SetWindowPos(hwnd,
					HWND_TOP,
					rectOwner.left - 50,
					rectOwner.top,
					0, 0,          // Ignores size arguments. 
					SWP_NOSIZE);
			}
		}
		return 0;
	}

	case WM_SYSKEYUP:
	{
		WORD vkCode = LOWORD(wParam);                                 // virtual-key code

		WORD keyFlags = HIWORD(lParam);

		WORD scanCode = LOBYTE(keyFlags);                             // scan code

		switch (scanCode)
		{
		case 44:
			direction = 10;
			return 0;
		}
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TOGGLE_MODE:
			screenR = fmod((screenR + 0.25f), 1);
			screenG = fmod((screenG + 0.25f), 1);
			screenB = fmod((screenB + 0.25f), 1);
			return 0;
		}
		return 0;
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
	screenR = 1.0f;
	screenG = 1.0f;
	screenB = 1.0f;
	brushR = 1.0f;
	brushG = 0.0f;
	brushB = 0.0f;
	RECT rect;
	//----------------------------------------------------------------------------------

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));

	// Run the message loop.

	MSG msg = { };
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			GetWindowRect(hwnd, &rectOwner);
			if (!TranslateAccelerator(hwnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			GetClientRect(hwnd, &rect);
		}
		else
		{
			//Update
			UpdatePos(&x, &y, &xSpeed, &ySpeed, rect);

			//Render
			graphics->BeginDraw();
			graphics->ClearScreen(screenR, screenG, screenB);
			graphics->DrawCircle(x, y, 50, brushR, brushG, brushB, 1);

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


void UpdatePos(float* x, float* y, float* xSpeed, float* ySpeed, RECT rect)
{
	//*xSpeed += 1.0f;
	ChangeSpeed(xSpeed, ySpeed);
	*x += *xSpeed;
	*y += *ySpeed;


	if (*y > rect.bottom)
	{
		*y = rect.bottom;
		*ySpeed = 0 - *ySpeed;
	}
	if (*y < rect.top)
	{
		*y = rect.top;
		*ySpeed = 0 - *ySpeed;
	}
	if (*x > rect.right)
	{
		*x = rect.right;
		*xSpeed = 0 - *xSpeed;
	}
	if (*x < rect.left)
	{
		*x = rect.left;
		*xSpeed = 0 - *xSpeed;
	}

	//*xSpeed /= 2;
	//*ySpeed /= 2;

	if (*xSpeed < 0.001f && *xSpeed > -0.001f) { *xSpeed = 0; }
	if (*ySpeed < 0.001f && *ySpeed > -0.001f) { *ySpeed = 0; }
}

void ChangeSpeed(float* xSpeed, float* ySpeed)
{
	switch (direction)
	{
	case 1:
		*xSpeed -= 0.3f;
		return;
	case 2:
		*xSpeed += 0.3f;
		return;
	case 3:
		*ySpeed -= 0.3f;
		return;
	case 4:
		*ySpeed += 0.3f;
		return;
	case 10:
		*xSpeed = 0;
		*ySpeed = 0;
		return;
	case 0:
		if (*xSpeed >= 0) *xSpeed -= 0.01f;
		else *xSpeed += 0.01f;
		if (*ySpeed >= 0) *ySpeed -= 0.01f;
		else *ySpeed += 0.01f;
		return;
	}
}
