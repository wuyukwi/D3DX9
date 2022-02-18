/********************************************************************************* 

  *FileName: bullet.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  
#include "engine.h"

struct VERTEX_2D
{
	float x, y, z;  //pos
	float u, v;		//tex
};
#define FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_TEX1)

Bullet::Bullet(/*SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage*/)
{
	//m_totalDistance = 0.0f;
	//m_expired = false;
	//m_hit = false;
	//m_hitobject = hitobject;
	//m_translation = translation;
	//m_direction = direction;
	//m_velocity = velocity;
	//m_range = range;
	//m_damage = damage;

	D3DXMatrixIdentity(&m_worldMatrix);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(Engine::GetInstance()->GetDevice(),
		"data/tex/bullet000.png",
		&m_tex);

	m_vb = NULL;

	////頂点バッファの生成
	//Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
	//	4 * sizeof(Vertex),
	//	D3DUSAGE_WRITEONLY,
	//	VERTEX_FVF,
	//	D3DPOOL_MANAGED,
	//	&m_vb, NULL);

	//m_bullet = new Vertex[4];

	//m_bullet[0].uv= D3DXVECTOR2(0.0f, 0.0f);
	//m_bullet[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	//m_bullet[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_bullet[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	//VOID* pVertices;
	//m_vb->Lock(0, sizeof(Vertex), (void**)&pVertices, 0);
	//memcpy(pVertices, m_bullet, sizeof(Vertex));
	//m_vb->Unlock();
	// 
	//m_ray[0] = TCVertex();
	//m_ray[1] = TCVertex();


	//Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
	//	2 * sizeof(TCVertex),
	//	D3DUSAGE_WRITEONLY,
	//	L_VERTEX_FVF,
	//	D3DPOOL_MANAGED,
	//	&m_vb, NULL);

	//VOID* pVertices;
	//m_vb->Lock(0, sizeof(m_ray), (void**)&pVertices, 0);
	//memcpy(pVertices, m_ray, sizeof(m_ray));
	//m_vb->Unlock();

}


Bullet::~Bullet()
{
	SAFE_DELETE(m_vb);
}


void Bullet::Update(const float& elapsed)
{
	/*m_direction.z -= m_velocity;
	m_translation.z -= m_velocity;
	if (m_translation.z <= -1000.0f)
	{
		m_expired = true;
		this->~Bullet();
	}*/


	//ImGui::Begin("bullet update");
	//ImGui::SliderFloat("tx", &rp.x, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("tx", &rp.y, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("tz", &rp.z, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("dx", &rd.x, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("dy", &rd.y, -1000.0f, 1000.0f);
	//ImGui::SliderFloat("dz", &rd.z, -1000.0f, 1000.0f);
	////ImGui::SliderFloat("m_direction", &m_direction, -D3DX_PI, D3DX_PI);
	//ImGui::End();

	//m_hit = Engine::GetInstance()->GetScene()->RayIntersectScene(m_translation, m_direction, false, m_hitobject, true);

	//m_ray[0] = TCVertex(rp);
	//m_ray[1] = TCVertex(rd);

	//Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
	//	2 * sizeof(TCVertex),
	//	D3DUSAGE_WRITEONLY,
	//	TC_VERTEX_FVF,
	//	D3DPOOL_MANAGED,
	//	&m_vb, NULL);

	//VOID* pVertices;
	//m_vb->Lock(0, sizeof(m_ray), (void**)&pVertices, 0);
	//memcpy(pVertices, m_ray, sizeof(m_ray));
	//m_vb->Unlock();

	Input* pInput = Engine::GetInstance()->GetInput();
	float size = 50.0f;

	Engine::GetInstance()->GetDevice()->CreateVertexBuffer(4 * sizeof(VERTEX_2D), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vb, 0);

	VERTEX_2D vertices[] =
	{
		{float(pInput->GetPosX()) - size, float(pInput->GetPosY()) - size, 0.0f , 0.0f, 0.0f},
		{float(pInput->GetPosX()) + size, float(pInput->GetPosY()) - size, 0.0f , 1.0f, 0.0f},
		{float(pInput->GetPosX()) - size, float(pInput->GetPosY()) + size, 0.0f , 0.0f, 1.0f},
		{float(pInput->GetPosX()) + size, float(pInput->GetPosY()) + size, 0.0f , 1.0f, 1.0f},
	};

	//
	void* pVertices = NULL;
	//
	m_vb->Lock(0, 0, (void**)&pVertices, 0);
	//
	memcpy(pVertices, vertices, sizeof(vertices));
	//
	m_vb->Unlock();
}

void Bullet::Render()
{

	/*D3DXMATRIX wm;
	D3DXMatrixIdentity(&wm);*/

	//頂点バッファをデータストリームに設定
	Engine::GetInstance()->GetDevice()->SetStreamSource(0, m_vb, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	Engine::GetInstance()->GetDevice()->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	Engine::GetInstance()->GetDevice()->SetTexture(0, m_tex);

	Engine::GetInstance()->GetDevice()->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//// 頂点バッファをデバイスのデータストリームに設定
	//Engine::GetInstance()->GetDevice()->SetStreamSource(0, m_vb, 0, sizeof(TCVertex));

	//// 頂点フォーマット
	//Engine::GetInstance()->GetDevice()->SetFVF(TC_VERTEX_FVF);


	//Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &m_worldMatrix);

	//Engine::GetInstance()->GetDevice()->DrawPrimitive(
	//	D3DPT_LINELIST, 0, 1);

}

bool Bullet::IsExpired()
{
	return m_expired;
}


//-------------------------------------------------
//	BulletManager Class
//-------------------------------------------------

//BulletManager::BulletManager()
//{
//	m_bullets = new std::vector<Bullet>();
//}
//
//
//BulletManager::~BulletManager()
//{
//	SAFE_DELETE(m_bullets);
//}
//
//
//void BulletManager::Update()
//{
//	std::vector<Bullet>::iterator iter;
//	for (iter = m_bullets->begin(); iter != m_bullets->end(); iter++)
//	{
//		if (iter->IsExpired())
//		{
//			iter->~Bullet();
//		}
//		else
//		{
//			iter->Update();
//		}
//	}
//	
//}
//
//void BulletManager::Render()
//{
//	std::vector<Bullet>::iterator iter;
//	for (iter = m_bullets->begin(); iter != m_bullets->end(); iter++)
//	{
//		iter->Render();
//	}
//
//}
//
//
//void BulletManager::AddBullet(SceneObject* hitobject, D3DXVECTOR3 translation, D3DXVECTOR3 direction, float velocity, float range, float damage)
//{
//	m_bullets->push_back(Bullet(hitobject,translation, direction, velocity, range, damage));
//}