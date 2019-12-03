#include <list>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "shader3D.h"
#include "game_object.h"
#include "camera.h"
#include "model.h"		// �O��"renderer.h"���K�v
#include "ball.h"

#include "scene.h"

#define SPEED	(0.05f)
#define ANGLE	(0.02f)

void CBall::Init()
{
	// �V�F�[�_�[�Z�b�g
	m_pShader3D = new CShader3D();
	m_pShader3D->Init("vertexShader3D.cso", "pixelShader3D.cso");

	m_pModel = new CModel();
	m_pModel->Load("data/MODEL/miku_01.obj");

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);


	m_Quaternion = XMQuaternionIdentity();
}


void CBall::Uninit(void)
{
	m_pModel->Unload();
	delete m_pModel;

	m_pShader3D->Uninit();
	delete m_pShader3D;
}


void CBall::Update()
{
	if (CInput::GetKeyPress('K'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.f, 1.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -ANGLE);

		// ��]���X�V���Ă�̂�Multiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		//m_Position.x += SPEED;
	}
	if (CInput::GetKeyPress('H'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.f, 1.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, ANGLE);

		// ��]���X�V���Ă�̂�Multiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		//m_Position.x -= SPEED;
	}
	if (CInput::GetKeyPress('U'))
	{
		XMFLOAT3 vx = XMFLOAT3(1.f, 0.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vx);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, ANGLE);

		// ��]���X�V���Ă�̂�Multiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		//m_Position.z += SPEED;

	}
	if (CInput::GetKeyPress('J'))
	{
		XMFLOAT3 vx = XMFLOAT3(1.f, 0.f, 0.f);
		XMVECTOR axis = XMLoadFloat3(&vx);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -ANGLE);

		// ��]���X�V���Ă�̂�Multiply
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		//m_Position.z -= SPEED;
	}

	m_pShader3D->Set();

}


void CBall::Draw()
{
	// �X�V���e�𔽉f
	m_World = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_World *= XMMatrixRotationQuaternion(m_Quaternion);
	m_World *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&m_World);

	// ���f���`��
	m_pModel->Draw();


}