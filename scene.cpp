/*********************************************************************************

  *FileName: scene.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/

#include "engine.h"

Scene::Scene(IDirect3DDevice9* device)
{
    m_device = device;
    m_vPosGround = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    m_hit = FALSE;

    m_device->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
        0, D3DFVF_CUSTOMVERTEX,
        D3DPOOL_MANAGED, &m_buf, NULL);

    CUSTOMVERTEX vertices1[] =
    {
        { -5000.0f,  0.0f, -5000.0f,  0.0f,  10.0f, },
        { -5000.0f,  0.0f,  5000.0f,  0.0f,   0.0f, },
        {  5000.0f,  0.0f, -5000.0f, 10.0f,  10.0f, },
        {  5000.0f,  0.0f,  5000.0f, 10.0f,   0.0f, }
    };

    VOID* pVertices1;
    m_buf->Lock(0, sizeof(vertices1), (void**)&pVertices1, 0);
    memcpy(pVertices1, vertices1, sizeof(vertices1));

    m_buf->Unlock();

    D3DXCreateTextureFromFileA(m_device, "data/grass.jpg", &m_tex);

    // Create the player object
    //m_player = new Player();

    m_camera = new Camera{ };

    // Create the skybox object
    m_skybox = new CSkyBox(m_device);
    m_skybox->InitSkyBox(100000.0f);
    m_skybox->InitSkyBoxTexture("data/tex/skybox.jpg");

    m_enemylist = new LinkedList<SceneObject>;

    SceneObject* pEnemy = new SceneObject(0, "data/enemy.x");
    pEnemy->SetTranslation(D3DXVECTOR3(100.0, 0.0f, 100.0f));
    m_enemylist->Add(pEnemy);

    pEnemy = new SceneObject(1, "data/badboy.x");
    pEnemy->SetTranslation(D3DXVECTOR3(-100.0, 0.0f, -100.0f));
    m_enemylist->Add(pEnemy);

    pEnemy = new SceneObject(2, "data/fish.x");
    pEnemy->SetTranslation(D3DXVECTOR3(100.0, 0.0f, -100.0f));
    m_enemylist->Add(pEnemy);

    m_bullet = new Bullet();

    // light off
    m_device->SetRenderState(D3DRS_LIGHTING, false);
}

Scene::~Scene()
{
    SAFE_RELEASE(m_tex);
    SAFE_RELEASE(m_buf);
    SAFE_DELETE(m_enemylist);

}

void Scene::Update(const float& elapsed)
{
    //
    Input* pInput = Engine::GetInstance()->GetInput();

    SceneObject* pEnemy = NULL;

    //m_player->Update(elapsed);
    m_camera->Update(elapsed);
    
    if (Engine::GetInstance()->GetTime() % 500 == 0)
    {
        pEnemy = new SceneObject(0, "data/enemy.x");
        pEnemy->SetTranslation(D3DXVECTOR3(-float(rand()%1000), 0.0f, -float(rand() % 1000)));
        pEnemy->SetHp(rand() % 20);
        m_enemylist->Add(pEnemy);
    }

    if (Engine::GetInstance()->GetTime() % 600 == 0)
    {
        pEnemy = new SceneObject(1, "data/badboy.x");
        pEnemy->SetTranslation(D3DXVECTOR3(-float(rand() % 1000), 0.0f, -float(rand() % 1000)));
        pEnemy->SetHp(30 + rand() % 30);
        m_enemylist->Add(pEnemy);
    }

    if (Engine::GetInstance()->GetTime() % 700 == 0)
    {
        pEnemy = new SceneObject(2, "data/fish.x");
        pEnemy->SetTranslation(D3DXVECTOR3(-float(rand() % 1000), 0.0f, -float(rand() % 1000)));
        pEnemy->SetHp(50+rand() % 50);
        m_enemylist->Add(pEnemy);
    }

    if (Engine::GetInstance()->GetTime() % 4000 == 0)
    {
        pEnemy = new SceneObject(4, "data/boos.x");
        pEnemy->SetTranslation(D3DXVECTOR3(-float(rand() % 1000), 0.0f, -float(rand() % 1000)));
        pEnemy->SetHp(1000+rand() % 200);
        m_enemylist->Add(pEnemy);
    }

    //ImGui::Text("time %d", Engine::GetInstance()->GetTime());
    //ImGui::Text("vPickRayOrig(%.1f ,%.1f ,%.1f)", vPickRayOrig.x, vPickRayOrig.y, vPickRayOrig.z);
    //ImGui::Text("vPickRayDir(%.1f ,%.1f ,%.1f)", vPickRayDir.x, vPickRayDir.y, vPickRayDir.z);
    //

    //// 初期化
    //vPickRayOrig = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    //vPickRayDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // 交点計算用

    // ビュー、ワールド、プロジェクトマトリクスの取得
    static D3DXMATRIXA16  matWorld, matProj, m;
    Engine::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

    //m_device->GetTransform(D3DTS_WORLD, &matWorld);
    D3DXMatrixIdentity(&matWorld);


    //计算世界观察矩阵的逆矩阵
    D3DXMATRIX mWorldView = matWorld * *m_camera->GetViewMatrix();
    D3DXMatrixInverse(&m, NULL, &mWorldView);

    //计算拾取射线的方向与原点
    D3DXVECTOR3 vTemp;
    vTemp.x = (((2.0f * pInput->GetPosX()) / 1920) - 1) / matProj._11;
    vTemp.y = -(((2.0f * pInput->GetPosY()) / 1080) - 1) / matProj._22;
    vTemp.z = 1.0f;

    vPickRayDir.x = vTemp.x * m._11 + vTemp.y * m._21 + vTemp.z * m._31;
    vPickRayDir.y = vTemp.x * m._12 + vTemp.y * m._22 + vTemp.z * m._32;
    vPickRayDir.z = vTemp.x * m._13 + vTemp.y * m._23 + vTemp.z * m._33;

    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

    static int time = 60;
    if (Engine::GetInstance()->GetTime() % 100 == 0)
    {
        time--;
    }
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("time", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::Text("%d", time);
    ImGui::End();

    static int score = 0;
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("score", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::BulletText("%d", score);
    ImGui::End();

    pEnemy = m_enemylist->GetFirst();
    int enemyNum = 0;

    while (pEnemy != NULL)
    {
        if (pEnemy->GetEnabled())
        {
            ImGui::SetNextWindowBgAlpha(0.0f);
            int hp = pEnemy->GetHp();
            ImGui::Begin("enemy",NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
            ImGui::SliderInt(" ", &hp,0,100);
            ImGui::End();

            if (pEnemy->GetTranslation()->x < m_camera->getPosition()->x)
            {
                pEnemy->AddTranslation(D3DXVECTOR3(0.3f, 0.0f, 0.0f));
            }
            else if(pEnemy->GetTranslation()->x > m_camera->getPosition()->x)
            {
                pEnemy->AddTranslation(D3DXVECTOR3(-0.3f, 0.0f, 0.0f));
            }

            if (pEnemy->GetTranslation()->z < m_camera->getPosition()->z)
            {
                pEnemy->AddTranslation(D3DXVECTOR3(0.0f, 0.0f, 0.3f));
            }
            else if (pEnemy->GetTranslation()->z > m_camera->getPosition()->z)
            {
                pEnemy->AddTranslation(D3DXVECTOR3(0.0f, 0.0f, -0.3f));
            }

            pEnemy->SetSpeed(elapsed);
            pEnemy->Update();
            //m_bullet->Update(elapsed);
            if (pInput->GetButtonPress(0,true))
            {
                D3DXVECTOR3 rp, rd;
                D3DXMATRIX inverse;
                D3DXMatrixInverse(&inverse, NULL, pEnemy->GetWorldMatrix());
                D3DXVec3TransformCoord(&rp, &vPickRayOrig, &inverse);
                D3DXVec3TransformNormal(&rd, &vPickRayDir, &inverse);

                D3DXIntersect(pEnemy->GetSpereMesh(), &rp, &rd, pEnemy->SetHit(),
                    NULL, NULL, NULL, NULL,
                    NULL, NULL);


                if (*pEnemy->SetHit())
                {
                    //pEnemy->SetEnabled(false);
                    pEnemy->SetHp(-1);
                    //pEnemy->SetVisible(false);
                }

                if (pEnemy->GetHp() < 0)
                {
                    score += (100 + pEnemy->GetType() * 100);
                    pEnemy->SetEnabled(false);
                    pEnemy->SetVisible(false);
                }
                
            }
           /* if (!*pEnemy->SetHit())
            {
                pEnemy->SetVisible(true);
            }*/
        }
        pEnemy = m_enemylist->GetNext(pEnemy);
    }
   

    if (time < 0)
    {
        ImGui::SetNextWindowBgAlpha(1.0f);
        static bool use_work_area = true;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        if (ImGui::Begin("Example: Fullscreen window", NULL, flags))
        {

            ImGui::SameLine();

           

          
            ImGui::SetCursorPosX(1000.0f);
            ImGui::SetCursorPosY(500.0f);
            ImGui::Text("GAME OVER");
            ImGui::Indent(600.0f);
            ImGui::Text("You Score : %d", score);
            ImGui::Unindent();

        }
        ImGui::End();
    }
   
	
	//构造地面世界矩阵
	D3DXMatrixTranslation(&m_matGround, m_vPosGround.x, 0.0f, m_vPosGround.z);
	
}

