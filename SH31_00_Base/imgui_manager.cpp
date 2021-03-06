#include "main.h"
#include "renderer.h"

// 静的メンバ変数初期化
bool ImguiManager::m_show_demo_window = true;
bool ImguiManager::m_show_another_window = true;

void ImguiManager::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// Setup Dear ImGui context //////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	IMGUI_CHECKVERSION();																		//
	ImGui::CreateContext();																		//
	ImGuiIO& io = ImGui::GetIO(); (void)io;														//
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls		//
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls		//
																								//
	// Setup Dear ImGui style																	//
	//ImGui::StyleColorsDark();																	//
	ImGui::StyleColorsClassic();																//
																								//
	// Setup Platform/Renderer bindings															//
	ImGui_ImplWin32_Init(GetWindow());															//
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());					//
	//////////////////////////////////////////////////////////////////////////////////////////////
}

void ImguiManager::Uninit()
{
	// ImGui Cleanup 
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::Draw()
{
	// IMGUIの描画(Render)
	ImGui::Render();										// 設定したIMGUIの諸々を描画してくれる命令。
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());	// 何かよくわかんない
}

void ImguiManager::Set()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (m_show_demo_window)
		ImGui::ShowDemoWindow(&m_show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &m_show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (m_show_another_window)
	{
		int mouseX, mouseY;
		ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Player position show");

		if (ImGui::Button("Close Me"))
			m_show_another_window = false;
		ImGui::End();
	}
	/* ================================================================================================ */

}
