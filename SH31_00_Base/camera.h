#ifndef CAMERA_H_
#define CAMERA_H_


class CCamera : public CGameObject	// CGameObjectから継承
{
private:

	XMVECTOR camPosition;
	XMVECTOR camAt;
	XMVECTOR camUp;

	RECT						m_Viewport;	// 
	float						m_fCameraAngle;// カメラアングル

	XMMATRIX	m_ViewMatrix;			// ビュー座標行列
	XMMATRIX	m_InvViewMatrix;		// ビュー逆行列変換用格納行列
	XMMATRIX	m_ProjectionMatrix;		// プロジェクション行列

	XMMATRIX camRotationMatrix;
	XMMATRIX groundWorld;

	float moveLeftRight;
	float moveBackForward;

	float camYaw;
	float camPitch;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix() { return m_ViewMatrix; };
	XMMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; };
	

	XMFLOAT3					m_Position;	// カメラの位置(座標)
	XMFLOAT3					m_Rotation;	// カメラの回転要素
	XMFLOAT3					m_Target;	// 注視点

	bool GetVisibility(XMFLOAT3 Position);
};

#endif // !CAMERA_H_