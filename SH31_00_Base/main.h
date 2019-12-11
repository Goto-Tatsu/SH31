#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX					// Windows.h�̑O

#include <stdio.h>
#include <windows.h>
#include <assert.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

/* ====== ASSIMP =================== */
#pragma comment(lib, "assimp.lib")
// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <assimp/matrix4x4.h>
/* ================================ */

#define SCREEN_WIDTH	(1920)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(1080)			// �E�C���h�E�̍���


HWND GetWindow();
