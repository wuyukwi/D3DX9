/********************************************************************************* 

  *FileName: AllocateHierarchy.h 
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef AllocateHierarchy_H_
#define AllocateHierarchy_H_

//-----------------------------------------------------------------------------
// DXDXFRAMEを継承した構造体
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
    D3DXMATRIXA16 CombinedTransformationMatrix;
};


//-----------------------------------------------------------------------------
// D3DXMESHCONTAINERを継承した構造体
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9 * ppTextures;            //テクスチャ
    LPD3DXMESH           pOrigMesh;             //メッシュ
    DWORD                NumInfl;               //頂点ごと影響を受けるボーンの数
    DWORD                NumAttributeGroups;    //サブグリッド数
    LPD3DXBUFFER         pBoneCombinationBuf;   //ボーン結合データ格納
    D3DXMATRIX** ppBoneMatrixPtrs;              //ボーン組み合わせ変換行列
    D3DXMATRIX* pBoneOffsetMatrices;             //ボーン初期変換行列
    DWORD                NumPaletteEntries;     //ボーン最大数
    bool                 UseSoftwareVP;         //ソフトウェア使うかどうか
};

//-----------------------------------------------------------------------------
// xファイルからデータを読み込む用クラス
//-----------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
    HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer);
    HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);

public:
    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ 
        LPCSTR              Name,
        CONST D3DXMESHDATA* pMeshData,
        CONST D3DXMATERIAL* pMaterials,
        CONST D3DXEFFECTINSTANCE* pEffectInstances,
        DWORD                     NumMaterials,
        CONST DWORD* pAdjacency,
        LPD3DXSKININFO pSkinInfo,
        LPD3DXMESHCONTAINER* ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};
#endif // AllocateHierarchy_H_