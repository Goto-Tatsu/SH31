#include <math.h>
#include "main.h"
#include "renderer.h"
#include "shader.h"

#include "game_object.h"
#include "polygon.h"
#include "texture.h"

// 頂点構造体
struct VERTEX_2D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
	XMFLOAT2 TexCoord;
};

void CPolygon::Init()
{
	VERTEX_2D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Color = XMFLOAT4(0.2f, 0.8f, 0.3f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(100.0f, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Color = XMFLOAT4(0.4f, 0.6f, 0.5f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, 200.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);;
	vertex[2].Color = XMFLOAT4(0.6f, 0.4f, 0.7f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(100.0f, 200.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Color = XMFLOAT4(0.8f, 0.2f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_2D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Shader = new CShader();
	m_Shader->Init("vertexShader2D.cso", "pixelShader2D.cso");

	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/cocoon.tga");
}

void CPolygon::Uninit()
{
	m_Shader->Uninit();
	delete m_Shader;

	m_Texture->Unload();
	delete m_Texture;

	m_VertexBuffer->Release();
}

void CPolygon::Update()
{
	//m_Shader->SetValue((sin(m_Value) + 1.0f) * 0.5f);
	//m_Value += 0.02f;
}

void CPolygon::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	XMFLOAT4X4 identity;
	DirectX::XMStoreFloat4x4(&identity, XMMatrixIdentity());

	XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&projection, XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
	m_Shader->SetProjectionMatrix(&projection);

	m_Shader->Set();

	//-- テクスチャの設定 --//
	CRenderer::SetTexture(m_Texture);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);
}