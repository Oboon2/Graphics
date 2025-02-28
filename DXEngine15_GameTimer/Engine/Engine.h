#pragma once
#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "InputClass.h"
#include "GameTimer.h"
class Engine : public DXApp
{
private:
	struct PerSceneBuffer
	{
		PerSceneBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX viewProjection;
		XMFLOAT3 worldLightPosition;
		XMFLOAT3 worldCameraPosition;
	};


public:
	Engine(HINSTANCE hinstance);
	~Engine();

	int Run();
	bool Init() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;

	// 입력 처리 함수
	void ProcessInput(float deltaTime);

	// 타이머 관련 함수
	void UpdateTimer() { gameTimer->UpdateTimer(window); }
	float GetDeltaTime() const { return gameTimer->GetDeltaTime(); }
	int GetFPS() const { return gameTimer->GetFPS(); }

private:

	// 장면(Scene} 초기화
	bool InitializeScene();
	bool InitializeTransformation();

	// 입력(Input) 초기화
	void InitializeInput();

	// 타이머(Timer) 초기화
	void InitializeTimer();

private:
	Material* material;
	Material* material2;
	//VertexShader* vertexShader;
	//PixelShader* pixelShader;

	ID3D11Buffer* constantBuffer;		// 뷰, 투영행렬 버퍼

	// 카메라 정보
	//XMVECTOR cameraPosition;		// 카메라 위치
	//XMVECTOR cameraView;			// 카메라 방향
	//XMVECTOR cameraUpVector;		// 카메라 위 방향
	Camera* camera;					// 카메라

	Mesh* mesh;
	Mesh* mesh2;

	InputClass* input;

	// 게임 타이머
	GameTimer* gameTimer;
};

