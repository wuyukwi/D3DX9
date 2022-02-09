/********************************************************************************* 

  *FileName: scene.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef SCENE_H_
#define SCENE_H_

//-----------------------------------------------------------------------------
// Scene Class
//-----------------------------------------------------------------------------
class Scene
{
private:
	IDirect3DDevice9* m_device;
	LPDIRECT3DVERTEXBUFFER9 m_buf;
	LPDIRECT3DTEXTURE9 m_tex;
	D3DXMATRIX m_matGround;

	D3DXVECTOR3 m_vPosGround;
	//D3DXVECTOR3 m_posPlayer;
	
	Skybox* m_skybox;
	Player* m_player;
	Enemy* m_enemy;

public:
	Scene(IDirect3DDevice9* device);
	 ~Scene();

	void Update();
	void Render();

	bool RayIntersectScene(D3DXVECTOR3 rayPosition, D3DXVECTOR3 rayDirection, bool checkScene, SceneObject* thisObject, bool checkObjects);
};


#endif // SCENE_H_