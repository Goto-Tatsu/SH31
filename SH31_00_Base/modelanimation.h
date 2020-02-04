#pragma once

#include <map>

enum PLAYER_FBX_tag
{
	PLAYER_WALK,
	PLAYER_ATTACK,

	PLAYER_MAX
};

struct MESH
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	unsigned int IndexNum;
};

struct MESHANIM
{
	std::map<std::string, aiQuaternion>* NodeRotation;
	std::map<std::string, aiVector3D>* NodePostion;
};

class CModelAnimation
{
private:
	MESH* m_Mesh;			// MESH構造体
	unsigned int m_MeshNum;	// Meshの数

	MESHANIM* m_MeshAnim;	// Animation用の要素保存場所構造体
	std::map<std::string, aiQuaternion> m_NodeRotation;
	std::map<std::string, aiVector3D> m_NodePostion;

	const aiScene* m_Scene[PLAYER_MAX];	// シーンをメンバに

	/* 前のやつ */
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	unsigned int m_IndexNum;

public:
	void Update(int Animation, int Frame);
	void Draw(int Animatioin, XMMATRIX Matrix);
	void DrawMesh(aiNode* Node, XMMATRIX Matrix);
	void Load();
	void Unload();

};