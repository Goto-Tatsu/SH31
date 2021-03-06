#ifndef MODEL_H_
#define MODEL_H_

class CShader3D;

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D* VertexArray;
	unsigned short	VertexNum;
	unsigned short* IndexArray;
	unsigned short	IndexNum;
	SUBSET* SubsetArray;
	unsigned short	SubsetNum;
};


class CModel
{
private:

	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMFLOAT3					m_Scale;

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	DX11_SUBSET* m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;

	CShader3D* m_Shader3D;

	void LoadObj(const char* FileName, MODEL* Model);
	void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned short* MaterialNum);

public:
	CModel() {};

	void Draw();
	void Load(const char* FileName);
	void Unload();

};

#endif // !MODEL_H_
