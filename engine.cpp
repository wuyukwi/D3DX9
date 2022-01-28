/********************************************************************************* 

  *FileName: engine.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/18

**********************************************************************************/  

#include "engine.h"

Engine * Engine::m_instance = NULL;

//-----------------------------------------------------------------------------
// Handles Windows messages.
//-----------------------------------------------------------------------------
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{

	case WM_ACTIVATEAPP:
		Engine::GetInstance()->SetDeactiveFlag(!wparam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}

//-----------------------------------------------------------------------------
// The engine class constructor.
//-----------------------------------------------------------------------------
Engine::Engine(EngineSetup *setup)
{
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	
	// Indicate that the engine is not yet loaded.
	m_loaded = false;

	// If no setup structure was passed in, then create a default one.
	// Otehrwise, make a copy of the passed in structure.
	m_setup = new EngineSetup;
	if (setup != NULL)
		memcpy(m_setup, setup, sizeof(EngineSetup));

	// Store a pointer to the engine in a global variable for easy access.
	m_instance = this;

	// ウインドウクラスの構造体
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);					// WNDCLASSEXのメモリサイズ
	wcex.style = CS_CLASSDC;							// ウインドウのスタイル
	wcex.lpfnWndProc = WindowProc;						// ウインドウプロシージャ
	wcex.cbClsExtra = 0;								// ０にする (通常は使用しない)
	wcex.cbWndExtra = 0;								// ０にする (通常は使用しない)
	wcex.hInstance = m_setup->instance;					// インスタンスハンドル
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// タスクバーのアイコン
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// マウスカーソル
	wcex.hbrBackground = NULL;							// クライアント領域の背景色
	wcex.lpszMenuName = NULL;							// メニューバー
	wcex.lpszClassName = L"WindowClass";				// ウインドウクラスの名前
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	// Initialise the COM using multithreaded concurrency.
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// Create the Direct3D interface.
	IDirect3D9 *d3d = Direct3DCreate9(D3D_SDK_VERSION);


	// ウインドウを作成
	m_window = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		m_setup->class_name,		// ウインドウスタイルの名前
		m_setup->window_name,		// ウインドウの名前
		WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// 　　〃　　の左上Y座標
		m_setup->screen_width,		// 　　〃　　の幅
		m_setup->screen_height,		// 　　〃　　の高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは子ウインドウID
		wcex.hInstance,				// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// Prepare the device presentation parameters.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	D3DDISPLAYMODE d3ddm;				// ディスプレイモード
	if (d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))
	{// 現在のディスプレイモードを取得
		MessageBox(0, L"GetAdapterDisplayMode() - FAILED", 0, 0);
	}

	//d3dpp.BackBufferWidth = m_setup->screen_width;
	//d3dpp.BackBufferHeight = m_setup->screen_height;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = m_setup->totalBackBuffers;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_window;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;

	// Create the Direct3D device with hardware vertex processing. 
	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_device)))
	{
		// Create the Direct3D device with mixed vertex processing. 
		if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_window, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_device)))
		{
			// Create the Direct3D device with software vertex processing. 
			if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_device)))
				return;
		}
	}

	// Setup Dear ImGui context--------------------------------------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_window);
	ImGui_ImplDX9_Init(m_device);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	//----------------------------------------------------------------------------------------------------------------------------------

	// Release the Direct3D interface as it is no longer needed.
	SAFE_RELEASE(d3d);

	// Switch lighting off by default.
	m_device->SetRenderState(D3DRS_LIGHTING, false);

	// Set the texture filters to use anisotropic texture filtering.
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Set the projection matrix.
	D3DXMATRIX projMatrix;
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 4, (float)d3dpp.BackBufferWidth / (float)d3dpp.BackBufferHeight, 0.1f / m_setup->scale, 1000.0f / m_setup->scale);
	m_device->SetTransform(D3DTS_PROJECTION, &projMatrix);

	// Store the display mode details.
	m_displayMode.Width = d3dpp.BackBufferWidth;
	m_displayMode.Height = d3dpp.BackBufferHeight;
	m_displayMode.RefreshRate = d3dpp.FullScreen_RefreshRateInHz;
	m_displayMode.Format = d3dpp.BackBufferFormat;

	// The swap chain always starts on the first back buffer.
	m_currentBackBuffer = 0;

	// Create the input object.
	m_input = new Input(m_window);

	// Seed the random number generator with the current time.
	srand(timeGetTime());

	// Allow the application to perform any state setup now.
	if (m_setup->StateSetup != NULL)
		m_setup->StateSetup();

	// The engine is fully loaded and ready to go.
	m_loaded = true;
}

