#ifndef IMGUI_MANAGER_
#define IMGUI_MANAGER_

class ImguiManager
{
private:
	static HWND m_hWnd;
	static bool m_show_demo_window;
	static bool m_show_another_window;

public:
	static void Init();
	static void Uninit();
	static void Draw();
	static void Set();
};


#endif // !IMGUI_MANAGER_
