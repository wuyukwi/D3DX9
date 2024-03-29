/********************************************************************************* 

  *FileName: main.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/18

**********************************************************************************/  

#include "main.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
{

	// Create the engine setup structure.
	EngineSetup setup;
	setup.instance = instance;
	setup.class_name = "windowClass";
	setup.window_name = "Game";
	setup.scale = 10.0f;
	setup.screen_width = 1920;
	setup.screen_height = 1080;
	

	// Create the engine (using the setup structure), then run it.
	Engine* engine = new Engine(&setup);
	engine->Run();
	SAFE_DELETE(engine);

	return true;
}