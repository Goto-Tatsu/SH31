#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader3D_wave.h"
#include <io.h>

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "wave.h"

#include "scene.h"


void EffectiveWaterSimulation::Init(const char* VertexShader, const char* PixelShader, const char* HullShader, const char* DomainShader)
{

	for (int i = 0; i < _countof(m_ConstantBuffers); i++)
	{
		m_ConstantBuffers[i] = nullptr;
	}

	m_FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	m_Pass = -2;


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
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }, };

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

	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(HullShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}

	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(DomainShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}

	// �萔�o�b�t�@����
	{
		m_ConstantBuffers[0] = CRenderer::CreateConstantBuffer(CRenderer::GetDevice(), nullptr, sizeof(EffectiveWaterSimulation::CBUFFER_HS), D3D11_CPU_ACCESS_WRITE);
		m_ConstantBuffers[1] = CRenderer::CreateConstantBuffer(CRenderer::GetDevice(), nullptr, sizeof(EffectiveWaterSimulation::CBUFFER_DS), D3D11_CPU_ACCESS_WRITE);
		m_ConstantBuffers[2] = CRenderer::CreateConstantBuffer(CRenderer::GetDevice(), nullptr, sizeof(EffectiveWaterSimulation::CBUFFER_PS), D3D11_CPU_ACCESS_WRITE);
	}
}

void EffectiveWaterSimulation::Uninit()
{
	for (int i = 0; i < _countof(m_ConstantBuffers); i++)
	{
		SAFE_RELEASE(m_ConstantBuffers[i]);
	}

	if (m_VertexLayout)
		m_VertexLayout->Release();

	if (m_VertexShader)
		m_VertexShader->Release();

	if (m_PixelShader)
		m_PixelShader->Release();

	if (m_HullShader)
		SAFE_RELEASE(m_HullShader);

	if (m_DomainShader)
		SAFE_RELEASE(m_DomainShader);
}

void EffectiveWaterSimulation::Set()
{
	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	CRenderer::GetDeviceContext()->HSSetShader(m_HullShader, NULL, 0);

	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffers[0], 0, NULL, &m_ConstantHS, 0, 0);
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffers[1], 0, NULL, &m_ConstantDS, 0, 0);
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffers[2], 0, NULL, &m_ConstantPS, 0, 0);

	// �萔�o�b�t�@�ݒ�
	//CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	CRenderer::GetDeviceContext()->HSSetConstantBuffers(0, 1, &m_ConstantBuffers[0]);
	CRenderer::GetDeviceContext()->DSSetConstantBuffers(1, 1, &m_ConstantBuffers[1]);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(2, 1, &m_ConstantBuffers[2]);
}
