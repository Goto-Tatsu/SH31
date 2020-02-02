#pragma once

// �g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B���̃��[�j���O��}������B
#pragma warning(disable: 4238)

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX					// Windows.h�̑O

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

#define SCREEN_WIDTH	(1440)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(810)			// �E�C���h�E�̍���

// ���������[�N�������Ƀf�o�b�K�ɏo�͂�����e���킩��₷������
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC                            // malloc�ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new �ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
#else
#define NEW  new
#endif

#define SAFE_RELEASE(x) if( x != nullptr ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != nullptr ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)  if( x != nullptr ){ delete[] x;  x = nullptr; }
#define SAFE_FREE(x)  if( x != nullptr ){ free( x );  x = nullptr; }

HWND GetWindow();
