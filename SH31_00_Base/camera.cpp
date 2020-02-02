
#include "camera.h"

#define SPEED (0.05f)

void CCamera::Init()
{
	m_fCameraAngle = 0.0f;

	m_Position = XMFLOAT3(0.0f, 5.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

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
	// �ړ�
	if (CInput::GetKeyPress('A')) {		// ��
		m_Position.x -= SPEED;
	}
	if (CInput::GetKeyPress('D')) {		// �E
		m_Position.x += SPEED;
	}
	if (CInput::GetKeyPress('W')) {		// �O��
		m_Position.z += SPEED;
	}
	if (CInput::GetKeyPress('S')) {		// ���
		m_Position.z -= SPEED;
	}

	// ����
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

	// �㏸
	if (CInput::GetKeyPress(VK_LSHIFT)) {
		m_Position.y -= SPEED;
	}
	else if (CInput::GetKeyPress(VK_RSHIFT)) {
		m_Position.y += SPEED;
	}
}

void CCamera::Draw()
{
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// �r���[�}�g���N�X�ݒ�(�ړ��͂����œ���Ă�)
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);	// ����
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);			// �ړ�	// ���[�p�Ɋ|���Z���Ă�

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);	// �t�s��

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);


	{
		float x, y, z;
		x = m_Position.x;
		y = m_Position.y;
		z = m_Position.z;

		ImGui::Begin("Check Camera Pos Parents");
		ImGui::Text("X:%f", x);
		ImGui::Text("Y:%f", y);
		ImGui::Text("Z:%f", z);
		ImGui::End();
	}
}

// �ȒP�ȃJ�����O�A�X�N���[���J�����O
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