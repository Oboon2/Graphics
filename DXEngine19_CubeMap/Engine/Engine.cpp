#include "Engine.h"



Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{
}


Engine::~Engine()
{
	//Memory::SafeDelete(mesh);
	//Memory::SafeDelete(mesh2);
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		Memory::SafeDelete((*mesh));
	}

	//material->Release();
	//Memory::SafeDelete(material);

	//material2->Release();
	//Memory::SafeDelete(material2);

	Memory::SafeDelete(camera);
	Memory::SafeDelete(input);

	Memory::SafeRelease(constantBuffer);
}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{

	if (DXApp::Init() == false)
		return false;

	// 타이머 초기화
	InitializeTimer();

	// 입력 초기화
	InitializeInput();


	// 장면 초기화
	if (InitializeScene() == false)
		return false;

	// 행렬 초기화
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update(float deltaTime)
{
	// 월드 행렬 바인딩
	//mesh->Update(deviceContext);
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectionMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);

	// 뷰/투영 행렬 바인딩.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	float color[] = { 0.0f, 0.7f, 0.7f, 1.0f };

	// 렌더 타겟을 설정한 색상으로 칠하기
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// 뎁스/스텐실 뷰 지우기
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		// 월드 행렬 바인딩
		(*mesh)->Update(deviceContext);

		// 셰이더 바인딩
		(*mesh)->BindShaders(deviceContext);

		// 텍스처 / 샘플러 스테이트 바인딩
		(*mesh)->BindTextures(deviceContext);
		(*mesh)->BindSamplerState(deviceContext);

		// 래스터라이저 스테이트 바인딩
		(*mesh)->BindRasterizerState(deviceContext);

		// 메시 버퍼 그리기
		(*mesh)->RenderBuffers(deviceContext);
	}

	swapChain->Present(1, 0);
}

void Engine::ProcessInput(float deltaTime)
{
	// ESC 종료 처리
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{
		if (MessageBox(NULL, TEXT("종료하시겠습니까?"), TEXT("종료"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(window->GetWindowHandle());
		}
	}

	float moveSpeed = 100.0f;

	// 카메라 이동/회전
	if (input->IsKeyDown(Keyboard::Keys::A) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveRight(-1.0f * moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::D) || input->IsKeyDown(Keyboard::Keys::Right))
	{
		camera->MoveRight(1.0f * moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::W) || input->IsKeyDown(Keyboard::Keys::Up))
	{
		camera->MoveForward(1.0f * moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::S) || input->IsKeyDown(Keyboard::Keys::Down))
	{
		camera->MoveForward(-1.0f * moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::Q))
	{
		camera->MoveUp(1.0f * moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::E))
	{
		camera->MoveUp(-1.0f * moveSpeed * deltaTime);
	}

	float rotationSpeed = 10.0f;
	// 마우스 왼쪽 버튼이 눌릴 때만 이동
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x * rotationSpeed * deltaTime);
		camera->Pitch(state.y * rotationSpeed * deltaTime);
	}
}

