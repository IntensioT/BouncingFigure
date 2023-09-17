#pragma once

#include <Windows.h>
#include <d2d1.h>

class Graphics
{
	ID2D1Factory* pFactory;

	ID2D1HwndRenderTarget* pRenderTarget; //представляет целевой объект отрисовки.
	ID2D1SolidColorBrush* brush; // представляет кисть.


public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void BeginDraw() { pRenderTarget->BeginDraw(); }
	void EndDraw() { pRenderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	
private:

};



