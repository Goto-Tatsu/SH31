#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader3D_NormalMap.h"
#include <io.h>

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "field.h"

#include "scene.h"

void CShader3D_NormalMap::Init(const char* VertexShader, const char* PixelShader)
{
	// ���_�V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// ���̓��C�A�E�g����
		// GPU(VRAM)�ɗ�������
		// ���_�f�[�^1���̍\��
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 9,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	4 * 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			UINT numElements = ARRAYSIZE(layout);

			CRenderer::GetDevice()->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);
		}

		delete[] buffer;
	}



	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}



	// �萔�o�b�t�@����
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT3D_NormalMap);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}
}




void CShader3D_NormalMap::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();
}




void CShader3D_NormalMap::Set()
{
	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant3DNormalMap, 0, 0);


	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_ConstantBuffer);
}

void CShader3D_NormalMap::Set(XMMATRIX object_world)
{
	/////////////////////////////////////////////////////////
	m_pCamera = CManager::GetScene()->GetGameObject<CCamera>(LAYER_CAMERA);

	XMMATRIX world, view, proj, WVP;
	world = object_world;			// Tranpose�ς�
	view = m_pCamera->GetViewMatrix();			// Tranpose���ĂȂ��I
	proj = m_pCamera->GetProjectionMatrix();	// Tranpose���ĂȂ��I

	WVP = world * view * proj;

	XMFLOAT4X4 mtxWVP;
	DirectX::XMStoreFloat4x4(&mtxWVP, WVP);
	m_Constant3DNormalMap.mtxWVP = mtxWVP;
	SetWorldViewProjectionMatrix(&mtxWVP);
	////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////
	// ���[���h�ϊ��s��t�s��
	XMMATRIX mtxWIT;
	mtxWIT = XMMatrixInverse(nullptr, world);	// �t�s��
	mtxWIT = XMMatrixTranspose(mtxWIT);			// �]�u
	XMFLOAT4X4 witf;
	DirectX::XMStoreFloat4x4(&witf, mtxWIT);
	m_Constant3DNormalMap.mtxWIP = witf;
	SetWorldInverseTranspose(&witf);
	// ���̂��ƃV�F�[�_�[���W�X�^�[�ɃZ�b�g�X���B
	///////////////////////////////////////////////////////


	XMFLOAT4X4 mtxWorld;
	DirectX::XMStoreFloat4x4(&mtxWorld, world);
	m_Constant3DNormalMap.mtxWorld = mtxWorld;

	m_Constant3DNormalMap.cameraPos = XMFLOAT4(m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z, 1.0f);

	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant3DNormalMap, 0, 0);


	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_ConstantBuffer);

}
