#pragma once

#include <list>
#include "main.h"
#include "renderer.h"

class CGameObject
{
protected:

	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMFLOAT3					m_Scale;

public:

	XMFLOAT3 GetPosition() { return m_Position; }
	XMFLOAT3 GetRotation() { return m_Rotation; }
	XMFLOAT3 GetScale() { return m_Scale; }

	void SetPosition(XMFLOAT3 Position) { m_Position = Position; }
	void SetRotation(XMFLOAT3 Rotation) { m_Rotation = Rotation; }
	void SetScale(XMFLOAT3 Scale) { m_Scale = Scale; }

public:

	CGameObject() {}

	virtual ~CGameObject()
	{
	}

	virtual void Uninit()
	{
	}

	virtual void Update()
	{
	}

	virtual void Draw()
	{
	}
};

inline XMFLOAT4X4 MathMF(XMMATRIX& matrix) {
	XMFLOAT4X4 value;
	XMStoreFloat4x4(&value, matrix);
	return value;
}

inline XMMATRIX MathFM(XMFLOAT4X4& float4x4) {
	XMMATRIX matrix;
	matrix = XMLoadFloat4x4(&float4x4);
	return matrix;
}

inline XMVECTOR GMathFV(XMFLOAT3& val)
{
	return XMLoadFloat3(&val);
}

inline XMFLOAT3 GMathVF(XMVECTOR& vec)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, vec);
	return val;
}
