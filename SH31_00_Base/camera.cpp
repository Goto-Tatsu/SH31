
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "game_object.h"

#include "camera.h"


void CCamera::Init()
{
	m_fCameraAngle = 0.0f;

	//m_DefaultForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	//m_DefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	//m_CamForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	//m_CamRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	m_Position = XMFLOAT3(0.0f, 5.0f, -5.0f);
	m_Rotation = XMFLOAT3(-5.0f, 0.0f, 0.0f);


	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

}


void CCamera::Uninit()
{

}


void CCamera::Update()
{
	//camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	//camAt = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	//
	//XMMATRIX RotateTempMatrix;
	//RotateTempMatrix = XMMatrixRotationY(camYaw);


	// 移動
	if (CInput::GetKeyPress('A')) {		// 左
		m_Position.x -= 0.5f;
	}
	if (CInput::GetKeyPress('D')) {		// 右
		m_Position.x += 0.5f;
	}
	if (CInput::GetKeyPress('W')) {		// 前方
		m_Position.z += 0.5f;
	}
	if (CInput::GetKeyPress('S')) {		// 後方
		m_Position.z -= 0.5f;
	}

	// 旋回
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_Rotation.y += 0.01f;
		//m_fCameraAngle += 0.01f;
	}
	else if (CInput::GetKeyPress(VK_LEFT)) {
		m_Rotation.y -= 0.01f;
		//m_fCameraAngle -= 0.01f;
	}
	else if (CInput::GetKeyPress(VK_UP)) {
		m_Rotation.x -= 0.01f;
		//m_fCameraAngle -= 0.01f;
	}
	else if (CInput::GetKeyPress(VK_DOWN)) {
		m_Rotation.x += 0.01f;
		//m_fCameraAngle -= 0.01f;
	}
	

	// 上昇
	if (CInput::GetKeyPress(VK_LSHIFT)) {
		m_Position.y -= 0.5f;
	}
	else if(CInput::GetKeyPress(VK_RSHIFT)) {
		m_Position.y += 0.5f;
	}



}



void CCamera::Draw()
{

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);



	// ビューマトリクス設定(移動はここで入れてる)
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);	// 旋回
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);			// 移動	// ヨー角に掛け算してる

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);	// 逆行列

	//CRenderer::SetViewMatrix(&m_ViewMatrix);	// ビュー行列を渡してあげる


	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	//CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);

}


// 簡単なカリング、スクリーンカリング
bool CCamera::GetVisibility(XMFLOAT3 Position)
{
	XMVECTOR worldPos, viewPos, projPos;
	XMFLOAT3 projPosF;
	worldPos = XMLoadFloat3(&Position);
	viewPos = XMVector3TransformCoord(worldPos, m_ViewMatrix);
	projPos = XMVector3TransformCoord(viewPos, m_ProjectionMatrix);
	XMStoreFloat3(&projPosF, projPos);

	if (-1.0f < projPosF.x && projPosF.x < 1.0f && -1.0f < projPosF.y && projPosF.y < 1.0f && 0 < projPosF.z && projPosF.z < 1.0f)
	{
		return true;
	}
	return false;

}