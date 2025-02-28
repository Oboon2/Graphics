#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(inputLayout);
}

bool Mesh::InitializeBuffers(ID3D11Device * device, ID3DBlob * vertexShaderBuffer)
{
	// 정점 배열
	Vertex vertices[] = {
		Vertex(XMFLOAT3(0.0f, 0.5f, 0.5f),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(XMFLOAT3(0.5f, -0.5f, 0.5f),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(XMFLOAT3(-0.5f, -0.5f, 0.5f),XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
	};

	// 배열 크기 저장
	nVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 정점 배열 정보 넣어줄 구조체
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	//정점 버퍼 생성
	HRESULT result = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (IsError(result, TEXT("정점 버퍼 생성 실패")))
	{
		return false;
	}

	// 입력 레이아웃 서술자 생성
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // POSITION의 크기(12)만큼 이동해서 시작

	};

	// 입력 레이아웃 생성
	result = device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);
	if (IsError(result, TEXT("입력 레이아웃 생성 실패")))
	{
		return false;
	}

	return true;
}

void Mesh::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 정점 버퍼 전달
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// 입력 레이아웃 전달
	deviceContext->IASetInputLayout(inputLayout);

	// 위상 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 그리기
	deviceContext->Draw(nVertices, 0);
}