bool Engine::InitializeScene()
{
	// FBX 이름 변수
	LPCSTR fbxTPP = "Resources/Models/HeroTPP.FBX";
	LPCSTR fbxBox = "Resources/Models/SK_CharM_Cardboard.FBX";
	LPCSTR fbxWarrior = "Resources/Models/SK_CharM_Warrior.FBX";
	LPCSTR fbxBarbarous = "Resources/Models/SK_CharM_Barbarous.FBX";
	LPCSTR fbxEnemyBear = "Resources/Models/Enemy_Bear.FBX";
	LPCSTR fbxCube = "Resources/Models/cube.FBX";
	LPCSTR fbxSphere = "Resources/Models/sphere.FBX";

	// 텍스처 이름 변수
	LPCTSTR tppDiffuseTexture = TEXT("Resources/Textures/T_Chr_FPS_D.png");
	LPCTSTR tppNormalTexture = TEXT("Resources/Textures/T_Chr_FPS_N.png");

	LPCTSTR boxDiffuseTexture = TEXT("Resources/Textures/Char_M_Cardboard_D.png");
	LPCTSTR boxNormalTexture = TEXT("Resources/Textures/Char_M_Cardboard_N.png");

	LPCTSTR warriorDiffuseTexture = TEXT("Resources/Textures/T_CharM_Warrior_D.TGA");
	LPCTSTR warriorNormalTexture = TEXT("Resources/Textures/T_CharM_Warrior_N.TGA");

	LPCTSTR barbarousDiffuseTexture = TEXT("Resources/Textures/T_Barbarous_D.TGA");
	LPCTSTR barbarousNormalTexture = TEXT("Resources/Textures/T_Barbarous_N.TGA");

	LPCTSTR enemyBearDiffuseTexture = TEXT("Resources/Textures/T_Enemy_Bear_D.TGA");
	LPCTSTR enemyBearNormalTexture = TEXT("Resources/Textures/T_Enemy_Bear_N.TGA");

	LPCTSTR lightStepTexture = TEXT("Resources/Textures/LightStep.png");
	LPCTSTR warpTexture = TEXT("Resources/Textures/Warp.png");

	LPCTSTR yokohamaTexture = TEXT("Resources/Textures/Yokohama.dds");
	LPCTSTR stairsTexture = TEXT("Resources/Textures/Stairs.dds");
	LPCTSTR lyckseleTexture = TEXT("Resources/Textures/Lycksele.dds");

	// 셰이더 이름 변수
	LPCTSTR blinnPhongShader = TEXT("Shader/BliinPhong");
	LPCTSTR diffuseShader = TEXT("Shader/Diffuse");
	LPCTSTR warpDiffuseShader = TEXT("Shader/WarpDiffuse");
	LPCTSTR specularShader = TEXT("Shader/Specular");
	LPCTSTR toonShader = TEXT("Shader/Toon");
	LPCTSTR toon2Shader = TEXT("Shader/Toon2");
	LPCTSTR normalMappingShader = TEXT("Shader/NormalMapping");
	LPCTSTR normalMappingShader2 = TEXT("Shader/NormalMapping2");
	LPCTSTR cubeMappingShader = TEXT("Shader/CubeMapping");

	//배경 메쉬 생성
	Mesh* sphere1 = new Mesh(fbxSphere, cubeMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	sphere1->SetPosition(0.0f, -90.0f, 0.0f);
	sphere1->SetScale(10000.0f, 10000.0f, 10000.0f);
	sphere1->AddTexture(yokohamaTexture);
	meshes.push_back(sphere1);

	Mesh* sphere2 = new Mesh(fbxSphere, cubeMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	sphere2->SetPosition(10000.0f, -90.0f, 0.0f);
	sphere2->SetScale(100.0f, 100.0f, 100.0f);
	sphere2->AddTexture(stairsTexture);
	meshes.push_back(sphere2);

	Mesh* sphere3 = new Mesh(fbxSphere, cubeMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	sphere3->SetPosition(20000.0f, -90.0f, 0.0f);
	sphere3->SetScale(100.0f, 100.0f, 100.0f);
	sphere3->AddTexture(lyckseleTexture);
	meshes.push_back(sphere3);


	// 메쉬 생성
	Mesh* mesh1 = new Mesh(fbxTPP, normalMappingShader2, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	mesh1->SetPosition(-300.0f, -90.0f, 0.0f);
	mesh1->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh1->AddTexture(tppDiffuseTexture);
	mesh1->AddTexture(tppNormalTexture);
	meshes.push_back(mesh1);

	Mesh* mesh2 = new Mesh(fbxBox, normalMappingShader2, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	mesh2->SetPosition(-150.0f, -90.0f, 0.0f);
	mesh2->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh2->AddTexture(boxDiffuseTexture);
	mesh2->AddTexture(boxNormalTexture);
	meshes.push_back(mesh2);

	Mesh* mesh3 = new Mesh(fbxEnemyBear, normalMappingShader2);
	mesh3->SetPosition(0.0f, -90.0f, 0.0f);
	mesh3->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh3->AddTexture(enemyBearDiffuseTexture);
	mesh3->AddTexture(enemyBearNormalTexture);
	meshes.push_back(mesh3);

	Mesh* mesh4 = new Mesh(fbxWarrior, normalMappingShader2);
	mesh4->SetPosition(150.0f, -90.0f, 0.0f);
	mesh4->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh4->AddTexture(warriorDiffuseTexture);
	mesh4->AddTexture(warriorNormalTexture);
	meshes.push_back(mesh4);

	Mesh* mesh5 = new Mesh(fbxBarbarous, normalMappingShader2);
	mesh5->SetPosition(300.0f, -90.0f, 0.0f);
	mesh5->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh5->AddTexture(barbarousDiffuseTexture);
	mesh5->AddTexture(barbarousNormalTexture);
	meshes.push_back(mesh5);
	
	// 메쉬 초기화
	if (InitializeMeshes() == false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// 시야각 / 종횡비 설정
	float fovY = XMConvertToRadians(100.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	// 카메라 객체 생성 
	camera = new Camera(fovY, aspectRatio, 1.0f, 1000000.0f);
	
	// 버퍼에 담을 구조체 변수 설정
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectionMatrix());

	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	// 버퍼 생성
	// 버퍼 서술자

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 배열 정보 넣어줄 구조체
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// 상수 버퍼 생성
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("씬 버퍼 생성 실패")))
	{
		return false;
	}
	return true;
}

void Engine::InitializeInput()
{
	input = new InputClass();
	input->InitializeInput(window->GetWindowHandle());
}

void Engine::InitializeTimer()
{
	gameTimer = new GameTimer();
	gameTimer->StartTimer();
}

bool Engine::InitializeMeshes()
{
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		// 머티리얼 컴파일
		if ((*mesh)->CompileShaders(device) == false)
			return false;
		

		// 각 셰이더 객체 생성
		if ((*mesh)->CreateShaders(device) == false)
			return false;

		// 텍스처 로드
		if ((*mesh)->LoadTextures(device) == false)
			return false;

		// 샘플러 스테이트 생성
		if ((*mesh)->CreateSamplerState(device) == false)
			return false;

		// 버퍼 초기화
		if ((*mesh)->InitializeBuffers(device) == false)
			return false;

		// 래스터라이저 스테이트 생성
		if ((*mesh)->CreateRasterizerState(device) == false)
			return false;
	}
	return true;
}
