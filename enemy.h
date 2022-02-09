/********************************************************************************* 

  *FileName: enemy.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/08

**********************************************************************************/  


#ifndef ENEMY_H_
#define ENEMY_H_

class Enemy
{
public:
	Enemy(D3DXVECTOR3 pos);
	~Enemy();

	void Update();
	void Render();

	SceneObject* GetSceneObject();
private:
	SceneObject* m_enemy;
	BulletManager* m_bullet;

	LPD3DXMESH                 g_pPickMesh;        //用于计算拾取三角形的网格对象
	LPDIRECT3DVERTEXBUFFER9    m_vb;              //存放所拾取到的三角形的顶点缓冲区
	BOOL m_hit;

	float o[3] = { 0.0f,0.0f,0.0f };
	float d[3] = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vPickRayOrig, vPickRayDir;
};


#endif // ENEMY_H_