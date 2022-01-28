/********************************************************************************* 

  *FileName: engine.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/21

**********************************************************************************/  


#ifndef ENGINE_H_
#define ENGINE_H_

#define DIRECTINPUT_VERSION 0x0800

#include <stdio.h>
#include <tchar.h>
#include <windowsx.h>
#include <iostream>

#include <d3dx9.h>
#include <dinput.h>
#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }
#define SAFE_RELEASE( p )      { if( p ) { ( p )->Release(); ( p ) = NULL; } }

#include "imgui.h"
#include "backends\imgui_impl_dx9.h"
#include "backends\imgui_impl_win32.h"

#include "input.h"
#include "allocatehierarchy.h"
#include "animation.h"
#include "animinstance.h"


// --------------------------------------------------
// ライブラリのリンク
// --------------------------------------------------
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要

//-----------------------------------------------------------------------------
// Engine Setup Structure
//-----------------------------------------------------------------------------
struct EngineSetup
{
	HINSTANCE	instance; 	    // Application instance handle.
	wchar_t*		class_name;		// ウインドウスタイルの名前
	wchar_t*		window_name;	// ウインドウの名前
	unsigned char totalBackBuffers; // Number of back buffers to use.
	float		  scale;            // Unit scale in meters/unit.
	int			screen_width;	// ウインドウの幅
	int			screen_height;	// ウインドウの高さ

	void (*StateSetup)();								  // State setup function.
	//void (*CreateMaterialResource)( Material **resource, char *name, char *path ); // Material resource creation function.	

	//-------------------------------------------------------------------------
	// The engine setup structure constructor.
	//-------------------------------------------------------------------------
	EngineSetup()
	{

		instance = NULL;
		class_name = L"Application";
		window_name = L"Game";
		totalBackBuffers = 1;
		scale = 1.0f;
		screen_width = 1280;
		screen_height = 720;
		StateSetup = NULL;
	}
};

//-----------------------------------------------------------------------------
// Engine Class
//-----------------------------------------------------------------------------
class Engine
{
private:
	static Engine* m_instance;
	char	          m_fpsText[16];  	// Frame rate character string.
	//Font*		  m_fpsFont;	  	// Font for rendering the frame rate.

	bool	          m_loaded;		// Indicates if the engine is loading.
	HWND		      m_window;		// Main window handle.
	bool	          m_deactive;    	// Indicates if the application is active or not.

	EngineSetup*      m_setup;		// Copy of the engine setup structure.
	IDirect3DDevice9* m_device;		// Direct3D device interface.
	D3DDISPLAYMODE	  m_displayMode; 	// Display mode of the current Direct3D device.
	unsigned char	  m_currentBackBuffer;  // Keeps track of which back buffer is at the front of the swap chain.	

	bool		   m_stateChanged; 	 // Indicates if the state changed in the current frame.

	Input* m_input;	      // Input object.

	CD3DXAnimation* m_animation;

public:
	Engine(EngineSetup *setup = NULL);
	virtual ~Engine();

	void	       Run();
	static Engine* GetInstance();

	HWND GetWindow();
	void SetDeactiveFlag(bool deactive);

	float		  GetScale();
	IDirect3DDevice9* GetDevice();
	D3DDISPLAYMODE*   GetDisplayMode();

	Input* GetInput();
};


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern Engine *g_engine;

#endif // ENGINE_H_