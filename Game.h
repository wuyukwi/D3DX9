#ifndef GAME_H
#define GAME_H


#define STATE_GAME 1


//-----------------------------------------------------------------------------
// Game Class
//-----------------------------------------------------------------------------
class Game : public State
{
public:
	Game();
	static Game* GetInstance();

	virtual void Load();
	virtual void Close();

	virtual void RequestViewer( ViewerSetup *viewer );
	virtual void Update( float elapsed );
	virtual void Render();

	BulletManager *GetBulletManager();


private:
	static Game* m_instance;
	Material*    m_crosshair; // Material used to render the crosshair.

	char  m_scoreBoardNames[MAX_PATH];  // Text for displaying the names of all the connected players.
	char  m_scoreBoardFrags[MAX_PATH];  // Text for displaying each player's frag count.
	char  m_scoreBoardDeaths[MAX_PATH]; // Text for displaying each player's death tally.
	Font* m_scoreBoardFont; 	    // Font used to render the score board.

	BulletManager *m_bulletManager; // Bullet manager.
	PlayerManager* m_playerManager; // Player manager.

};


#endif