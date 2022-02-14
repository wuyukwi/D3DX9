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
		return D3DXVECTOR3( finalTransformationMatrix._41, finalTransformationMatrix._42, finalTransformationMatrix._43 );
	}
};


struct MeshContainer : public D3DXMESHCONTAINER
{
	char**	            materialNames;	  // Temporary array of material (texture) names.
	Material**	    materials;		  // Array of materials used by the mesh container.
	ID3DXMesh*	    originalMesh;	  // Original mesh.
	D3DXATTRIBUTERANGE* attributeTable;	  // Mesh's attribute table.
	unsigned long	    totalAttributeGroups; // Total number of attribute groups.
	D3DXMATRIX**	    boneMatrixPointers;   // Array of pointers to the bone transformation matrices.
};


//-----------------------------------------------------------------------------
// Allocate Hierarchy Class
//-----------------------------------------------------------------------------
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
	STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
	STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer );
	STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
	STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
};


//-----------------------------------------------------------------------------
// ���b�V���N���X
//-----------------------------------------------------------------------------
class Mesh : public BoundingVolume, public Resource< Mesh >
{
private:
	Frame*			  m_firstFrame;		 // ���b�V���̃t���[���K�w�̍ŏ��̃t���[���B
	ID3DXAnimationController* m_animationController; // �A�j���[�V�����R���g���[���[

	D3DXMATRIX*   m_boneMatrices;	   // �{�[���ϊ��s��̔z��B
	unsigned long m_totalBoneMatrices; // �z����̃{�[���̐��B

	MeshContainer*  m_staticMesh; // ���b�V���̐ÓI�i�A�j���[�V����������Ă��Ȃ��j�o�[�W�����B
	Vertex*		m_vertices;   // �ÓI���b�V������̒��_�̔z��B
	unsigned short* m_indices;    // ���_�z��ւ̃C���f�b�N�X�̔z��B

	LinkedList<Frame>* m_frames;    // ���b�V�����̂��ׂẴt���[���ւ̃|�C���^�̃����N���X�g�B
	LinkedList<Frame>* m_refPoints; // ���b�V�����̂��ׂĂ̎Q�ƃ|�C���g�ւ̃|�C���^�̃����N���X�g�B

public:
	Mesh( char *name, char *path = "./" );
	virtual ~Mesh();

	void Update();
	void Render();

	void CloneAnimationController( ID3DXAnimationController **animationController );

	MeshContainer*  GetStaticMesh();
	Vertex*		GetVertices();
	unsigned short* GetIndices();

	LinkedList<Frame>* GetFrameList();
	Frame*		   GetFrame( char *name );
	Frame*		   GetReferencePoint( char *name );

private:
	void PrepareFrame( Frame *frame );
	void UpdateFrame( Frame *frame, D3DXMATRIX *parentTransformationMatrix = NULL );
	void RenderFrame( Frame *frame );
};

#endif