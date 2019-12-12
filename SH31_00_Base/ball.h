#pragma once

class CModel;
class CShader3D;

class CBall :public CGameObject
{
private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

	CModel* m_pModel;
	XMVECTOR m_Quaternion;

	CShader3D* m_pShader3D;
	CCamera* m_pCamera;
	XMMATRIX m_World;

public:
	CBall() {
		m_Position = {};
		m_Rotation = {};
		m_Scale = {};
	};
	~CBall() {};

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	XMMATRIX GetWorldMatrix() { return m_World; };

};