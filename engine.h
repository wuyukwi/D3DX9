/********************************************************************************* 

  *FileName: engine.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/21

**********************************************************************************/  


#ifndef ENGINE_H_
#define ENGINE_H_

#define DIRECTINPUT_VERSION 0x0800
#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }
#define SAFE_RELEASE( p )      { if( p ) { ( p )->Release(); ( p ) = NULL; } }

#include <stdio.h>
#include <tchar.h>
#include <windowsx.h>
#include <iostream>

#include <d3dx9.h>
#include <dinput.h>

#include "imgui.h"
#include "backends\imgui_impl_dx9.h"
#include "backends\imgui_impl_win32.h"

#include "camera.h"
#include "LinkedList.h"
#include "ResourceManagement.h"
#include "Geometry.h"
#include "Font.h"
#include "Scripting.h"
#include "Input.h"
#include "BoundingVolume.h"
#include "Material.h"
#include "Mesh.h"
#include "SceneObject.h"
#include "AnimatedObject.h"
#include "SpawnerObject.h"
#include "ViewFrustum.h"
#include "RenderCache.h"
#include "SceneManager.h"
#include "CollisionDetection.h"
#include "State.h"

// --------------------------------------------------
// ライブラリのリンク
// --------------------------------------------------
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要

////-----------------------------------------------------------------------------
//// Engine Setup Structure
////-----------------------------------------------------------------------------
//struct EngineSetup
//{
//	HINSTANCE	instance; 	    // Application instance handle.
//	wchar_t*		class_name;		// ウインドウスタイルの名前
//	wchar_t*		window_name;	// ウインドウの名前
//	unsigned char totalBackBuffers; // Number of back buffers to use.
//	float		  scale;            // Unit scale in meters/unit.
//	int			screen_width;	// ウインドウの幅
//	int			screen_height;	// ウインドウの高さ
//
//	void (*StateSetup)();								  // State setup function.
//	//void (*CreateMaterialResource)( Material **resource, char *name, char *path ); // Material resource creation function.	
//
//	//-------------------------------------------------------------------------
//	// The engine setup structure constructor.
//	//-------------------------------------------------------------------------
//	EngineSetup()
//	{
//
//		instance = NULL;
//		class_name = L"Application";
//		window_name = L"Game";
//		totalBackBuffers = 1;
//		scale = 1.0f;
//		screen_width = 1280;
//		screen_height = 720;
//		StateSetup = NULL;
//	}
//};

//-----------------------------------------------------------------------------
// Engine Setup 構造体
//-----------------------------------------------------------------------------
struct EngineSetup
{
	HINSTANCE	  instance; 	    // アプリケーションインスタンスハンドル。
	char* name;				// アプリケーションインネーム
	float		  scale;            // 単位スケール。(メートル/単位)
	unsigned char 	  totalBackBuffers; // 使用するバックバッファの数。
	char* spawnerPath;	    // スクリプトオブジェクトのパス。
	int			screen_width;	// ウインドウの幅
	int			screen_height;	// ウインドウの高さ

	void (*StateSetup)();								  // ステートセットアップ関数
	//void (*CreateMaterialResource)(Material** resource, char* name, char* path); // マテリアルソース作成関数

	//-------------------------------------------------------------------------
	// EngineSetup構造コンストラクター。
	//-------------------------------------------------------------------------
	EngineSetup()
	{
		instance = NULL;
		name = "Application";
		scale = 1.0f;
		totalBackBuffers = 1;
		StateSetup = NULL;
		spawnerPath = "./";
		screen_width = 1280;
		screen_height = 720;

	}
};

//-----------------------------------------------------------------------------
// Engine クラス
//-----------------------------------------------------------------------------
class Engine
{
private:
	static Engine* m_instance;
	char	          m_fpsText[16];  	// フレームレート格納の文字列。
	Font* m_fpsFont;	  	// フレームレートを描画するためのフォント。

	bool	          m_loaded;		// エンジンがロードされているかどうか
	HWND		  m_window;		// メインウィンドウのハンドル。
	bool	          m_deactive;    	// アプリケーションがアクティブかどうか

	EngineSetup* m_setup;		// エンジンセットアップ構造のコピー。
	IDirect3DDevice9* m_device;		// Direct3Dデバイスインターフェイス。
	D3DDISPLAYMODE	  m_displayMode; 	// 現在のDirect3Dデバイスの表示モード。
	ID3DXSprite* m_sprite;		// 2Dテクスチャプリミティブを描画するためのスプライトインターフェイス。
	unsigned char	  m_currentBackBuffer;  // どのバックバッファがスワップチェーンの先頭にあるかを追跡します。

	LinkedList<State>* m_states;		 // ステートのリンクリスト
	State* m_currentState; 	 // 現在のステートへのポインタ。
	bool		   m_stateChanged; 	 // 現在のフレームでステートが変化したかどうか

	ResourceManager<Script>* m_scriptManager;   // Script manager.
	ResourceManager<Material>* m_materialManager; // Material manager.
	ResourceManager<Mesh>* m_meshManager;     // Mesh manager.

	Input* m_input;	      // Input object.
	SceneManager* m_sceneManager; // Scene manager.

	Camera* m_camera;
	D3DXMATRIX m_view;

public:
	Engine(EngineSetup* setup = NULL);
	virtual ~Engine();

	void	       Run();
	static Engine* GetInstance();

	HWND GetWindow();
	void SetDeactiveFlag(bool deactive);

	float		  GetScale();
	IDirect3DDevice9* GetDevice();
	D3DDISPLAYMODE* GetDisplayMode();
	ID3DXSprite* GetSprite();

	void   AddState(State* state, bool change = true);
	void   RemoveState(State* state);
	void   ChangeState(unsigned long id);
	State* GetCurrentState();

	ResourceManager<Script>* GetScriptManager();
	ResourceManager<Material>* GetMaterialManager();
	ResourceManager<Mesh>* GetMeshManager();

	Input* GetInput();
	SceneManager* GetSceneManager();
};


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern Engine* g_engine;

#endif // ENGINE_H_