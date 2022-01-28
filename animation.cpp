/********************************************************************************* 

  *FileName: animation.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  

#include "engine.h"

CD3DXAnimation::CD3DXAnimation(IDirect3DDevice9* device)
	:m_pDevice(device),
	m_pAllocateHier(NULL),
	m_pAnimController(NULL),
	m_pFrameRoot(NULL)
	// m_BoneMatrix(NULL)
{
}


CD3DXAnimation::~CD3DXAnimation(void)
{
	D3DXFrameDestroy(m_pFrameRoot, m_pAllocateHier);
	SAFE_RELEASE(m_pAnimController);
	SAFE_DELETE(m_pAllocateHier);
}

//--------------------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: 设置好各级框架的组合变换矩阵。
//--------------------------------------------------------------------------------------
HRESULT CD3DXAnimation::SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot)
{
	if (pFrameBase->pMeshContainer != NULL)
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX * [cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: 绘制骨骼
//--------------------------------------------------------------------------------------
void CD3DXAnimation::DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame)
{
	if (pFrame == NULL) return;
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = pFrame->pMeshContainer;                    // 取得网格容器
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pd3dDevice, pMeshContainer, pFrame);  // 绘制非空蒙皮网格
		pMeshContainer = pMeshContainer->pNextMeshContainer;    // 遍历所有网格容器
	}

	DrawFrame(pd3dDevice, pFrame->pFrameSibling);               // 绘制兄弟框架
	DrawFrame(pd3dDevice, pFrame->pFrameFirstChild);            // 绘制子框架
}

//--------------------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: 绘制蒙皮容器中的蒙皮网格
//--------------------------------------------------------------------------------------
void CD3DXAnimation::DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
						pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
	else  // standard mesh, just draw it after setting material properties
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

//--------------------------------------------------------------------------------------
// Name: UpdateFrameMatrics()
// Desc: 更新框架中的变换矩阵
//--------------------------------------------------------------------------------------
void CD3DXAnimation::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	if (pFrameBase == NULL || pParentMatrix == NULL) return;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	// 将当前骨骼的相对于父骨骼的偏移矩阵作累积运算
	D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);

	UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);                              // 更新兄弟骨骼
	UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);   // 更新子骨骼
}

//---------------------------------------------------------
//Name:真正暴露给外部调用的函数
//Desc:关于动画的创建，更新，绘制
//---------------------------------------------------------

bool CD3DXAnimation::Init(LPCTSTR filename)
{
	m_pAllocateHier = new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, m_pDevice, m_pAllocateHier, NULL, &m_pFrameRoot, &m_pAnimController);
	SetupBoneMatrixPointers(m_pFrameRoot, m_pFrameRoot);
	return true;
}

void CD3DXAnimation::Render(const LPD3DXMATRIX matrix)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, matrix);
	UpdateFrameMatrices(m_pFrameRoot, matrix);
	DrawFrame(m_pDevice, m_pFrameRoot);
}


LPD3DXANIMATIONCONTROLLER CD3DXAnimation::CloneAnimCtrl(void)
{
	//克隆一个动画控制器，真正使用的是克隆的动画控制器，而并非原始的动画控制器，原始的动画控制器仅供克隆。
	LPD3DXANIMATIONCONTROLLER pControl = NULL;
	if (FAILED(m_pAnimController->CloneAnimationController(
		m_pAnimController->GetMaxNumAnimationOutputs(),
		m_pAnimController->GetMaxNumAnimationSets(),
		m_pAnimController->GetMaxNumTracks(),
		m_pAnimController->GetMaxNumEvents(),
		&pControl)))
	{
		return NULL;
	}
	return pControl;
}
