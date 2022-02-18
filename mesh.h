/********************************************************************************* 

  *FileName: .h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  


#ifndef MESH_H
#define MESH_H

//-----------------------------------------------------------------------------
// Frame Structure
//-----------------------------------------------------------------------------
struct Frame : public D3DXFRAME
{
	D3DXMATRIX finalTransformationMatrix; // Frame's final transformation after being combined with its parent.

	D3DXVECTOR3 GetTranslation()
	{
		return D3DXVECTOR3(finalTransformationMatrix._41, finalTransformationMatrix._42, finalTransformationMatrix._43);
	}
};


struct MeshContainer : public D3DXMESHCONTAINER
{
	char** materialNames;	  // Temporary array of material (texture) names.
	Material** materials;		  // Array of materials used by the mesh container.
	ID3DXMesh* originalMesh;	  // Original mesh.
	D3DXATTRIBUTERANGE* attributeTable;	  // Mesh's attribute table.
	unsigned long	    totalAttributeGroups; // Total number of attribute groups.
	D3DXMATRIX** boneMatrixPointers;   // Array of pointers to the bone transformation matrices.
};


//-----------------------------------------------------------------------------
// Allocate Hierarchy Class
//-----------------------------------------------------------------------------
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA* pMeshData, CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
};


//-----------------------------------------------------------------------------
// メッシュクラス
//-----------------------------------------------------------------------------
class Mesh : public BoundingVolume, public Resource< Mesh >
{
private:
	Frame* m_firstFrame;		 // メッシュのフレーム階層の最初のフレーム。
	ID3DXAnimationController* m_animationController; // アニメーションコントローラー

	D3DXMATRIX* m_boneMatrices;	   // ボーン変換行列の配列。
	unsigned long m_totalBoneMatrices; // 配列内のボーンの数。

	MeshContainer* m_staticMesh; // メッシュの静的（アニメーション化されていない）バージョン。
	Vertex* m_vertices;   // 静的メッシュからの頂点の配列。
	unsigned short* m_indices;    // 頂点配列へのインデックスの配列。

	LinkedList<Frame>* m_frames;    // メッシュ内のすべてのフレームへのポインタのリンクリスト。
	LinkedList<Frame>* m_refPoints; // メッシュ内のすべての参照ポイントへのポインタのリンクリスト。

public:
	Mesh(char* name, char* path = "./");
	virtual ~Mesh();

	void Update();
	void Render();

	void CloneAnimationController(ID3DXAnimationController** animationController);

	MeshContainer* GetStaticMesh();
	Vertex* GetVertices();
	unsigned short* GetIndices();

	LinkedList<Frame>* GetFrameList();
	Frame* GetFrame(char* name);
	Frame* GetReferencePoint(char* name);

private:
	void PrepareFrame(Frame* frame);
	void UpdateFrame(Frame* frame, D3DXMATRIX* parentTransformationMatrix = NULL);
	void RenderFrame(Frame* frame);
};

#endif // _H_