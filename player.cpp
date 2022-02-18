/********************************************************************************* 

  *FileName: .cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  

#include "engine.h"


//-----------------------------------------------------------------------------
// The player class constructor.
//-----------------------------------------------------------------------------
Player::Player()
{

    // xファイル読み込み オブジェクト作成

    m_player = new SceneObject(0, "data\\enemy.x");
    //m_bullet = new BulletManager();
    //m_camera = new Camera{ };

}
 

//-----------------------------------------------------------------------------
// The skybox class destructor.
//-----------------------------------------------------------------------------
Player::~Player()
{
    SAFE_DELETE(m_player);
    //SAFE_DELETE(m_bullet);
}

//-----------------------------------------------------------------------------
// The Player class update.
//-----------------------------------------------------------------------------
void Player::Update(const float& elapsed)
{
	//CAnimInstance* mesh = m_player->GetMesh();

 //   float speed = elapsed;

 //   static float angle = 0.0f;

 //   ImGui::SliderFloat("angle", &angle, -D3DX_PI, D3DX_PI);
 //   ImGui::SliderFloat("speed", &speed, 0.01f, 0.1f);
 //   ImGui::Text("angle: %.1f ", angle);

 //   ImGui::Text("Player position(%.1f ,%.1f ,%.1f)", m_player->GetTranslation()->x,m_player->GetTranslation()->y, m_player->GetTranslation()->z);

    //Input* pinput = Engine::GetInstance()->GetInput();




        // 位置を反映
        //float orientation = 3 * D3DX_PI / 2 - angle;

        //m_bullet->AddBullet(m_player, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, -100.0f), 0.01f, 0, 0);
    
  
    //D3DXMatrixRotationY(m_player->GetRotationMatrix(), 3 * D3DX_PI / 2 - angle);


   

 /*   TCVertex v[2];
    v[0] = TCVertex(vPickRayOrig);
    v[1] = TCVertex(vPickRayDir);

    Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
        2 * sizeof(TCVertex),
        D3DUSAGE_WRITEONLY,
        TC_VERTEX_FVF,
        D3DPOOL_MANAGED,
        &m_vb, NULL);*/

 /*   VOID* pVertices;
    m_vb->Lock(0, sizeof(v), (void**)&pVertices, 0);
    memcpy(pVertices, v, sizeof(v));
    m_vb->Unlock();*/
  /*  m_player->AddTranslation(0.1f,0.0f,0.0f);

    m_player->SetSpeed(speed);
    m_player->Update();
    m_bullet->Update();*/

   /* m_camera->pitch(pinput->GetDeltaY() * elapsed *0.1f);
    m_camera->yaw(pinput->GetDeltaX() * elapsed * 0.1f);
    
    m_camera->getViewMatrix(m_player->GetViewMatrix());
    Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, m_player->GetViewMatrix());*/
}

//-----------------------------------------------------------------------------
// Player render.
//-----------------------------------------------------------------------------
void Player::Render()
{

    // プレイヤーの描画
    //m_player->Render();
    //m_bullet->Render();
 

    //D3DXMATRIX wm;
    //D3DXMatrixIdentity(&wm);
    ////wm = *m_player->GetWorldMatrix();
    //// 頂点バッファをデバイスのデータストリームに設定
    //Engine::GetInstance()->GetDevice()->SetStreamSource(0, m_vb, 0, sizeof(TCVertex));

    //// 頂点フォーマット
    //Engine::GetInstance()->GetDevice()->SetFVF(TC_VERTEX_FVF);


    //Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &wm);

    //Engine::GetInstance()->GetDevice()->DrawPrimitive(
    //    D3DPT_LINELIST, 0, 1);
}

SceneObject* Player::GetPlayerObject()
{
    return m_player;
}
