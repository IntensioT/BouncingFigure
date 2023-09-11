#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

ID2D1Factory* pFactory;
HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

ID2D1HwndRenderTarget* pRenderTarget; //представляет целевой объект отрисовки.
ID2D1SolidColorBrush* pBrush; // представляет кисть.
D2D1_ELLIPSE            ellipse;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void    CalculateLayout();
HRESULT CreateGraphicsResources(HWND m_hwnd);
void    DiscardGraphicsResources();
void    OnPaint(HWND m_hwnd);
void    Resize(HWND m_hwnd);

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

	ShowWindow(hwnd, nCmdShow);
	//-----------------------------------------------------------------------------------


	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg); // Перевод нажатия клавитуры, виртуального ключа в сообщение
		DispatchMessage(&msg);// Неявный вызов WindowProc
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		OnPaint(hwnd);
		return 0;
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void OnPaint(HWND m_hwnd)
{
	HRESULT hr = CreateGraphicsResources(m_hwnd);
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void Resize(HWND m_hwnd)
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

HRESULT CreateGraphicsResources(HWND m_hwnd)
{
	HRESULT hr = S_OK; //succes code

	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
	}

	return hr;
}

