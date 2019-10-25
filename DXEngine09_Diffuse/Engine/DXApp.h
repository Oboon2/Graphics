#pragma once

#include "AppWindow.h"
#include "DXUtil.h"

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	virtual int Run() = 0;
	virtual bool Init();
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:

	AppWindow* window;

	// 자원 생성
	ID3D11Device* device;
	// 생성한 자원 연동
	ID3D11DeviceContext* deviceContext;
	// front버퍼와 back버퍼를 바꿔줌
	IDXGISwapChain* swapChain;
	// 그림 파일 정보 저장
	ID3D11RenderTargetView* renderTargetView;

	ID3D11DepthStencilView* depthStencilView;

	ID3D11Texture2D* depthStencilBuffer;

protected:

	bool InitializeDirect3D();
	bool InitializeDepthStencilBuffer();

	void SetViewport();

};

