#include "Main.h"

Game* Game::m_instance = NULL;

//-----------------------------------------------------------------------------
// Game class constructor.
//-----------------------------------------------------------------------------
Game::Game() : State( STATE_GAME )
{
	// Store a pointer to the game state in a global variable for easy access.
	m_instance = this;

	// Invalidate the score board font.
	m_scoreBoardFont = NULL;

	// Invalidate both the bullet manager and the player manager.
	m_bulletManager = NULL;
	m_playerManager = NULL;
	
}


Game* Game::GetInstance()
{
	if(!m_instance)
		m_instance = new Game();
	return m_instance;
}


//-----------------------------------------------------------------------------
// Allows the game state to preform any pre-processing construction.
//-----------------------------------------------------------------------------
void Game::Load()
{
	// Hide the mouse cursor.
	//ShowCursor( false );
	
	// Load the crosshair material.
	m_crosshair = Engine::GetInstance()->GetMaterialManager()->Add( "Crosshair.dds.txt", "./Assets/" );

	// Create the score board font.
	m_scoreBoardFont = new Font( "Arial", 14, FW_BOLD );
	m_scoreBoardNames[0] = 0;
	m_scoreBoardFrags[0] = 0;
	m_scoreBoardDeaths[0] = 0;

	// Create both the bullet manager and the player manager.
	m_bulletManager = new BulletManager;
	m_playerManager = new PlayerManager;


	// Load the scene from the host player's selection.
	Engine::GetInstance()->GetSceneManager()->LoadScene("Abandoned City.txt", "./Assets/Scenes/");

}

//-----------------------------------------------------------------------------
// Allows the game state to preform any post-processing destruction.
//-----------------------------------------------------------------------------
void Game::Close()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Destroy the scene.
	Engine::GetInstance()->GetSceneManager()->DestroyScene();

	// Destroy both the bullet manager and the player manager.
	SAFE_DELETE(m_bulletManager);
	SAFE_DELETE(m_playerManager);

	// Destroy the score board font.
	SAFE_DELETE(m_scoreBoardFont);

	// Destroy the crosshair material.
	Engine::GetInstance()->GetMaterialManager()->Remove(&m_crosshair);

}

//-----------------------------------------------------------------------------
// Returns the view setup details for the given frame.
//-----------------------------------------------------------------------------
void Game::RequestViewer( ViewerSetup *viewer )
{
	viewer->viewer = m_playerManager->GetViewingPlayer();
	viewer->viewClearFlags = D3DCLEAR_ZBUFFER;
}

//-----------------------------------------------------------------------------
// Update the game state.
//-----------------------------------------------------------------------------
void Game::Update( float elapsed )
{

	//// Allow the player to respawn by pressing the R key.
	//if (Engine::GetInstance()->GetInput()->GetKeyPress(DIK_R) == true)
	//{
	//	m_playerManager->GetLocalPlayer()->SetEnabled(false);
	//	m_playerManager->SpawnLocalPlayer();
	//}

	// Update the bullet manager before the player manager. This will prevent
	// bullets that are fired this frame from being processed this frame.
	m_bulletManager->Update(elapsed);

	// Update the player manager.
	m_playerManager->Update(elapsed);

	// Check if the user is holding down the tab key.
	if (Engine::GetInstance()->GetInput()->GetKeyPress(DIK_TAB, true) == true)
	{
		// Build the score board text.
		sprintf(m_scoreBoardNames, "PLAYER\n");
		sprintf(m_scoreBoardFrags, "FRAGS\n");
		sprintf(m_scoreBoardDeaths, "DEATHS\n");

	}

	// Check if the user wants to exit back to the menu.
	if (Engine::GetInstance()->GetInput()->GetKeyPress(DIK_ESCAPE))
		Engine::GetInstance()->ChangeState(STATE_MENU);
}

//-----------------------------------------------------------------------------
// Render the game state.
//-----------------------------------------------------------------------------
void Game::Render()
{
	// Ensure the scene is loaded.
	if( Engine::GetInstance()->GetSceneManager()->IsLoaded() == false )
		return;

	// If the user is holding down the tab key, then render the score board.
	if( Engine::GetInstance()->GetInput()->GetKeyPress( DIK_TAB, true ) == true )
	{
		m_scoreBoardFont->Render( m_scoreBoardNames, 20, 100, 0xFFFF7700 );
		m_scoreBoardFont->Render( m_scoreBoardFrags, 180, 100, 0xFFFF7700 );
		m_scoreBoardFont->Render( m_scoreBoardDeaths, 260, 100, 0xFFFF7700 );
	}

	// Draw the local player's crosshair in the centre of the screen.
	Engine::GetInstance()->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND );
	Engine::GetInstance()->GetSprite()->Draw( m_crosshair->GetTexture(), NULL, NULL, &D3DXVECTOR3( Engine::GetInstance()->GetDisplayMode()->Width / 2.0f - 15.0f, Engine::GetInstance()->GetDisplayMode()->Height / 2.0f - 15.0f, 0.0f ), 0xFFFFFFFF );
	Engine::GetInstance()->GetSprite()->End();
}

//-----------------------------------------------------------------------------
// Returns a pointer to the bullet manager.
//-----------------------------------------------------------------------------
BulletManager *Game::GetBulletManager()
{
	return m_bulletManager;
}

