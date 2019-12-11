#ifndef FIELD_H_
#define FIELD_H_

#define TEXTURE_MAX	(2)
class CShader3D_NormalMap;

class CField : public CGameObject
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;
	

	static const int FIELD_X = 50;
	static const int FIELD_Z = 50;

	VERTEX_3D_NORMALMAP m_Vertex[FIELD_X * FIELD_Z];

	CCamera* m_pCamera;

	CTexture* m_pTexture[TEXTURE_MAX];
	CShader3D_NormalMap* m_pShader3D_Normalmap;
	
	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;



public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetWorldMatrix() { return m_World; };
	
};


#endif // !FIELD_H_
