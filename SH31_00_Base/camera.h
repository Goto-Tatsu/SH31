#ifndef CAMERA_H_
#define CAMERA_H_

#include "main.h"
#include "imgui_manager.h"
#include "renderer.h"
#include "input.h"
#include "game_object.h"


class CCamera : public CGameObject	// CGameObjectから継承
{
private:
	RECT						m_Viewport;	// 
	float						m_fCameraAngle;// カメラアングル

	XMMATRIX	m_ViewMatrix;			// ビュー座標行列
	XMMATRIX	m_InvViewMatrix;		// ビュー逆行列変換用格納行列
	XMMATRIX	m_ProjectionMatrix;		// プロジェクション行列

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