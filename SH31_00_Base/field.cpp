#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "shader3D_NormalMap.h"
#include <io.h>

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "field.h"
#include "ball.h"

#include "scene.h"

#define FILENAME_BASECOLOR	("data/TEXTURE/field004.tga")
#define FILENAME_NORMALMAP	("data/TEXTURE/NormalMap.tga")
//#define	FILENAME_HEIGHTMAP	()
//#define FILENAME_METALIC	()

static float g_rot = 0.0f;

void CField::Init()
{
	m_pShader3D_Normalmap = new CShader3D_NormalMap();
	m_pShader3D_Normalmap->Init("vertexShader3D_NormalMap.cso", "pixelShader3D_NormalMap.cso");

	for (int z = 0; z < FIELD_Z; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			m_Vertex[z * FIELD_X + x].Position.x = x - FIELD_X / 2;
			m_Vertex[z * FIELD_X + x].Position.z = -z + FIELD_Z / 2;
			m_Vertex[z * FIELD_X + x].Position.y = 0.0f;
			//m_Vertex[z * FIELD_X + x].Position.y = sinf(x * 0.5f) * sinf(z * 0.3f) * 2.0f;
			m_Vertex[z * FIELD_X + x].Binormal = XMFLOAT3(0.0f, 0.0f, 1.0f);
			m_Vertex[z * FIELD_X + x].Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			m_Vertex[z * FIELD_X + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_Vertex[z * FIELD_X + x].TexCoord = XMFLOAT2(x, z);
		}
	}

	for (int z = 1; z < FIELD_Z - 1; z++)
	{
		for (int x = 1; x < FIELD_X - 1; x++)
		{
			XMFLOAT3 va, vb, n;
			float len;

			va.x = m_Vertex[(z - 1) * FIELD_X + x].Position.x - m_Vertex[(z + 1) * FIELD_X + x].Position.x;
			va.y = m_Vertex[(z - 1) * FIELD_X + x].Position.y - m_Vertex[(z + 1) * FIELD_X + x].Position.y;
			va.z = m_Vertex[(z - 1) * FIELD_X + x].Position.z - m_Vertex[(z + 1) * FIELD_X + x].Position.z;

			vb.x = m_Vertex[z * FIELD_X + (x + 1)].Position.x - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.x;
			vb.y = m_Vertex[z * FIELD_X + (x + 1)].Position.y - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.y;
			vb.z = m_Vertex[z * FIELD_X + (x + 1)].Position.z - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.z;

			n.x = va.y * vb.z - va.z * vb.y;
			n.y = va.z * vb.x - va.x * vb.z;
			n.z = va.x * vb.y - va.y * vb.x;

			len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

			n.x /= len;
			n.y /= len;
			n.z /= len;

			m_Vertex[z * FIELD_X + x].Normal = n;
		}
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D_NORMALMAP) * FIELD_X * FIELD_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	unsigned short index[(FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2];

	unsigned short i = 0;
	for (int z = 0; z < FIELD_Z - 1; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			index[i] = (z + 1) * FIELD_X + x;
			i++;
			index[i] = z * FIELD_X + x;
			i++;
		}

		if (z == FIELD_Z - 2)
			break;

		index[i] = z * FIELD_X + FIELD_X - 1;
		i++;
		index[i] = (z + 2) * FIELD_X;
		i++;
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * ((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_pTexture[i] = new CTexture();
	}
	m_pTexture[0]->Load(FILENAME_BASECOLOR);
	m_pTexture[1]->Load(FILENAME_NORMALMAP);
	//m_pTexture[2]->Load("data/TEXTURE/Rock_Displacement.tga");

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void CField::Uninit()
{
	m_VertexBuffer->Release();

	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_pTexture[i]->Unload();
		delete m_pTexture[i];
	}

	m_pShader3D_Normalmap->Uninit();
	delete m_pShader3D_Normalmap;
}

void CField::Update()
{
}

void CField::Draw()
{
	if (CInput::GetKeyTrigger('B'))
	{
		Uninit();
		InitB();
	}
	else if (CInput::GetKeyTrigger('I'))
	{
		Uninit();
		Init();
	}

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D_NORMALMAP);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// テクスチャ設定
	CTexture* pTexture[] = { m_pTexture[0] , m_pTexture[1] };
	//CRenderer::SetTexture(2, pTexture);
	CRenderer::SetTexture(0, m_pTexture[0]);
	CRenderer::SetTexture(1, m_pTexture[1]);

	// マトリクス設定
	m_World = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_World *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_World *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&m_World);

	{
		// 自身のワールド行列と、カメラのview,projection行列
		m_pCamera = CManager::GetScene()->GetGameObject<CCamera>(LAYER_CAMERA);
		XMMATRIX mtxWorld, mtxView, mtxProj, mtxWVP;
		mtxWorld = m_World;							// Tranpose済み
		mtxView = m_pCamera->GetViewMatrix();		// Tranposeしてない！
		mtxProj = m_pCamera->GetProjectionMatrix();	// Tranposeしてない！

		// WVPをそれぞれ渡してあげる、逆行列
		//XMFLOAT4X4 world, view, proj;
		//DirectX::XMStoreFloat4x4(&world, mtxWorld);
		//DirectX::XMStoreFloat4x4(&view, mtxWorld);
		//DirectX::XMStoreFloat4x4(&world, mtxWorld);
		//m_pShader3D_Normalmap->SetWorldViewProjectionMatrix(&world, &view, &proj);

		mtxWVP = mtxWorld * mtxView * mtxProj;
		XMFLOAT4X4 WVP;
		DirectX::XMStoreFloat4x4(&WVP, mtxWVP);
		m_pShader3D_Normalmap->SetWorldViewProjectionMatrix(&WVP);

		/////////////////////////////////////////////////
		// ワールド変換行列逆行列
		XMMATRIX mtxWIT;
		mtxWIT = XMMatrixInverse(nullptr, mtxWorld);	// 逆行列
		mtxWIT = XMMatrixTranspose(mtxWIT);				// 転置
		XMFLOAT4X4 witf;
		DirectX::XMStoreFloat4x4(&witf, mtxWIT);
		m_pShader3D_Normalmap->SetWorldInverseTranspose(&witf);
		// このあとシェーダーレジスターにセットスル。
		///////////////////////////////////////////////////////

		XMFLOAT4X4 World;
		DirectX::XMStoreFloat4x4(&World, mtxWorld);
		m_pShader3D_Normalmap->SetWorldTranspose(&World);
		m_pShader3D_Normalmap->GetCameraPos(m_pCamera->GetPosition());

		m_pShader3D_Normalmap->Set();
	}

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2), 0, 0);
}

