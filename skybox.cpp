/********************************************************************************* 

  *FileName: skybox.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/31

**********************************************************************************/ 
#include "engine.h"

struct SkyBoxVertex
{
	float x, y, z;  //定点位置
	float u, v;		//纹理位置
};
#define D3DFVF_SKYBOX D3DFVF_XYZ | D3DFVF_TEX1

//-----------------------------------------------------------------------------
// The skybox class constructor.
//-----------------------------------------------------------------------------
CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pVertexBuffer = NULL;
	for (int i = 0; i < 5; i++)
		m_pTexture[i] = NULL;
	m_fLength = 0.0f;
}


CSkyBox::~CSkyBox(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}

bool CSkyBox::InitSkyBox(float length)
{
	m_fLength = length;

	//创建定点缓冲区
	m_pDevice->CreateVertexBuffer(20 * sizeof(SkyBoxVertex), 0, D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);

	float u = 1.0f / 4;
	float v = 1.0f / 3;

	SkyBoxVertex vertices[] =
	{
		//前面的顶点
		{-m_fLength / 2, 0.0f, m_fLength / 2, 2*u, 2*v,},
		{-m_fLength / 2, m_fLength / 2, m_fLength / 2, 2*u, v,},
		{m_fLength / 2, 0.0f, m_fLength / 2, 3*u, 2*v,},
		{m_fLength / 2, m_fLength / 2, m_fLength / 2, 3*u, v,},

		//后面的顶点
		{m_fLength / 2, 0.0f, -m_fLength / 2, 0.0f, 2*v,},
		{m_fLength / 2, m_fLength / 2, -m_fLength / 2, 0.0f, v,},
		{-m_fLength / 2, 0.0f, -m_fLength / 2, u, 2*v,},
		{-m_fLength / 2, m_fLength / 2, -m_fLength / 2, u, v,},

		//左面的顶点
		{-m_fLength / 2, 0.0f, -m_fLength / 2 , u, 2 * v,},
		{-m_fLength / 2, m_fLength / 2, -m_fLength / 2, u, v,},
		{-m_fLength / 2, 0.0f, m_fLength / 2, 2*u, 2 * v,},
		{-m_fLength / 2, m_fLength / 2, m_fLength / 2, 2*u, v,},

		//右面的顶点
		{m_fLength / 2, 0.0f, m_fLength / 2, 3*u, 2*v,},
		{m_fLength / 2, m_fLength / 2, m_fLength / 2, 3 * u, v,},
		{m_fLength / 2, 0.0f, -m_fLength / 2, 1.0f, 2*v},
		{m_fLength / 2, m_fLength / 2, -m_fLength / 2, 1.0f, v,},

		//上面的顶点
		{m_fLength / 2, m_fLength / 2, -m_fLength / 2, u, 0.0f,},
		{m_fLength / 2, m_fLength / 2, m_fLength / 2, 2*u,0.0f,},
		{-m_fLength / 2, m_fLength / 2, -m_fLength / 2,  u, v,},
		{-m_fLength / 2, m_fLength / 2, m_fLength / 2, 2*u, v,},

	/*	{-m_fLength / 2, m_fLength / 2, m_fLength / 2, u, v,},
		{m_fLength / 2, m_fLength / 2, -m_fLength / 2, 2 * u, 0.0f,},
		{m_fLength / 2, m_fLength / 2, m_fLength / 2, 2 * u, v,},
		{-m_fLength / 2, m_fLength / 2, -m_fLength / 2, u, 0.0f,},*/
		
	};
	//填充定点缓冲数据
	void* pVertices = NULL;
	//加锁
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//拷贝顶点数据到缓冲区中
	memcpy(pVertices, vertices, sizeof(vertices));
	//解锁
	m_pVertexBuffer->Unlock();



	return true;
}

bool CSkyBox::InitSkyBoxTexture(LPSTR szFrontTexture)
{
	//从文件中加载天空盒纹理
	D3DXCreateTextureFromFile(m_pDevice, szFrontTexture, &m_pTexture[0]);//前
	
	return true;
}

void CSkyBox::RenderSkyBox(D3DXMATRIX* matWorld, bool bRenderFrame /* = false */)
{
	
	

	//纹理颜色混合的第一个参数用于输出
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//纹理颜色混合的第一个参数取纹理的颜色值
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//世界变换
	m_pDevice->SetTransform(D3DTS_WORLD, matWorld);
	//设置资源流
	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SkyBoxVertex));
	//设置灵活顶点格式
	m_pDevice->SetFVF(D3DFVF_SKYBOX);

	m_pDevice->SetTexture(0, m_pTexture[0]);

	//绘制
	for (int i = 0; i < 5; i++)
	{	
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}
