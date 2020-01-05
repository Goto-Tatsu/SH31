#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX					// Windows.hの前

#include <stdio.h>
#include <windows.h>
#include <assert.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

/* ====== IMGUI =================== */
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
/* ================================ */

/* ====== ASSIMP =================== */
#pragma comment(lib, "assimp.lib")
// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <assimp/matrix4x4.h>
/* ================================ */

/* ======== ORIGINAL ============= */
#include "imgui_manager.h"
/* =============================== */

#define SCREEN_WIDTH	(1280)			// ウインドウの幅
#define SCREEN_HEIGHT	(780)			// ウインドウの高さ


HWND GetWindow();
