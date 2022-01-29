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
	m_vEyePt = D3DXVECTOR3(1800.0f, 2545.0f, -1800.0f);
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

    D3DXCreateTextureFromFile(m_device, L"data\\floor.jpg", &m_tex);

    // Create the animation object.
    m_animation = new CD3DXAnimation(m_device);
    m_animation->Init(L"data\\tiny.x");

    // Create the animIntance object
    m_animInstance = new CAnimInstance();
    m_animInstance->Init(m_animation);
   
   // // プロジェクションマトリックスの初期化
   // D3DXMATRIXA16 matProj;
   //// D3DXMatrixIdentity(&matProj);

   // // プロジェクションマトリックスの作成
   // float fAspectRatio = 1280.0f / 720.0f;
   // D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, fAspectRatio, 1.0f, 100000.0f);

   // // プロジェクションマトリックスの設定
   // m_device->SetTransform(D3DTS_PROJECTION, &matProj);

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

    SAFE_DELETE(m_animation);
    SAFE_DELETE(m_animInstance);
}

void Scene::Update()
{

	//设置观察矩阵
	static D3DXMATRIX matView;
	D3DXVECTOR3	vLookatPt(-200.0f, 0.0f, 200.0f);
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &m_vEyePt, &vLookatPt, &vUpVec);
	Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	//设置世界矩阵
	static D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_device->SetTransform(D3DTS_WORLD, &matWorld);

	if (GetCapture())
	{
		//获取当前世界变换矩阵
		static D3DXMATRIXA16 matProj, matView, matWorld;
		m_device->GetTransform(D3DTS_PROJECTION, &matProj);
		m_device->GetTransform(D3DTS_VIEW, &matView);
		m_device->GetTransform(D3DTS_WORLD, &matWorld);

		//获取当前视口信息
		static D3DVIEWPORT9 viewPort;
		m_device->GetViewport(&viewPort);

		//获取当前鼠标点的位置
		Input* input = Engine::GetInstance()->GetInput();
		;
		D3DXVECTOR3 vScreen((float)input->GetPosX(), (float)input->GetPosY(), 0.0f), vOut;
		D3DXVec3Unproject(&vOut, &vScreen, &viewPort, &matProj, &matView, &matWorld);

		//计算交点
		static D3DXVECTOR3	vMousePt;
		static D3DXPLANE plane;
		D3DXVECTOR3 v1(1.0f, 0.0f, 1.0f);
		D3DXVECTOR3 v2(-1.0f, 0.0f, 1.0f);
		D3DXVECTOR3 v3(-1.0f, 0.0f, -1.0f);
		D3DXPlaneFromPoints(&plane, &v1, &v2, &v3);
		D3DXPlaneIntersectLine(&vMousePt, &plane, &m_vEyePt, &vOut);

		//计算并设置角色方向角
		float fAngle1 = (float)atan2(vMousePt.z, vMousePt.x);
		D3DXMATRIX rot;
		D3DXMatrixRotationY(&rot, 3 * D3DX_PI / 2 - fAngle1);
		m_animInstance->SetMatrix(&rot);

		m_animInstance->Update(1.0f);

		//计算并设置地面位置
		m_vPosGround.x -= m_fWalkStep * ((float)cos(fAngle1));
		m_vPosGround.z -= m_fWalkStep * ((float)sin(fAngle1));
	}

	//构造地面世界矩阵
	D3DXMatrixTranslation(&m_matGround, m_vPosGround.x, 0.0f, m_vPosGround.z);
}

void Scene::Render()
{
	//渲染地面
	m_device->SetTransform(D3DTS_WORLD, &m_matGround);
	m_device->SetTexture(0, m_tex);
	m_device->SetStreamSource(0, m_buf, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//渲染角色模型
	m_animInstance->Render();
}