#pragma once

// 拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。このワーニングを抑制する。
#pragma warning(disable: 4238)

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX					// Windows.hの前

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <memory>
#include <mmsystem.h>  // timeGetTime()

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")		 // timeGetTime()
#pragma comment (lib, "xaudio2.lib")

/* ====== IMGUI =================== */
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
/* ================================ */

/* ====== ASSIMP =================== */
#pragma comment(lib, "assimp.lib")
//#include <assimp/cimport.h>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <assimp/matrix4x4.h>
/* ================================ */

/* ======== ORIGINAL ============= */
#include "imgui_manager.h"
/* =============================== */

#define SCREEN_WIDTH	(1440)			// ウインドウの幅
#define SCREEN_HEIGHT	(810)			// ウインドウの高さ

// メモリリーク発生時にデバッガに出力する内容をわかりやすくする
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC                            // mallocによるメモリリーク検出でCPPファイル名と行数出力指定
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出でCPPファイル名と行数出力指定
#else
#define NEW  new
#endif

#define SAFE_RELEASE(x) if( x != nullptr ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != nullptr ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)  if( x != nullptr ){ delete[] x;  x = nullptr; }
#define SAFE_FREE(x)  if( x != nullptr ){ free( x );  x = nullptr; }

HWND GetWindow();
