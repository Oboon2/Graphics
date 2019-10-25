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

	// Ÿ�̸� �ʱ�ȭ
	InitializeTimer();

	// �Է� �ʱ�ȭ
	InitializeInput();


	// ��� �ʱ�ȭ
	if (InitializeScene() == false)
		return false;

	// ��� �ʱ�ȭ
	if (InitializeTransformation() == false)
		return false;

	// ���۵� ����/������ �ʱ�ȭ
	if (InitializeDeferredBuffers() == false)
		return false;

	return true;
}

void Engine::Update(float deltaTime)
{
	// ���� ��� ���ε�
	//mesh->Update(deviceContext);
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectionMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	OutputDebugString(TEXT("X: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().x).c_str());
	OutputDebugString(TEXT("Y: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().y).c_str());
	OutputDebugString(TEXT("Z: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().z).c_str());
	OutputDebugString(TEXT("\n"));

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);

	// ��/���� ��� ���ε�
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	// ���� �� �ؽ�ó
	UpdatePerspectiveCamera();
	RenderToTexture();

	float color[] = { 0.0f, 0.7f, 0.7f, 1.0f };
	BeginScene(color);

	// ���� ����
	UpdateOthographicCamera();

	// ��Ƽ���� ���ε�
	deferredMaterial->BindShaders(deviceContext);
	deferredMaterial->BindSamplerState(deviceContext);
	deferredMaterial->BindTextures(deviceContext);

	// ���۵� ���������� �׸���
	deferredRenderer->Update(deviceContext);
	deferredRenderer->Render(deviceContext);

	swapChain->Present(1, 0);
}

void Engine::ProcessInput(float deltaTime)
{
	// ESC ���� ó��
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{
		if (MessageBox(NULL, TEXT("�����Ͻðڽ��ϱ�?"), TEXT("����"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(window->GetWindowHandle());
		}
	}

	float moveSpeed = 200.0f;

	// ī�޶� �̵�/ȸ��
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
	// ���콺 ���� ��ư�� ���� ���� �̵�
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x * rotationSpeed * deltaTime);
		camera->Pitch(state.y * rotationSpeed * deltaTime);
	}
}

bool Engine::InitializeScene()
{
	// FBX �̸� ����
	LPCSTR fbxTPP = "Resources/Models/HeroTPP.FBX";
	LPCSTR fbxBox = "Resources/Models/SK_CharM_Cardboard.FBX";
	LPCSTR fbxWarrior = "Resources/Models/SK_CharM_Warrior.FBX";
	LPCSTR fbxBarbarous = "Resources/Models/SK_CharM_Barbarous.FBX";
	LPCSTR fbxEnemyBear = "Resources/Models/Enemy_Bear.FBX";
	LPCSTR fbxCube = "Resources/Models/cube.FBX";
	LPCSTR fbxSphere = "Resources/Models/sphere.FBX";

	// �ؽ�ó �̸� ����
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

	// ���̴� �̸� ����
	LPCTSTR blinnPhongShader = TEXT("Shader/BliinPhong");
	LPCTSTR diffuseShader = TEXT("Shader/Diffuse");
	LPCTSTR warpDiffuseShader = TEXT("Shader/WarpDiffuse");
	LPCTSTR specularShader = TEXT("Shader/Specular");
	LPCTSTR toonShader = TEXT("Shader/Toon");
	LPCTSTR toon2Shader = TEXT("Shader/Toon2");
	LPCTSTR normalMappingShader = TEXT("Shader/NormalMapping");
	LPCTSTR normalMappingShader2 = TEXT("Shader/NormalMapping2");
	LPCTSTR cubeMappingShader = TEXT("Shader/CubeMapping");
	LPCTSTR rimShader = TEXT("Shader/Rim");
	LPCTSTR orenNayerShader = TEXT("Shader/OrenNayer");
	LPCTSTR cookTorranceShader = TEXT("Shader/CookTorrance");
	LPCTSTR deferredShader = TEXT("Shader/Deferred");

	//��� �޽� ����
	//Mesh* sphere1 = new Mesh(fbxSphere, cubeMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	//sphere1->SetPosition(0.0f, -90.0f, 0.0f);
	//sphere1->SetScale(10000.0f, 10000.0f, 10000.0f);
	//sphere1->AddTexture(yokohamaTexture);
	//meshes.push_back(sphere1);


	// �޽� ����
	Mesh* mesh1 = new Mesh(fbxTPP, deferredShader);
	mesh1->SetPosition(-300.0f, -90.0f, 0.0f);
	mesh1->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh1->AddTexture(tppDiffuseTexture);
	//mesh1->AddTexture(tppNormalTexture);
meshes.push_back(mesh1);

	//Mesh* mesh2 = new Mesh(fbxBox, deferredShader);
	//mesh2->SetPosition(-150.0f, -90.0f, 0.0f);
	//mesh2->SetRotation(-90.0f, 180.0f, 0.0f);
	//mesh2->AddTexture(boxDiffuseTexture);
	////mesh2->AddTexture(boxNormalTexture);
	//meshes.push_back(mesh2);

	Mesh* mesh3 = new Mesh(fbxEnemyBear, deferredShader);
	mesh3->SetPosition(0.0f, -90.0f, 0.0f);
	mesh3->SetRotation(-90.0f, 180.0f, 0.0f);
	mesh3->AddTexture(enemyBearDiffuseTexture);
	//mesh3->AddTexture(enemyBearNormalTexture);
	meshes.push_back(mesh3);

	//Mesh* mesh4 = new Mesh(fbxWarrior, rimShader);
	//mesh4->SetPosition(150.0f, -90.0f, 0.0f);
	//mesh4->SetRotation(-90.0f, 180.0f, 0.0f);
	//mesh4->AddTexture(warriorDiffuseTexture);
	//mesh4->AddTexture(warriorNormalTexture);
	//meshes.push_back(mesh4);

	//Mesh* mesh5 = new Mesh(fbxBarbarous, normalMappingShader2);
	//mesh5->SetPosition(300.0f, -90.0f, 0.0f);
	//mesh5->SetRotation(-90.0f, 180.0f, 0.0f);
	//mesh5->AddTexture(barbarousDiffuseTexture);
	//mesh5->AddTexture(barbarousNormalTexture);
	//meshes.push_back(mesh5);
	
	// �޽� �ʱ�ȭ
	if (InitializeMeshes() == false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// �þ߰� / ��Ⱦ�� ����
	float fovY = XMConvertToRadians(60.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	// ī�޶� ��ü ���� 
	camera = new Camera(fovY, aspectRatio, 1.0f, 1000000.0f);
	
	// ���ۿ� ���� ����ü ���� ����
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectionMatrix());

	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	// ���� ����
	// ���� ������

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� �迭 ���� �־��� ����ü
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// ��� ���� ����
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("�� ��� ���� ���� ����")))
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
		// ��Ƽ���� ������
		if ((*mesh)->CompileShaders(device) == false)
			return false;
		

		// �� ���̴� ��ü ����
		if ((*mesh)->CreateShaders(device) == false)
			return false;

		// �ؽ�ó �ε�
		if ((*mesh)->LoadTextures(device) == false)
			return false;

		// ���÷� ������Ʈ ����
		if ((*mesh)->CreateSamplerState(device) == false)
			return false;

		// ���� �ʱ�ȭ
		if ((*mesh)->InitializeBuffers(device) == false)
			return false;

		// �����Ͷ����� ������Ʈ ����
		if ((*mesh)->CreateRasterizerState(device) == false)
			return false;
	}
	return true;
}
void Engine::RenderToTexture()
{
	// ���� Ÿ�� �ٲٱ�
	deferredBuffer->SetRenderTargets(deviceContext, depthStencilView);

	// ���� ĥ�ϱ�, ���� Ÿ�� �� �����
	float color[] = { 0.0f, 0.0f ,1.0f, 1.0f };
	deferredBuffer->ClearRenderTargets(deviceContext, depthStencilView, color);

	// �� �׸���
	RenderScene();

	// ���� Ÿ���� �ٽ� ������ ����(�����)�� ��������
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void Engine::RenderScene()
{
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		// ���� ��� ���ε�
		(*mesh)->Update(deviceContext);

		// ���̴� ���ε�
		(*mesh)->BindShaders(deviceContext);

		// �ؽ�ó / ���÷� ������Ʈ ���ε�
		(*mesh)->BindTextures(deviceContext);
		(*mesh)->BindSamplerState(deviceContext);

		// �����Ͷ����� ������Ʈ ���ε�
		(*mesh)->BindRasterizerState(deviceContext);

		// �޽� ���� �׸���
		(*mesh)->RenderBuffers(deviceContext);
	}
}

void Engine::BeginScene(float color[])
{
	// ���� Ÿ���� ������ �������� ĥ�ϱ�
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// ����/���ٽ� �� �����
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::UpdatePerspectiveCamera()
{
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectionMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
}
void Engine::UpdateOthographicCamera()
{
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(deferredRenderer->GetViewMatrix() * deferredRenderer->GetProjectionMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
}

// ���۵� ���� �ʱ�ȭ.
bool Engine::InitializeDeferredBuffers()
{
	deferredBuffer = new DeferredBuffer();
	if (deferredBuffer->Initialize(
		device,
		window->GetScreenWidth(),
		window->GetScreenHeight()) == false)
		return false;

	deferredMaterial = new Material(TEXT("Shader/DeferredDiffuse"));
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		Texture newTex;
		newTex.textureResourceView
			= deferredBuffer->GetShaderResourceView(ix);
		deferredMaterial->AddTexture(newTex);
	}

	// ������.
	if (deferredMaterial->CompileShaders(device) == false)
		return false;

	if (deferredMaterial->CreateShaders(device) == false)
		return false;

	if (deferredMaterial->CreateSamplerState(device) == false)
		return false;

	// ���۵� ������.
	deferredRenderer = new DeferredRenderer();
	if (deferredRenderer->Initialize(
		device,
		deferredMaterial->GetVertexShader()->GetShaderBuffer(),
		camera,
		window->GetScreenWidth(),
		window->GetScreenHeight()
	) == false)
		return false;

	return true;
}