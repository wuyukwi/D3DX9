/********************************************************************************* 

  *FileName: player.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  

#ifndef PLAYER_H_
#define PLAYER_H_

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Render();


private:
	SceneObject* m_player;
	BulletManager* m_bullet;

	LPD3DXMESH                 g_pPickMesh;               //用于计算拾取三角形的网格对象
	LPDIRECT3DVERTEXBUFFER9    m_vb;              //存放所拾取到的三角形的顶点缓冲区
	BOOL m_hit;

	float o[3] = { 0.0f,0.0f,0.0f };
	float d[3] = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vPickRayOrig, vPickRayDir;
};



#endif // PLAYER_H_