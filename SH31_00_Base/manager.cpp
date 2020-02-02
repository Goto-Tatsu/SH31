
#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "shader.h"
#include "scene.h"
#include "polygon.h"



CScene*	CManager::m_Scene;


void CManager::Init()
{

	CRenderer::Init();
	CInput::Init();

	m_Scene = new CScene();
	m_Scene->Init();

}

void CManager::Uninit()
{
	ImguiManager::Uninit();

	m_Scene->Uninit();
	delete m_Scene;

	CInput::Uninit();
	CRenderer::Uninit();

}

void CManager::Update()
{

	CInput::Update();

	m_Scene->Update();

	ImguiManager::Set();
}

void CManager::Draw()
{

	CRenderer::BeginDepth();
	// ここにライト方向からのレンダリング(Orthoでのレンダリング)
	// (W, LV, orthoProjection)
	m_Scene->Draw();

	CRenderer::Begin();
	// 上のレンダリングで使ったfarを使用してWVP
	m_Scene->Draw();

	ImguiManager::Draw();

	CRenderer::End();

}