#pragma once

class CModel;
class CShader3D_Toon;

#define TEXTURE_MAX	(2)

class CBallToon :public CGameObject
{
private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

	CModel* m_pModel;
	XMVECTOR m_Quaternion;

	CTexture* m_pTexture[TEXTURE_MAX];
	CShader3D_Toon* m_pShader3D;
	CCamera* m_pCamera;
	XMMATRIX m_World;

public:
	CBallToon() {
		m_Position = {};
		m_Rotation = {};
		m_Scale = {};
	};
	~CBallToon() {};

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	XMMATRIX GetWorldMatrix() { return m_World; };
};