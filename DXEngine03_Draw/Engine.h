#pragma once
#include "DXApp.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
class Engine : public DXApp
{
public:
	Engine(HINSTANCE hinstance);
	~Engine();

	int Run();
	bool Init() override;
	void Update() override;
	void Render() override;

private:

	// 장면(Scene} 초기화
	bool InitializeScene();


private:

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Mesh* mesh;

};

