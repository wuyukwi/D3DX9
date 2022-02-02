/********************************************************************************* 

  *FileName: .cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  

#include "engine.h"

//-----------------------------------------------------------------------------
// Skybox render
//-----------------------------------------------------------------------------
Player::Player(IDirect3DDevice9* device)
{
    // 初期化
    m_device = device;
    D3DXMatrixIdentity(&m_mtxPlayer);
    this->m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    this->m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    this->m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    this->m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    
    // xファイル読み込み アニメーションオブジェクト作成
    m_animation = new CD3DXAnimation(m_device);
    m_animation->Init(L"data\\enemy.x");

    m_animInstance = new CAnimInstance();
    m_animInstance->Init(m_animation);
    m_animInstance->SetMatrix(&m_mtxPlayer);
}

//-----------------------------------------------------------------------------
// Skybox render
//-----------------------------------------------------------------------------
Player::~Player()
{
    SAFE_DELETE(m_animation);
    SAFE_DELETE(m_animInstance);
}

void Player::Update()
{
    static float speed = 0.005f;
    static float angle = 0.0f;

    
    ImGui::SliderFloat("animspeed", &speed, 0.001f, 0.1f);
    ImGui::SliderFloat("angle", &angle, -D3DX_PI, D3DX_PI);
    ImGui::Text("angle: %.1f )", angle);
    ImGui::Text("Player position(%.1f ,%.1f ,%.1f)", m_pos.x, m_pos.y, m_pos.z);

    Input* pinput = Engine::GetInstance()->GetInput();

    // 交点計算用
    static D3DXVECTOR3	vMousePt;
    static D3DXPLANE plane;
    static D3DXVECTOR3 vScreen;
    D3DXVECTOR3 v1(1.0, 0.0f, 1.0);
    D3DXVECTOR3 v2(-1.0f, 0.0f, 1.0f);
    D3DXVECTOR3 v3(-1.0f, 0.0f, -1.0f);
    D3DXVECTOR3 vOut;


    if (pinput->GetButtonPress(0))
    {
        // ビュー、ワールド、プロジェクトマトリクスの取得
        static D3DXMATRIXA16 matView, matWorld, matProj;
        m_device->GetTransform(D3DTS_PROJECTION, &matProj);
        m_device->GetTransform(D3DTS_VIEW, &matView);
        //m_device->GetTransform(D3DTS_WORLD, &matWorld);
        D3DXMatrixIdentity(&matWorld);

        // ビューポートの取得
        static D3DVIEWPORT9 viewPort;
        m_device->GetViewport(&viewPort);

        // マウススクリーン座標からワールド座標を求める
        vScreen = D3DXVECTOR3((float)pinput->GetPosX(), (float)pinput->GetPosY(), 0.0f);
        D3DXVec3Unproject(&vOut, &vScreen, &viewPort, &matProj, &matView, &matWorld);

        // 交点を算出
        D3DXPlaneFromPoints(&plane, &v1, &v2, &v3);
        D3DXPlaneIntersectLine(&vMousePt, &plane, Engine::GetInstance()->GetCamera()->getPosition() , &vOut);

        //计算并设置角色方向角
        angle = (float)atan2(vMousePt.z, vMousePt.x);
        
    }

  

    //static float walkSpeed = 0.1f;

    //if (pinput->GetKeyPress(DIK_W, true))
    //{
    //    //angle = rot;
    //    m_move.x += cosf(angle) * walkSpeed;
    //    m_move.z += sinf(angle) * walkSpeed;
    //}

    //if (pinput->GetKeyPress(DIK_S, true))
    //{
    //    //angle = rot + (-D3DX_PI);
    //    m_move.x -= cosf(angle) * walkSpeed;
    //    m_move.z -= sinf(angle) * walkSpeed;
    //}
    //
    //if (pinput->GetKeyPress(DIK_A, true))
    //{
    //    m_move.x += cosf(angle) * walkSpeed;
    //    m_move.z += sinf(angle) * walkSpeed;
    //}

    //if (pinput->GetKeyPress(DIK_D, true))
    //{
    //    m_move.x -= cosf(angle) * walkSpeed;
    //    m_move.z -= sinf(angle) * walkSpeed;
    //}
    ////慣性・移動量を更新 (減衰させる)
    //m_move.x += (0.0f - m_move.x) * 0.1f;
    //m_move.z += (0.0f - m_move.z) * 0.1f;

    //m_pos += m_move;

    
    // 位置を反映
    float orientation = 3 * D3DX_PI / 2 - angle;
    D3DXMatrixRotationY(&m_mtxPlayer, orientation);
    m_animInstance->SetMatrix(&m_mtxPlayer);

    m_animInstance->Update(speed);

}

void Player::Render()
{
    // プレイヤーの描画
    m_animInstance->Render();
}