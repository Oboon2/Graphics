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

	// �ڿ� ����
	ID3D11Device* device;
	// ������ �ڿ� ����
	ID3D11DeviceContext* deviceContext;
	// front���ۿ� back���۸� �ٲ���
	IDXGISwapChain* swapChain;
	// �׸� ���� ���� ����
	ID3D11RenderTargetView* renderTargetView;

	ID3D11DepthStencilView* depthStencilView;

	ID3D11Texture2D* depthStencilBuffer;

protected:

	bool InitializeDirect3D();
	bool InitializeDepthStencilBuffer();

	void SetViewport();

};

