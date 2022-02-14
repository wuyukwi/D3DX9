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
Enemy::Enemy(D3DXVECTOR3 pos)
{
    // 初期化
    vPickRayOrig = D3DXVECTOR3(o[0], o[1], o[2]);
    vPickRayDir = D3DXVECTOR3(d[0], d[1], d[2]);

    // xファイル読み込み オブジェクト作成
    m_hit = 0;

    m_enemy = new SceneObject(0, "data\\enemy.x");
    m_bullet = new BulletManager();

    m_enemy->SetTranslation(pos);
}


//-----------------------------------------------------------------------------
// The skybox class destructor.
//-----------------------------------------------------------------------------
Enemy::~Enemy()
{
    SAFE_DELETE(m_enemy);
    SAFE_DELETE(m_bullet);
}

//-----------------------------------------------------------------------------
// The Enemy class update.
//-----------------------------------------------------------------------------
void Enemy::Update()
{
    //CAnimInstance* mesh = m_enemy->GetMesh();

    //vPickRayOrig = D3DXVECTOR3(o[0], o[1], o[2]);
    //vPickRayDir = D3DXVECTOR3(d[0], d[1], d[2]);

    //D3DXIntersect(mesh->GetOrigMesh(), &vPickRayOrig, &vPickRayDir, &m_hit,
    //    NULL, NULL, NULL, NULL,
    //    NULL, NULL);

    float speed = Engine::GetInstance()->GettimeDelta();
    //static float angle = 0.0f;
    //ImGui::SliderFloat3("vPickRayOrig", &o[0], -100.0f, 100.0f);
    //ImGui::SliderFloat3("vPickRayDir", &d[0], -100.0f, 100.0f);
    //ImGui::SliderFloat("angle", &angle, -D3DX_PI, D3DX_PI);
    //ImGui::SliderFloat("speed", &speed, 0.01f, 0.1f);
    //ImGui::Text("angle: %.1f ", angle);
    //ImGui::Text("hit %d ", m_hit);
    //ImGui::Text("Enemy position(%.1f ,%.1f ,%.1f)", m_enemy->GetTranslation().x, m_enemy->GetTranslation().y, m_enemy->GetTranslation().z);

    //Input* pinput = Engine::GetInstance()->GetInput();

    //// 交点計算用
    //static D3DXVECTOR3	vMousePt;
    //static D3DXPLANE plane;
    //static D3DXVECTOR3 vScreen;
    //D3DXVECTOR3 v1(1.0, 0.0f, 1.0);
    //D3DXVECTOR3 v2(-1.0f, 0.0f, 1.0f);
    //D3DXVECTOR3 v3(-1.0f, 0.0f, -1.0f);
    //D3DXVECTOR3 vOut;


    //if (pinput->GetButtonPress(1))
    //{
    //    // ビュー、ワールド、プロジェクトマトリクスの取得
    //    static D3DXMATRIXA16 matView, matWorld, matProj;
    //    Engine::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
    //    Engine::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
    //    //m_device->GetTransform(D3DTS_WORLD, &matWorld);
    //    D3DXMatrixIdentity(&matWorld);

    //    // ビューポートの取得
    //    static D3DVIEWPORT9 viewPort;
    //    Engine::GetInstance()->GetDevice()->GetViewport(&viewPort);

    //    // マウススクリーン座標からワールド座標を求める
    //    vScreen = D3DXVECTOR3((float)pinput->GetPosX(), (float)pinput->GetPosY(), 0.0f);
    //    D3DXVec3Unproject(&vOut, &vScreen, &viewPort, &matProj, &matView, &matWorld);

    //    // 交点を算出
    //    D3DXPlaneFromPoints(&plane, &v1, &v2, &v3);
    //    D3DXPlaneIntersectLine(&vMousePt, &plane, Engine::GetInstance()->GetCamera()->getPosition(), &vOut);

    //    //计算并设置角色方向角
    //    angle = (float)atan2(vMousePt.z, vMousePt.x);

    //    // 位置を反映
    //    //float orientation = 3 * D3DX_PI / 2 - angle;

    //    m_bullet->AddBullet(m_enemy, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -100.0f), 0.01f, 0, 0);
    //}

    //D3DXMatrixRotationY(m_enemy->GetRotationMatrix(), 3 * D3DX_PI / 2 - angle);
    static float runSpeed = 1.0f;
    if (m_enemy->GetTranslation()->z > 0.0f)
        m_enemy->GetTranslation()->z -= runSpeed;
    else if(m_enemy->GetTranslation()->z < 0.0f)
        m_enemy->GetTranslation()->z += runSpeed;

    if (m_enemy->GetTranslation()->x > 0.0f)
        m_enemy->GetTranslation()->x -= runSpeed;
    else if (m_enemy->GetTranslation()->x < 0.0f)
        m_enemy->GetTranslation()->x += runSpeed;


    m_enemy->SetSpeed(speed);
    m_enemy->Update();

    ImGui::Begin("enemy");
    ImGui::SliderFloat("x", &m_enemy->GetTranslation()->x, -1000.0f, 1000.0f);
    ImGui::SliderFloat("z", &m_enemy->GetTranslation()->z, -1000.0f, 1000.0f);
    ImGui::SliderFloat("speed", &runSpeed, 0.0f, 10.0f);
    ImGui::End();

    //m_bullet->Update();

    //TCVertex v[2];
    //v[0] = TCVertex(vPickRayOrig);
    //v[1] = TCVertex(vPickRayDir);

    //Engine::GetInstance()->GetDevice()->CreateVertexBuffer(
    //    2 * sizeof(TCVertex),
    //    D3DUSAGE_WRITEONLY,
    //    TC_VERTEX_FVF,
    //    D3DPOOL_MANAGED,
    //    &m_vb, NULL);

    //VOID* pVertices;
    //m_vb->Lock(0, sizeof(v), (void**)&pVertices, 0);
    //memcpy(pVertices, v, sizeof(v));
    //m_vb->Unlock();
}

//-----------------------------------------------------------------------------
// Enemy render.
//-----------------------------------------------------------------------------
void Enemy::Render()
{

    // プレイヤーの描画
    m_enemy->Render();
    //m_bullet->Render();


    //D3DXMATRIX wm;
    ////D3DXMatrixIdentity(&wm);
    //wm = *m_enemy->GetWorldMatrix();
    //// 頂点バッファをデバイスのデータストリームに設定
    //Engine::GetInstance()->GetDevice()->SetStreamSource(0, m_vb, 0, sizeof(TCVertex));

    //// 頂点フォーマット
    //Engine::GetInstance()->GetDevice()->SetFVF(TC_VERTEX_FVF);


    //Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &wm);

    //Engine::GetInstance()->GetDevice()->DrawPrimitive(
    //    D3DPT_LINELIST, 0, 1);
}

SceneObject* Enemy::GetSceneObject()
{
    return m_enemy;
}
