#pragma once
#ifndef WAVE_H_
#define WAVE_H_

#include "main.h"

class EffectiveWaterSimulation
{
private:
	// ハルシェーダー用　定数バッファ構造体
	typedef struct _CBUFFER_HS
	{
		float fTessFactor;
		float fInsideTessFactor;
		float Dummy2;
		float Dummy3;
	}CBUFFER_HS;

	// ドメインシェーダー用　定数バッファ構造体
	typedef struct _CBUFFER_DS
	{
		XMMATRIX matCameraWorldViewProj;
		// 波の生成位置
		XMFLOAT4 vecWaveGeneratePosition[2];
		// 0:振幅、1:波長、2:角度、3：波のエッジ
		XMFLOAT4 vecWaveParameters[2];
	}CBUFFER_DS;

	// ピクセルシェーダー用　定数バッファ構造体
	typedef struct _CBUFFER_PS
	{
		// 平行光源の位置ベクトル
		XMFLOAT4 vecLightPos;
	}CBUFFER_PS;

public:
	typedef struct _WAVE
	{
		XMFLOAT4 vecWaveGeneratePosition;   // 波の生成座標
		float fAmplitude;                   // 振幅
		float fWavelength;                  // 波長
		float fAngle;                       // 角度
		float fAngularVelocity;             // 角速度
		float fWaveEdge;                    // 波のエッジ
	}WAVE;

private:
	WAVE m_pWave[2];


	// シェーダー用定数バッファ
	ID3D11Buffer* m_ConstantBuffers[3];
	_CBUFFER_HS* m_ConstantHS;
	_CBUFFER_DS* m_ConstantDS;
	_CBUFFER_PS* m_ConstantPS;

	// シェーダーに必要なポインター
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
