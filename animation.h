/********************************************************************************* 

  *FileName: animathion.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef ANIMATION_H_
#define ANIMATION_H_

class CD3DXAnimation
{
private:
	IDirect3DDevice9*	m_pDevice;					// D3Dデバイス
	CAllocateHierarchy* m_pAllocateHier;			// アニメーションメッシュ
	LPD3DXFRAME			m_pFrameRoot;				// フレーム
	LPD3DXANIMATIONCONTROLLER m_pAnimController;	// アニメーションコントローラー
	LPD3DXMESH m_pOrigMesh;							// メッシュ
private:

	void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
public:
	CD3DXAnimation(IDirect3DDevice9* device);
	~CD3DXAnimation(void);

	// インターフェース

	// ボーンアニメーション初期化
	bool Init(LPCTSTR filename);

	//　ボーンコントローラーコピー
	LPD3DXANIMATIONCONTROLLER CloneAnimCtrl(void);

	// ボーンアニメーション描画
	void Render(const LPD3DXMATRIX matrix);

	// メッシュ情報の取得
	LPD3DXMESH GetOrigMesh()const { return m_pOrigMesh; }
};


#endif // ANIMATION_H_