/********************************************************************************* 

  *FileName: skybox.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/31

**********************************************************************************/ 
#include "engine.h"

//-----------------------------------------------------------------------------
// The skybox class constructor.
//-----------------------------------------------------------------------------
Skybox::Skybox(float skyboxSize,IDirect3DDevice9* device)
{
    m_device = device;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(device,L"data\\tex\\skybox.jpg",&m_tex);

	// 頂点バッファの生成
	device->CreateVertexBuffer(
		sizeof(LVertex) * 24,
		0,
		L_VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_vb,
		NULL);

	LVertex Vtx[24];		// 頂点情報へのポインタ

	float fxsize = 1.0f / 4;
	float fysize = 1.0f / 3;

	// 前
	Vtx[0] = LVertex(-skyboxSize, -skyboxSize, skyboxSize, fxsize * 3, fysize * 2);
	Vtx[1] = LVertex(-skyboxSize, skyboxSize, skyboxSize, fxsize * 3, fysize);
	Vtx[2] = LVertex(skyboxSize, -skyboxSize, skyboxSize, fxsize * 4, fysize * 2);
	Vtx[3] = LVertex(skyboxSize, skyboxSize, skyboxSize, fxsize * 4, fysize);

	// 後ろ
	Vtx[4] = LVertex(skyboxSize, -skyboxSize, -skyboxSize, fxsize, fysize * 2);
	Vtx[5] = LVertex(skyboxSize, skyboxSize, -skyboxSize, fxsize, fysize);
	Vtx[6] = LVertex(-skyboxSize, -skyboxSize, -skyboxSize, fxsize * 2, fysize * 2);
	Vtx[7] = LVertex(-skyboxSize, skyboxSize, -skyboxSize, fxsize * 2, fysize);

	// 左
	Vtx[8] = LVertex(-skyboxSize, -skyboxSize, -skyboxSize, fxsize * 2, fysize * 2);
	Vtx[9] = LVertex(-skyboxSize, skyboxSize, -skyboxSize, fxsize * 2, fysize);
	Vtx[10] = LVertex(-skyboxSize, -skyboxSize, skyboxSize, fxsize * 3, fysize * 2);
	Vtx[11] = LVertex(-skyboxSize, skyboxSize, skyboxSize, fxsize * 3, fysize);

	// 右
	Vtx[12] = LVertex(skyboxSize, -skyboxSize, skyboxSize, 0.0f, fysize * 2);
	Vtx[13] = LVertex(skyboxSize, skyboxSize, skyboxSize, 0.0f, fysize);
	Vtx[14] = LVertex(skyboxSize, -skyboxSize, -skyboxSize, fxsize, fysize * 2);
	Vtx[15] = LVertex(skyboxSize, skyboxSize, -skyboxSize, fxsize, fysize);

	// 上
	Vtx[16] = LVertex(skyboxSize, skyboxSize, -skyboxSize, fxsize, fysize);
	Vtx[17] = LVertex(skyboxSize, skyboxSize, skyboxSize, fxsize, 0.0f);
	Vtx[18] = LVertex(-skyboxSize, skyboxSize, -skyboxSize, fxsize * 2, fysize);
	Vtx[19] = LVertex(-skyboxSize, skyboxSize, skyboxSize, fxsize * 2, 0.0f);

	// 下
	Vtx[20] = LVertex(-skyboxSize, -skyboxSize, -skyboxSize, fxsize * 2, fysize * 2);
	Vtx[21] = LVertex(-skyboxSize, -skyboxSize, skyboxSize, fxsize * 2, fysize * 3);
	Vtx[22] = LVertex(skyboxSize, -skyboxSize, -skyboxSize, fxsize, fysize * 2);
	Vtx[23] = LVertex(skyboxSize, -skyboxSize, skyboxSize, fxsize, fysize * 3);

	// 頂点情報をロックし、頂点情報書き込み
	VOID* pVertices;
	m_vb->Lock(0, sizeof(Vtx), (void**)&pVertices, 0);
	memcpy(pVertices, Vtx, sizeof(Vtx));

	// 頂点バッファをアンロックする
	m_vb->Unlock();

}


//-----------------------------------------------------------------------------
// The skybox class destructor.
//-----------------------------------------------------------------------------
Skybox::~Skybox()
{
	SAFE_RELEASE(m_tex);
	SAFE_RELEASE(m_vb);
}

//-----------------------------------------------------------------------------
// Skybox render
//-----------------------------------------------------------------------------
void Skybox::Render()
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&mtxRot, -0.000005f * timeGetTime());

	mtxWorld = mtxRot * mtxTrans;

	m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	// ワールドマトリックスの設定
	m_device->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	m_device->SetStreamSource(0, m_vb, 0, sizeof(L_VERTEX_FVF));

	// 頂点フォーマットの設定
	m_device->SetFVF(L_VERTEX_FVF);

	// テクスチャの設定
	m_device->SetTexture(0, m_tex);

	for (int i = 0; i < 6; i++)
	{
		// ポリゴンの描画
		m_device->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}
}