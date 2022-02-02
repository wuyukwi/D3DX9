/********************************************************************************* 

  *FileName: skybox.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/31

**********************************************************************************/  


#ifndef SKYBOX_H_
#define SKYBOX_H_

class Skybox
{
private:
    
    IDirect3DDevice9* m_device;
    LPDIRECT3DVERTEXBUFFER9 m_vb;
    LPDIRECT3DTEXTURE9 m_tex;



public:
    Skybox(float skyboxSize, IDirect3DDevice9* device);

    ~Skybox();

    void Render();
};

#endif // SKYBOX_H_