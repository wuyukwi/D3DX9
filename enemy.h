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
	Enemy();
	~Enemy();

	void Update(const float& elapsed);
	void Render();
	void AddEnemy(D3DXVECTOR3 pos,char* name);

	SceneObject* GetSceneObject();
private:
	LinkedList< SceneObject >* m_enemylist;

	SceneObject* m_enemy;
	//BulletManager* m_bullet;

};


#endif // ENEMY_H_