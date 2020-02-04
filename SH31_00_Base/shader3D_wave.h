#pragma once
#ifndef WAVE_H_
#define WAVE_H_

#include "main.h"

class EffectiveWaterSimulation
{
private:
	// �n���V�F�[�_�[�p�@�萔�o�b�t�@�\����
	typedef struct _CBUFFER_HS
	{
		float fTessFactor;
		float fInsideTessFactor;
		float Dummy2;
		float Dummy3;
	}CBUFFER_HS;

	// �h���C���V�F�[�_�[�p�@�萔�o�b�t�@�\����
	typedef struct _CBUFFER_DS
	{
		XMMATRIX matCameraWorldViewProj;
		// �g�̐����ʒu
		XMFLOAT4 vecWaveGeneratePosition[2];
		// 0:�U���A1:�g���A2:�p�x�A3�F�g�̃G�b�W
		XMFLOAT4 vecWaveParameters[2];
	}CBUFFER_DS;

	// �s�N�Z���V�F�[�_�[�p�@�萔�o�b�t�@�\����
	typedef struct _CBUFFER_PS
	{
		// ���s�����̈ʒu�x�N�g��
		XMFLOAT4 vecLightPos;
	}CBUFFER_PS;

public:
	typedef struct _WAVE
	{
		XMFLOAT4 vecWaveGeneratePosition;   // �g�̐������W
		float fAmplitude;                   // �U��
		float fWavelength;                  // �g��
		float fAngle;                       // �p�x
		float fAngularVelocity;             // �p���x
		float fWaveEdge;                    // �g�̃G�b�W
	}WAVE;

private:
	WAVE m_pWave[2];


	// �V�F�[�_�[�p�萔�o�b�t�@
	ID3D11Buffer* m_ConstantBuffers[3];
	_CBUFFER_HS* m_ConstantHS;
	_CBUFFER_DS* m_ConstantDS;
	_CBUFFER_PS* m_ConstantPS;

	// �V�F�[�_�[�ɕK�v�ȃ|�C���^�[
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11HullShader* m_HullShader;
	ID3D11DomainShader* m_DomainShader;

	int m_Pass;

	XMMATRIX m_MatCameraView, m_MatCameraProj;
	XMFLOAT4 m_VecLightPos;

	D3D11_FILL_MODE m_FillMode;

public:
	void Init(const char* VertexShader, 
		const char* PixelShader, 
		const char* HullShader, 
		const char* DomainShader);
	void Uninit();
	void Set();

	inline UINT GetMaxPass() { return 1; };
};

#endif // !WAVE_H_
