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
	SceneObject* m_owner;
	RayIntersectionResult* m_hitResult;
	float					m_totalDistance;
	bool						m_expired;

	D3DXVECTOR3				m_translation;
	D3DXVECTOR3				m_direction;
	float					m_velocity;
	float					m_range;
	float					m_damage;

public:
	Bullet(SceneObject* owner, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage);
	virtual ~Bullet();

	void Update(float elapsed);
	bool IsExpired();
};


#endif // BULLET_H_