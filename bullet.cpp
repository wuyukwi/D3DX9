/********************************************************************************* 

  *FileName: bullet.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  
#include "engine.h"

Bullet::Bullet(SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage)
{
	m_totalDistance = 0.0f;
	m_expired = false;
	m_hit = false;
	m_hitobject = hitobject;
	m_translation = translation;
	m_direction = direction;
	m_velocity = velocity;
	m_range = range;
	m_damage = damage;

	m_ray[0] = TCVertex(m_translation);
	m_ray[1] = TCVertex(m_direction);

	/*v[2] = LVertex(0.0f, size, 0.0f, 0.0f, 1.0f);
	v[3] = LVertex(size, size, 0.0f, 1.0f, 1.0f);

	Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
		4 * sizeof(LVertex),
		D3DUSAGE_WRITEONLY,
		L_VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_vb, NULL);

	VOID* pVertices;
	m_vb->Lock(0, sizeof(v), (void**)&pVertices, 0);
	memcpy(pVertices, v, sizeof(v));
	m_vb->Unlock();*/

}


Bullet::~Bullet()
{
	//SAFE_DELETE(m_vb);
}


void Bullet::Update()
{
	m_direction.z -= m_velocity;
	m_translation.z -= m_velocity;
	if (m_translation.z <= -1000.0f)
	{
		m_expired = true;
		this->~Bullet();
	}
	

	ImGui::Begin("bullet update");
	ImGui::SliderFloat("tx", &m_translation.x, -1000.0f, 1000.0f);
	ImGui::SliderFloat("tz", &m_translation.z, -1000.0f, 1000.0f);
	ImGui::SliderFloat("dx", &m_direction.x, -1000.0f, 1000.0f);
	ImGui::SliderFloat("dy", &m_direction.y, -1000.0f, 1000.0f);
	ImGui::SliderFloat("dz", &m_direction.z, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("m_direction", &m_direction, -D3DX_PI, D3DX_PI);
	ImGui::SliderFloat("m_velocity", &m_velocity, 0.0f, 100.0f);
	ImGui::Text("hit:%d ", m_hit);
	ImGui::End();

	m_hit = Engine::GetInstance()->GetScene()->RayIntersectScene(m_translation, m_direction, false, m_hitobject, true);

	m_ray[0] = TCVertex(m_translation);
	m_ray[1] = TCVertex(m_direction);

	Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
		2 * sizeof(TCVertex),
		D3DUSAGE_WRITEONLY,
		TC_VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_vb, NULL);

	VOID* pVertices;
	m_vb->Lock(0, sizeof(m_ray), (void**)&pVertices, 0);
	memcpy(pVertices, m_ray, sizeof(m_ray));
	m_vb->Unlock();
}

void Bullet::Render()
{

	/*D3DXMATRIX wm;
	D3DXMatrixIdentity(&wm);*/


	// 頂点バッファをデバイスのデータストリームに設定
	Engine::GetInstance()->GetDevice()->SetStreamSource(0, m_vb, 0, sizeof(TCVertex));

	// 頂点フォーマット
	Engine::GetInstance()->GetDevice()->SetFVF(TC_VERTEX_FVF);


	Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, m_hitobject->GetWorldMatrix());

	Engine::GetInstance()->GetDevice()->DrawPrimitive(
		D3DPT_LINELIST, 0, 1);

}

bool Bullet::IsExpired()
{
	return m_expired;
}


//-------------------------------------------------
//	BulletManager Class
//-------------------------------------------------

BulletManager::BulletManager()
{
	m_bullets = new std::vector<Bullet>();
}


BulletManager::~BulletManager()
{
	SAFE_DELETE(m_bullets);
}


void BulletManager::Update()
{
	std::vector<Bullet>::iterator iter;
	for (iter = m_bullets->begin(); iter != m_bullets->end(); iter++)
	{
		if (iter->IsExpired())
		{
			iter->~Bullet();
		}
		else
		{
			iter->Update();
		}
	}
	
}

void BulletManager::Render()
{
	std::vector<Bullet>::iterator iter;
	for (iter = m_bullets->begin(); iter != m_bullets->end(); iter++)
	{
		iter->Render();
	}

}


void BulletManager::AddBullet(SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage)
{
	m_bullets->push_back(Bullet(hitobject,translation, direction, velocity, range, damage));
}