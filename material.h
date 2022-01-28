/********************************************************************************* 

  *FileName: material.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef MATERIAL_H_
#define MATERIAL_H_

class Material : public Resource< Material >
{
private:
	IDirect3DTexture9* m_texture;
	D3DMATERIAL9	   m_lighting;
	unsigned long	   m_width;
	unsigned long	   m_height;
	bool		   m_ignoreFace;
	bool		   m_ignoreFog;
	bool		   m_ignoreRay;

public:
	Material();
	Material(char* name, char* path = "./");
	virtual ~Material();

	IDirect3DTexture9* GetTexture();
	D3DMATERIAL9* GetLighting();
	unsigned long	   GetWidth();
	unsigned long	   GetHeight();
	bool		   GetIgnoreFace();
	bool		   GetIgnoreFog();
	bool		   GetIgnoreRay();
};


#endif // MATERIAL_H_