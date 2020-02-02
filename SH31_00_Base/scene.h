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
	LAYER_CAMERA,		// �J�������C���[
	LAYER_BG,			// �w�i(BackGround)���C���[
	LAYER_3D,			// 3D���f�����C���[
	LAYER_EFFECTS,		// Effects���C���[
	LAYER_2DPOLYGONS,	// 2D�|���S�����C���[

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
	// ����������
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
	// �I������
	/////////////////////
	virtual void Uninit() {
		for (int i = 0; i < LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i])
			{
				// ���g�̔j��
				object->Uninit();
				delete object;
			}

			// ���ꕨ�̔j��
			m_GameObject[i].clear();
		}
	}


	/////////////////////
	// �X�V����
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
	// �`�揈��
	/////////////////////
	virtual void Draw() {
		// �z��Ǘ��pfor�\��
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

	// �z��p�̊Ǘ��e���v���[�g
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