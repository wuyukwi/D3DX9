/********************************************************************************* 

  *FileName: player.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  

#ifndef PLAYER_H_
#define PLAYER_H_

class Player
{
public:
	Player(IDirect3DDevice9* device);
	~Player();

	void Update();
	void Render();

private:
	IDirect3DDevice9*	m_device;		//DX9デバイス
	CD3DXAnimation*		m_animation;	//アニメーションオブジェクト
	CAnimInstance*		m_animInstance;	//アニメーションコントロールオブジェクト
	D3DXMATRIXA16		m_mtxPlayer;		// ワールドマトリックス

	D3DXVECTOR3			m_pos;		// 位置
	D3DXVECTOR3			m_posOld;		// 前回位置
	D3DXVECTOR3			m_move;		// 移動量
	D3DXVECTOR3			m_rot;		// 向き
};



#endif // PLAYER_H_