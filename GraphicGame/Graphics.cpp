#include "Graphics.h"

Graphics::Graphics()
{
	pFactory = NULL;
	pRenderTarget = NULL;
	brush = NULL;
}

Graphics::~Graphics()
{
	if (pFactory) pFactory->Release(); //Com interfaces should be releasesd
	if (pRenderTarget) pRenderTarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
	if (res != S_OK)
	{
		return false;
	}

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&pRenderTarget);

	if (res != S_OK)
	{
		return false;
	}

	res = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0,0,0,0), &brush);

	if (res != S_OK)
	{
		return false;
	}

	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	pRenderTarget->Clear(D2D1::ColorF(r,g,b));
}
void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2(x, y), radius, radius), brush, 3.0f);
}
