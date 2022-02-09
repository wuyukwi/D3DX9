/********************************************************************************* 

  *FileName: bullet.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/07

**********************************************************************************/  


#ifndef BULLET_H_
#define BULLET_H_

//-----------------------------------
// Bullet
//-----------------------------------
class Bullet
{
private:
	float					m_totalDistance;
	bool					m_expired;
	bool					m_hit;
	D3DXVECTOR3				m_translation;
	D3DXVECTOR3				m_direction;
	float					m_velocity;
	float					m_range;
	float					m_damage;

	D3DXMATRIX  m_worldMatrix; // World matrix.
	D3DXMATRIX m_translationMatrix; // Translation matrix.
	D3DXMATRIX m_rotationMatrix;    // Rotation matrix.

	LPDIRECT3DVERTEXBUFFER9 m_vb;

	TCVertex m_ray[2];
	SceneObject* m_hitobject;

public:
	Bullet(SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage);
	virtual ~Bullet();

	void Update();
	void Render();
	bool IsExpired();
};

//-----------------------------------
//  Bullet Manager
//-----------------------------------
class BulletManager
{
private:
	std::vector<Bullet>* m_bullets;

public:
	BulletManager();
	virtual ~BulletManager();

	void Update();
	void Render();
	void AddBullet(SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage);
};


#endif // BULLET_H_