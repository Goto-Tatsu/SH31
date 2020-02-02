#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.h"
#include "imgui_manager.h"
#include "renderer.h"
#include "input.h"
#include "game_object.h"


class CCamera : public CGameObject	// CGameObject����p��
{
private:
	RECT						m_Viewport;	// 
	float						m_fCameraAngle;// �J�����A���O��

	XMMATRIX	m_ViewMatrix;			// �r���[���W�s��
	XMMATRIX	m_InvViewMatrix;		// �r���[�t�s��ϊ��p�i�[�s��
	XMMATRIX	m_ProjectionMatrix;		// �v���W�F�N�V�����s��

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix() { return m_ViewMatrix; };
	XMMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; };
	
	bool GetVisibility(XMFLOAT3 Position);
};

#endif // !CAMERA_H_