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

    D3DXCreateTextureFromFileA(m_device, "data\\floor.jpg", &m_tex);

	// Create the player object
	m_player = new Player(m_device);
   
	// Create the skybox object
	m_skybox = new Skybox(10000.0f, m_device);



    // ステージステートの設定
    m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    // light off
    m_device->SetRenderState(D3DRS_LIGHTING, false);
}

Scene::~Scene()
{
    SAFE_RELEASE(m_tex);
    SAFE_RELEASE(m_buf);


}

void Scene::Update()
{
	
	m_player->Update();

	//设置世界矩阵
	static D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_device->SetTransform(D3DTS_WORLD, &matWorld);

	//获取当前世界变换矩阵
	/*static D3DXMATRIXA16 matProj;
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_WORLD, &matWorld);*/

	//获取当前视口信息
	static D3DVIEWPORT9 viewPort;
	m_device->GetViewport(&viewPort);

	//获取当前鼠标点的位置
	Input* input = Engine::GetInstance()->GetInput();


	

	static bool mouseL = false;
	if (input->GetButtonPress(0,mouseL))
	{
		mouseL = true;
	}
	else
	{
		mouseL = false;
	}
	D3DXVECTOR3 vOut;

	//计算交点
	static D3DXVECTOR3	vMousePt;
	static D3DXPLANE plane;
	static D3DXVECTOR3 vScreen;

	D3DXMATRIX rot;
	//float fAngle1;



	//计算并设置角色方向角
	//fAngle1 = (float)atan2(vMousePt.z, vMousePt.x);

	//D3DXMatrixRotationY(&rot, 3 * D3DX_PI / 2 - fAngle1);



	//if(mouseL)
	//{ 
	////计算并设置地面位置
	//m_vPosGround.x -= m_fWalkStep * ((float)cos(fAngle1));
	//m_vPosGround.z -= m_fWalkStep * ((float)sin(fAngle1));
	//}
	
	//构造地面世界矩阵
	D3DXMatrixTranslation(&m_matGround, m_vPosGround.x, 0.0f, m_vPosGround.z);
	
}

void Scene::Render()
{
	m_skybox->Render();

	//渲染地面
	m_device->SetTransform(D3DTS_WORLD, &m_matGround);
	m_device->SetTexture(0, m_tex);
	m_device->SetStreamSource(0, m_buf, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// プレイヤーの描画
	m_player->Render();
}