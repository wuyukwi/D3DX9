/********************************************************************************* 

  *FileName: skybox.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/31

**********************************************************************************/  


#ifndef SKYBOX_H_
#define SKYBOX_H_

class CSkyBox
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;			//设备指针
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;//定点缓冲区指针
	LPDIRECT3DTEXTURE9 m_pTexture[5];		//纹理对象指针
	float m_fLength;						//天空盒宽度
public:
	CSkyBox(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkyBox(void);

	//初始化天空盒
	bool InitSkyBox(float length);

	//加载纹理
	bool InitSkyBoxTexture(LPSTR szFrontTexture);

	//渲染天空盒
	void RenderSkyBox(D3DXMATRIX* matWorld, bool bRenderFrame = false);
};

#endif // SKYBOX_H_