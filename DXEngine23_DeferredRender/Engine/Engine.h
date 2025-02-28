#pragma once
#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "InputClass.h"
#include "GameTimer.h"

#include "DeferredBuffer.h"
#include "DeferredRenderer.h"

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

	// 메쉬 초기화
	bool InitializeMeshes();

	// 렌더 함수
	void RenderToTexture();
	void RenderScene();
	void BeginScene(float color[]);

	// 투영행렬 업데이트 함수
	void UpdatePerspectiveCamera();
	void UpdateOthographicCamera();

	// 디퍼드 버퍼 초기화
	bool InitializeDeferredBuffers();

private:
	//Material* material;
	//Material* material2;


	ID3D11Buffer* constantBuffer;		// 뷰, 투영행렬 버퍼

	Camera* camera;					// 카메라

	//Mesh* mesh;
	//Mesh* mesh2;

	// 메쉬 배열
	std::vector<Mesh*> meshes;

	InputClass* input;

	// 게임 타이머
	GameTimer* gameTimer;

	// 디퍼드 변수.
	Material* deferredMaterial = NULL;
	DeferredBuffer* deferredBuffer = NULL;
	DeferredRenderer* deferredRenderer = NULL;
};

