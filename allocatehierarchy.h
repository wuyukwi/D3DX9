/********************************************************************************* 

  *FileName: AllocateHierarchy.h 
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef AllocateHierarchy_H_
#define AllocateHierarchy_H_

//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: 
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	//纹理信息
	LPDIRECT3DTEXTURE9* ppTextures;			//テクスチャ  

	//网格信息             
	LPD3DXMESH pOrigMesh;					// メッシュ
	LPD3DXATTRIBUTERANGE pAttributeTable;	// 属性テーブルエントリ
	DWORD NumAttributeGroups;				// 属性テーブルエントリ数量
	DWORD NumInfl;							// ボーンにより影響の頂点最大値
	LPD3DXBUFFER pBoneCombinationBuf;		// ボーン結合バファ
	D3DXMATRIX** ppBoneMatrixPtrs;			// ボーン結合マトリクス
	D3DXMATRIX* pBoneOffsetMatrices;		// ボーン初期マトリクス
	DWORD NumPaletteEntries;				// ボーン最大値
	bool UseSoftwareVP;						// ソフトウェア使うかどうか
	DWORD iAttributeSW;						// used to denote the split between SW and HW if necessary for non-indexed skinning
};

//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
// xファイルからメッシュとアニメーション読み込み用
//--------------------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);
	HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	CAllocateHierarchy()
	{
	}
};

#endif // AllocateHierarchy_H_