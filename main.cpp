/********************************************************************************* 

  *FileName: .cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/18

**********************************************************************************/  

#include "main.h"


void StateSetup()
{
	Engine::GetInstance()->AddState(new Menu, false);
	Engine::GetInstance()->AddState(new Game, true);
}



int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
{

	// Create the engine setup structure.
	EngineSetup setup;
	setup.instance = instance;
	setup.name = "Cityscape";
	setup.scale = 0.01f;
	setup.totalBackBuffers = 1;
	setup.StateSetup = StateSetup;
	setup.spawnerPath = "./Assets/Objects/";
	setup.screen_width = 1280;
	setup.screen_height = 720;
	

	// Create the engine (using the setup structure), then run it.
	Engine* engine = new Engine(&setup);
	engine->Run();
	SAFE_DELETE(engine);

	return true;
}