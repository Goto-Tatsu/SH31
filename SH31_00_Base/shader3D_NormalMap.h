#pragma once

class CCamera;	// View,Projectionをもらう
class CField;	// Worldをもらう

struct CONSTANT3D_NormalMap
{
	XMFLOAT4X4 mtxWVP;
	XMFLOAT4X4 mtxWIP;
	XMFLOAT4X4 mtxWorld;
	XMFLOAT4 cameraPos;
};

struct CONSTANT3D_NormalMap_WVP
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 camPos;
};

class CShader3D_NormalMap
{
private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11Buffer* m_ConstantBuffer;
	CONSTANT3D_NormalMap	m_Constant3DNormalMap;
	CONSTANT3D_NormalMap_WVP m_Constant3D_WVP;

	CCamera* m_pCamera;
	CField* m_pField;

public:
	void Init(const char* VertexShader, const char* PixelShader);
	void Uninit();
	void Set();

	void GetCameraPos(XMFLOAT3 position) { m_Constant3DNormalMap.cameraPos = XMFLOAT4(position.x, position.y, position.z, 1.0f); }

	//void SetProjectionMatrix(XMFLOAT4X4* ProjectionMatrix) { m_Constant3DNormalMap.ProjectionMatrix = Transpose(ProjectionMatrix); }
	void SetWorldTranspose(XMFLOAT4X4* mtxWorld) { m_Constant3DNormalMap.mtxWorld = Transpose(mtxWorld); }

	void SetWorldViewProjectionMatrix(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* projection) {
		m_Constant3D_WVP.world = Transpose(world);
		m_Constant3D_WVP.view = Transpose(view);
		m_Constant3D_WVP.projection = Transpose(projection);
	};

	void SetWorldViewProjectionMatrix(XMFLOAT4X4* mtxWVP) { m_Constant3DNormalMap.mtxWVP = Transpose(mtxWVP); };
	void SetWorldInverseTranspose(XMFLOAT4X4* mtxWIT) { m_Constant3DNormalMap.mtxWIP = Transpose(mtxWIT); };

	XMFLOAT4X4 Transpose(XMFLOAT4X4* Matrix)
	{
		XMFLOAT4X4 outMatrix;

		outMatrix._11 = Matrix->_11;
		outMatrix._12 = Matrix->_21;
		outMatrix._13 = Matrix->_31;
		outMatrix._14 = Matrix->_41;

		outMatrix._21 = Matrix->_12;
		outMatrix._22 = Matrix->_22;
		outMatrix._23 = Matrix->_32;
		outMatrix._24 = Matrix->_42;

		outMatrix._31 = Matrix->_13;
		outMatrix._32 = Matrix->_23;
		outMatrix._33 = Matrix->_33;
		outMatrix._34 = Matrix->_43;

		outMatrix._41 = Matrix->_14;
		outMatrix._42 = Matrix->_24;
		outMatrix._43 = Matrix->_34;
		outMatrix._44 = Matrix->_44;

		return outMatrix;
	}
};