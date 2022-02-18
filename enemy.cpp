/********************************************************************************* 

  *FileName: enemy.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/08

**********************************************************************************/  
#include "engine.h"


//-----------------------------------------------------------------------------
// The Enemy class constructor.
//-----------------------------------------------------------------------------
Enemy::Enemy()
{

    // xファイル読み込み オブジェクト作成
    m_enemy = NULL;
    //m_bullet = new BulletManager();
    m_enemylist = new LinkedList<SceneObject>;
   
}


//-----------------------------------------------------------------------------
// The skybox class destructor.
//-----------------------------------------------------------------------------
Enemy::~Enemy()
{
    SAFE_DELETE(m_enemylist);
    //SAFE_DELETE(m_bullet);
}

//-----------------------------------------------------------------------------
// The Enemy class update.
//-----------------------------------------------------------------------------
void Enemy::Update(const float& elapsed)
{

 /*   static float runSpeed = 1.0f;
    if (m_enemy->GetTranslation()->z > 0.0f)
        m_enemy->GetTranslation()->z -= runSpeed;
    else if(m_enemy->GetTranslation()->z < 0.0f)
        m_enemy->GetTranslation()->z += runSpeed;

    if (m_enemy->GetTranslation()->x > 0.0f)
        m_enemy->GetTranslation()->x -= runSpeed;
    else if (m_enemy->GetTranslation()->x < 0.0f)
        m_enemy->GetTranslation()->x += runSpeed;*/

        // Go through the list of bullets.
    m_enemy = m_enemylist->GetFirst();

    while (m_enemy != NULL)
    {
        m_enemy->SetSpeed(elapsed);
        m_enemy->Update();

        m_enemy = m_enemylist->GetNext(m_enemy);
    }


}

//-----------------------------------------------------------------------------
// Enemy render.
//-----------------------------------------------------------------------------
void Enemy::Render()
{

    m_enemy = m_enemylist->GetFirst();

    while (m_enemy != NULL)
    {
        m_enemy->Render();

        m_enemy = m_enemylist->GetNext(m_enemy);
    }
   
}

void Enemy::AddEnemy(D3DXVECTOR3 pos, char* name)
{
    m_enemy = new SceneObject(0, "data/enemy.x");
    m_enemy->SetTranslation(pos);
    m_enemylist->Add(m_enemy);
}

SceneObject* Enemy::GetSceneObject()
{
    return m_enemy;
}
