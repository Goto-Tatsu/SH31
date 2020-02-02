#include <list>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "Toon.h"
#include "game_object.h"
#include "texture.h"
#include "camera.h"
#include "model.h"		// 前に"renderer.h"が必要
#include "ball_toon.h"

#include "scene.h"

#define TEXTURE_BASE	("data/TEXTURE/field004.tga")
#define TEXTURE_TOON	("data/TEXTURE/toon.tga")
#define SPEED	(0.05f)
#define ANGLE	(0.02f)

void CBallToon::Init(void)
{
	// シェーダーセット
	m_pShader3D = new CShader3D_Toon();
	m_pShader3D->Init("VS_ToonMap.cso", "PS_ToonMap.cso");

	m_pModel = new CModel();
	m_pModel->Load("data/MODEL/torus.obj");


	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_pTexture[i] = new CTexture();
	}

	m_pTexture[0]->Load(TEXTURE_BASE);
	m_pTexture[1]->Load(TEXTURE_TOON);

	m_Position = XMFLOAT3(0.0f, 3.0f, 10.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Quaternion = XMQuaternionIdentity();

	m_pCamera = CManager::GetScene()->GetGameObject<CCamera>(LAYER_CAMERA);

}

void CBallToon::Uninit(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++) {
		m_pTexture[i]->Unload();
		delete m_pTexture[i];
	}

	m_pModel->Unload();
	delete m_pModel;

	m_pShader3D->Uninit();
	delete m_pShader3D;
}

void CBallToon::Update()
{
	if (CInput::GetKeyPress('K'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.f, 1.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -ANGLE);

		// 回転を更新してるのがMultiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
	if (CInput::GetKeyPress('H'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.f, 1.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, ANGLE);

		// 回転を更新してるのがMultiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
	if (CInput::GetKeyPress('U'))
	{
		XMFLOAT3 vx = XMFLOAT3(1.f, 0.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vx);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, ANGLE);

		// 回転を更新してるのがMultiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
	if (CInput::GetKeyPress('J'))
	{
		XMFLOAT3 vx = XMFLOAT3(1.f, 0.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vx);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -ANGLE);

		// 回転を更新してるのがMultiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}


}

void CBallToon::Draw()
{
	// 更新内容を反映
	m_World = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_World *= XMMatrixRotationQuaternion(m_Quaternion);
	m_World *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&m_World);


	XMMATRIX world, view, proj, WVP;
	world = m_World;
	view = m_pCamera->GetViewMatrix();			// Tranposeしてない！
	proj = m_pCamera->GetProjectionMatrix();	// Tranposeしてない！
	WVP = m_World * view * proj;

	XMFLOAT4X4 mtxWVP;
	DirectX::XMStoreFloat4x4(&mtxWVP, WVP);
	m_pShader3D->SetWorldViewProjectionMatrix(&mtxWVP);

	//////////////////////////////////////////////////////
// ワールド変換行列逆行列
	XMMATRIX mtxWIT;
	mtxWIT = XMMatrixInverse(nullptr, m_World);	// 逆行列
	mtxWIT = XMMatrixTranspose(mtxWIT);			// 転置
	XMFLOAT4X4 witf;
	DirectX::XMStoreFloat4x4(&witf, mtxWIT);
	m_pShader3D->SetWorldInverseTranspose(&witf);
	// このあとシェーダーレジスターにセットスル。
	///////////////////////////////////////////////////////

	XMFLOAT4X4 mtxWorld;
	DirectX::XMStoreFloat4x4(&mtxWorld, world);
	m_pShader3D->SetWorldTranspose(&mtxWorld);
	m_pShader3D->GetCameraPos(m_pCamera->GetPosition());

	{
		float x, y, z;
		x = m_pCamera->GetPosition().x;
		y = m_pCamera->GetPosition().y;
		z = m_pCamera->GetPosition().z;

		ImGui::Begin("Check Camera Pos");
		ImGui::Text("X:%f", x);
		ImGui::Text("Y:%f", y);
		ImGui::Text("Z:%f", z);
		ImGui::End();
	}

	m_pShader3D->Set();
	// モデル描画
	CRenderer::SetTexture(0, m_pTexture[0]);
	CRenderer::SetTexture(1, m_pTexture[1]);
	m_pModel->Draw();
}