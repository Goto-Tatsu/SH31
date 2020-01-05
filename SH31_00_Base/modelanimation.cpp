/*==============================================================================


														 Author : Tatsuki Goto
														 Date   : 2019/09/03
--------------------------------------------------------------------------------
==============================================================================*/

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "game_object.h"
#include "texture.h"
#include "modelanimation.h"


void CModelAnimation::Update(int Animation, int Frame)
{
	aiAnimation* animation = m_Scene[Animation]->mAnimations[0];	// 何番目のアニメーション呼ぶ？

	// 回転の内容
	for (auto c = 0; c < animation->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[c];
		int f = Frame % nodeAnim->mNumRotationKeys;
		m_NodeRotation[nodeAnim->mNodeName.C_Str()] = nodeAnim->mRotationKeys[Frame].mValue;
	}

	// 移動の内容
	for (auto c = 0; c < animation->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[c];
		int f = Frame % nodeAnim->mNumPositionKeys;
		m_NodePostion[nodeAnim->mNodeName.C_Str()] = nodeAnim->mPositionKeys[Frame].mValue;
	}

	// 拡縮の内容
}

void CModelAnimation::Draw(int Animation, XMMATRIX Matrix)	// 引数のMatrixはPlayerなどの行列を受け取る
{
	// 階層構造による再帰関数呼び出し
	DrawMesh(m_Scene[Animation]->mRootNode, Matrix);
}

void CModelAnimation::DrawMesh(aiNode* Node, XMMATRIX Matrix)
{
	/* ※行列計算の順番は【Rotation＊Translate＊Scale】 *

	/* Rotation */
	/* RotAnimation */
	aiQuaternion aiQuat = m_NodeRotation[Node->mName.C_Str()];
	XMVECTOR quat = XMLoadFloat4(&XMFLOAT4(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w));
	XMMATRIX world = XMMatrixRotationQuaternion(quat);

	/* Translate */
	aiVector3D aiVec = m_NodePostion[Node->mName.C_Str()];
	XMVECTOR vec = XMLoadFloat3(&XMFLOAT3(aiVec.x, aiVec.y, aiVec.z));
	world *= XMMatrixTranslationFromVector(vec);

	world *= Matrix;					// 親の行列(Matrix)、親から見た新しい行列に変換される。
	CRenderer::SetWorldMatrix(&world);	// 親から見た位置にセット

	for (int n = 0; n < Node->mNumMeshes; n++)
	{
		unsigned int m = Node->mMeshes[n];	// mはメッシュ番号

		/* メッシュ描画(前回のDraw(void)関数の中身) */
		CRenderer::SetVertexBuffers(m_Mesh[m].VertexBuffer);
		CRenderer::SetIndexBuffer(m_Mesh[m].IndexBuffer);
		CRenderer::DrawIndexed(m_Mesh[m].IndexNum, 0, 0);
		/* ======================================== */
	}

	for (int i = 0; i < Node->mNumChildren; i++)
	{
		DrawMesh(Node->mChildren[i], world);
	}

}

void CModelAnimation::Load()
{
	unsigned int texture;

	// Assimpの構造体を用いて
	m_Scene[PLAYER_WALK] = aiImportFile("asset/model/Player/machine_Walk.fbx", aiProcess_Triangulate);
	m_Scene[PLAYER_ATTACK] = aiImportFile("asset/model/Player/machine_Attack.fbx", aiProcess_Triangulate);

	m_MeshNum = m_Scene[PLAYER_WALK]->mNumMeshes;
	m_Mesh = new MESH[m_MeshNum];

	for (auto i = 0; i < PLAYER_MAX; i++) {
		for (int m = 0; m < m_MeshNum; m++) {
			aiMesh* mesh = m_Scene[i]->mMeshes[m];

			// 頂点バッファの設定
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];	// 頂点を保存

			for (int i = 0; i < mesh->mNumVertices; i++) {
				vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex[i].Diffuse = XMFLOAT4(1, 1, 1, 1);
				vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}

			// 頂点バッファの生成
			{
				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				//bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = vertex;

				// バッファを作るための汎用的な関数
				CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].VertexBuffer);
			}
			delete[] vertex;


			// Indexばっふぁせってい
			unsigned short* index;
			index = new unsigned short[mesh->mNumFaces * 3];
			m_Mesh[m].IndexNum = mesh->mNumFaces * 3;				// 最大頂点数を算出
			for (int i = 0; i < mesh->mNumFaces; i++) {				// 面の数だけforループ
				index[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
				index[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				index[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			}
			// Indexばっふぁ生成
			{	// 小さなスコープで限定する
				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(unsigned short) * mesh->mNumFaces * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				//bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = index;

				// バッファを作るための汎用的な関数
				CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].IndexBuffer);
			}
			delete[] index;



		}
	}
}

void CModelAnimation::Unload()
{
	for (auto m = 0; m < m_MeshNum; m++)
	{
		m_Mesh[m].VertexBuffer->Release();
		m_Mesh[m].IndexBuffer->Release();
	}

	for (auto i = 0; i < PLAYER_MAX; i++) {
		aiReleaseImport(m_Scene[i]);
	}
}