void Scene::Render()
{
	D3DXMATRIX wm;
	D3DXMatrixIdentity(&wm);
	D3DXMatrixTranslation(&wm, 0.0f, -2000.0f, 0.0f);
	m_skybox->RenderSkyBox(&wm);

	//地面
	m_device->SetTransform(D3DTS_WORLD, &m_matGround);
	m_device->SetTexture(0, m_tex);
	m_device->SetStreamSource(0, m_buf, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	// プレイヤーの描画
	//m_player->Render();

    SceneObject* pEnemy = m_enemylist->GetFirst();
    while (pEnemy != NULL)
    {
        if(pEnemy->GetVisible())
            pEnemy->Render();

        pEnemy = m_enemylist->GetNext(pEnemy);
    }

    //m_bullet->Render();
}

//-----------------------------------------------------------------------------
// Returns the result of a ray intersection with the scene and all its objects.
//-----------------------------------------------------------------------------
bool Scene::RayIntersectScene(D3DXVECTOR3 rayPosition, D3DXVECTOR3 rayDirection, bool checkScene, SceneObject* thisObject, bool checkObjects)
{
	float hitDistance = 0.0f;

	// Check if the ray needs to check for intersection with the scene.
	//if (checkScene == true)
		//RecursiveSceneRayCheck(m_firstLeaf, result, rayPosition, rayDirection, &hitDistance);

	// Check if the ray needs to check for intersection with the objects.
	if (checkObjects == true)
	{
		BOOL hit;
		//D3DXIntersect(m_enemy->GetSceneObject()->GetMesh()->GetOrigMesh(), &rayPosition, &rayDirection, &hit, NULL, NULL, NULL, &hitDistance, NULL, NULL);
		if (hit)
			return true;
	}
}