void CField::InitB()
{
	m_pShader3D_Normalmap = new CShader3D_NormalMap();
	m_pShader3D_Normalmap->Init("vertexShader3D_NormalMap.cso", "pixelShader3D_NormalMap.cso");

	for (int z = 0; z < FIELD_Z; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			m_Vertex[z * FIELD_X + x].Position.x = x - FIELD_X / 2;
			m_Vertex[z * FIELD_X + x].Position.z = -z + FIELD_Z / 2;
			m_Vertex[z * FIELD_X + x].Position.y = 0.0f;
			//m_Vertex[z * FIELD_X + x].Position.y = sinf(x * 0.5f) * sinf(z * 0.3f) * 2.0f;
			m_Vertex[z * FIELD_X + x].Binormal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			m_Vertex[z * FIELD_X + x].Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			m_Vertex[z * FIELD_X + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_Vertex[z * FIELD_X + x].TexCoord = XMFLOAT2(x, z);
		}
	}

	for (int z = 1; z < FIELD_Z - 1; z++)
	{
		for (int x = 1; x < FIELD_X - 1; x++)
		{
			XMFLOAT3 va, vb, n;
			float len;

			va.x = m_Vertex[(z - 1) * FIELD_X + x].Position.x - m_Vertex[(z + 1) * FIELD_X + x].Position.x;
			va.y = m_Vertex[(z - 1) * FIELD_X + x].Position.y - m_Vertex[(z + 1) * FIELD_X + x].Position.y;
			va.z = m_Vertex[(z - 1) * FIELD_X + x].Position.z - m_Vertex[(z + 1) * FIELD_X + x].Position.z;

			vb.x = m_Vertex[z * FIELD_X + (x + 1)].Position.x - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.x;
			vb.y = m_Vertex[z * FIELD_X + (x + 1)].Position.y - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.y;
			vb.z = m_Vertex[z * FIELD_X + (x + 1)].Position.z - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.z;

			n.x = va.y * vb.z - va.z * vb.y;
			n.y = va.z * vb.x - va.x * vb.z;
			n.z = va.x * vb.y - va.y * vb.x;

			len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

			n.x /= len;
			n.y /= len;
			n.z /= len;

			m_Vertex[z * FIELD_X + x].Normal = n;
		}
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D_NORMALMAP) * FIELD_X * FIELD_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	unsigned short index[(FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2];

	unsigned short i = 0;
	for (int z = 0; z < FIELD_Z - 1; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			index[i] = (z + 1) * FIELD_X + x;
			i++;
			index[i] = z * FIELD_X + x;
			i++;
		}

		if (z == FIELD_Z - 2)
			break;

		index[i] = z * FIELD_X + FIELD_X - 1;
		i++;
		index[i] = (z + 2) * FIELD_X;
		i++;
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * ((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_pTexture[i] = new CTexture();
	}
	m_pTexture[0]->Load(FILENAME_BASECOLOR);
	m_pTexture[1]->Load(FILENAME_NORMALMAP);
	//m_pTexture[2]->Load("data/TEXTURE/Rock_Displacement.tga");

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}