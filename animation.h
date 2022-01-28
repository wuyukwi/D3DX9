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
	IDirect3DDevice9*	m_pDevice;					//D3D设备对象
	CAllocateHierarchy* m_pAllocateHier;			//骨骼动画网格模型指针
	LPD3DXFRAME			m_pFrameRoot;				//帧
	LPD3DXANIMATIONCONTROLLER m_pAnimController;	//动画控制器
private:
	//一些微软自带函数，关于骨骼动画加载与绘制更新的函数，将其封装，不使用这些接口

	void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
public:
	CD3DXAnimation(IDirect3DDevice9* device);
	~CD3DXAnimation(void);

	//提供给外界的接口

	//创建骨骼动画
	bool Init(LPCTSTR filename);

	//复制骨骼动画控制器
	LPD3DXANIMATIONCONTROLLER CloneAnimCtrl(void);

	//绘制骨骼动画
	void Render(const LPD3DXMATRIX matrix);

};


#endif // ANIMATION_H_