//-----------------------------------------------------------------------------
// The engine class destructor.
//-----------------------------------------------------------------------------
Engine::~Engine()
{
	// Ensure the engine is loaded.
	if (m_loaded == true)
	{
		// Destroy everything.
		SAFE_DELETE(m_input);


		// Release the device.
		SAFE_RELEASE(m_device);
	}

	// Uninitialise the COM.
	CoUninitialize();

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Unregister the window class.
	UnregisterClass(L"WindowClass", m_setup->instance);

	// Destroy the engine setup structure.
	SAFE_DELETE(m_setup);
}

Engine* Engine::GetInstance()
{
	if (!m_instance)
		m_instance = new Engine();
	return m_instance;
}

//-----------------------------------------------------------------------------
// Enters the engine into the main processing loop.
//-----------------------------------------------------------------------------
void Engine::Run()
{
	// Ensure the engine is loaded.
	if (m_loaded == true)
	{
		// Show the window.
		ShowWindow(m_window, SW_NORMAL);
		UpdateWindow(m_window);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Enter the message loop.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (!m_deactive)
			{
				// Start the Dear ImGui frame
				ImGui_ImplDX9_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
				if (show_demo_window)
					ImGui::ShowDemoWindow(&show_demo_window);

				// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
				{
					static float f = 0.0f;
					static int counter = 0;

					ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

					ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
					ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
					ImGui::Checkbox("Another Window", &show_another_window);

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
					ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

					if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
						counter++;
					ImGui::SameLine();
					ImGui::Text("counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

				// 3. Show another simple window.
				if (show_another_window)
				{
					ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
					ImGui::Text("Hello from another window!");
					if (ImGui::Button("Close Me"))
						show_another_window = false;
					ImGui::End();
				}

				// Calculate the elapsed time.
				unsigned long currentTime = timeGetTime();
				static unsigned long lastTime = currentTime;
				float elapsed = (currentTime - lastTime) / 1000.0f;
				lastTime = currentTime;

				// Calculate the frame rate.
				static float frameTime = 1.0f;
				static int frameCount = 0;
				frameTime += elapsed;
				frameCount++;

				// Update the fps every half a second.
				if (frameTime > 1.0f)
				{
					sprintf(m_fpsText, "%d", frameCount);

					frameTime = 0.0f;
					frameCount = 0;
				}


				// Update the input object, reading the keyboard and mouse.
				m_input->Update();

				// Check if the user wants to make a forced exit.
				if (m_input->GetKeyPress(DIK_F1))
					PostQuitMessage(0);

				// Begin the scene.
				ImGui::EndFrame();
				m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

				D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));

				m_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET), clear_col_dx, 1.0f, 0);
				if (SUCCEEDED(m_device->BeginScene()))
				{
		
					ImGui::Render();
					ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
					// End the scene and present it.
					m_device->EndScene();
					m_device->Present(NULL, NULL, NULL, NULL);

					// Keep track of the index of the current back buffer.
					if (++m_currentBackBuffer == m_setup->totalBackBuffers + 1)
						m_currentBackBuffer = 0;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Returns the window handle.
//-----------------------------------------------------------------------------
HWND Engine::GetWindow()
{
	return m_window;
}

//-----------------------------------------------------------------------------
// Sets the deactive flag.
//-----------------------------------------------------------------------------
void Engine::SetDeactiveFlag(bool deactive)
{
	m_deactive = deactive;
}

//-----------------------------------------------------------------------------
// Returns the scale that the engine is currently running in.
//-----------------------------------------------------------------------------
float Engine::GetScale()
{
	return m_setup->scale;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the Direct3D device.
//-----------------------------------------------------------------------------
IDirect3DDevice9 *Engine::GetDevice()
{
	return m_device;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the display mode of the current Direct3D device.
//-----------------------------------------------------------------------------
D3DDISPLAYMODE *Engine::GetDisplayMode()
{
	return &m_displayMode;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the input object.
//-----------------------------------------------------------------------------
Input *Engine::GetInput()
{
	return m_input;
}