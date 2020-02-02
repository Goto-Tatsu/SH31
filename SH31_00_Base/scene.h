#pragma once

#include <typeinfo>
#include <list>
#include "main.h"

#include "game_object.h"

#include "polygon.h"
#include "camera.h"
#include "field.h"
#include "ball.h"
#include "ball_toon.h"

enum LAYER
{
	LAYER_CAMERA,		// カメラレイヤー
	LAYER_BG,			// 背景(BackGround)レイヤー
	LAYER_3D,			// 3Dモデルレイヤー
	LAYER_EFFECTS,		// Effectsレイヤー
	LAYER_2DPOLYGONS,	// 2Dポリゴンレイヤー

	LAYER_MAX
};

class CScene
{
protected:
	std::list<CGameObject*>	m_GameObject[LAYER_MAX];

public:
	CScene(){}
	virtual ~CScene(){}

	/////////////////////
	// 初期化処理
	/////////////////////
	virtual void Init()
	{
		// Camera
		AddGameObject<CCamera>(LAYER_CAMERA);

		// 3DObjects
		AddGameObject<CField>(LAYER_BG);
		AddGameObject<CBall>(LAYER_3D);
		//AddGameObject<CBallToon>(LAYER_3D);

		// Polygon
		AddGameObject<CPolygon>(LAYER_2DPOLYGONS);
	}

	/////////////////////
	// 終了処理
	/////////////////////
	virtual void Uninit() {
		for (int i = 0; i < LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i])
			{
				// 中身の破棄
				object->Uninit();
				delete object;
			}

			// 入れ物の破棄
			m_GameObject[i].clear();
		}
	}


	/////////////////////
	// 更新処理
	/////////////////////
	virtual void Update() {
		for (int i = 0; i < LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}

			//m_GameObject[i].remove_if([](CGameObject* object) { return object->Destroy(); });
		}
	}


	/////////////////////
	// 描画処理
	/////////////////////
	virtual void Draw() {
		// 配列管理用for構文
		for (int i = 0; i < LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i]) {
				object->Draw();
			}
		}
	}


	template <typename T>
	T* AddGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject.push_back( gameObject );

		return gameObject;
	}

	// 配列用の管理テンプレート
	template<typename ClassName>ClassName* AddGameObject(int Layer) {
		ClassName* object = new ClassName();
		object->Init();
		m_GameObject[Layer].push_back(object);
		return object;
	}

	template<typename ClassName>ClassName* GetGameObject(int Layer) {
		for (CGameObject* object : m_GameObject[Layer]) {
			if (typeid(*object) == typeid(ClassName)) {
				return (ClassName*)object;
			}
		}
	}

};