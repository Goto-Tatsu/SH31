#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader3D.h"
#include <io.h>

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "ball.h"

#include "scene.h"



void CShader3D::Init(const char* VertexShader, const char* PixelShader)
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
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

		hBufferDesc.ByteWidth = sizeof(CONSTANT3D);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}
}




void CShader3D::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();
}




void CShader3D::Set()
{
	/////////////////////////////////////////////////////////
	// �{�[���ɑ΂��Ē��ł͂����Ă邩��v����
	m_Camera = CManager::GetScene()->GetGameObject<CCamera>(LAYER_CAMERA);
	m_Ball = CManager::GetScene()->GetGameObject<CBall>(LAYER_3DMODELS);

	XMMATRIX world, view, proj, WVP;
	world = m_Ball->GetWorldMatrix();		// Tranpose�ς�
	view = m_Camera->GetViewMatrix();		// Tranpose���ĂȂ��I
	proj = m_Camera->GetProjectionMatrix();	// Tranpose���ĂȂ��I

	WVP = world * view * proj;
	
	XMFLOAT4X4 mtxWVP;
	DirectX::XMStoreFloat4x4(&mtxWVP, WVP);
	m_Constant3D.mtxWVP = mtxWVP;
	SetWorldViewProjectionMatrix(&mtxWVP);
	////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////
	// ���[���h�ϊ��s��t�s��
	XMMATRIX mtxWIT;
	mtxWIT = XMMatrixInverse(nullptr, world);	// �t�s��
	mtxWIT = XMMatrixTranspose(mtxWIT);			// �]�u
	XMFLOAT4X4 witf;
	DirectX::XMStoreFloat4x4(&witf, mtxWIT);
	m_Constant3D.mtxWIP = witf;
	SetWorldInverseTranspose(&witf);
	// ���̂��ƃV�F�[�_�[���W�X�^�[�ɃZ�b�g�X���B
	///////////////////////////////////////////////////////


	XMFLOAT4X4 mtxWorld;
	DirectX::XMStoreFloat4x4(&mtxWorld, world);
	m_Constant3D.mtxWorld = mtxWorld;

	m_Constant3D.cameraPos = XMFLOAT4(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z, 0.0f);

	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant3D, 0, 0);


	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_ConstantBuffer);
}