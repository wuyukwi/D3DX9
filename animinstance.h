/********************************************************************************* 

  *FileName: animinstance.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef __ANIMINSTANCE_H_
#define __ANIMINSTANCE_H_

class CAnimInstance
{
private:
	CD3DXAnimation*				m_pAnimMesh;		// アニメーションへポインター
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;	// アニメーションへコントローラ
	D3DXMATRIX					m_MatWorld;			// アニメーションワールドマトリクス
	float						m_fSpeed;			// アニメーション再生速度
	bool						m_bIsLoop;			// ループ用
public:
	CAnimInstance(void);
	~CAnimInstance(void);

	// 初期化して、メッシュへのポインター獲得、アニメーションコントローラーコピー
	bool Init(CD3DXAnimation* mesh);

	// アニメーション描画
	void Render();

	// アニメーションネームよりアニメーション再生
	bool PlayAnimation(LPCTSTR name, bool isLoop = true);

	// アニメーション更新
	void Update(float delayTime);

	// アニメーションスピード設定
	void SetSpeed(float speed);

	// アニメーションスピード獲得
	float GetSpeed()const { return m_fSpeed; }

	// マトリクス設定
	void SetMatrix(const LPD3DXMATRIX matrix);

	// マトリクス獲得
	const D3DMATRIX& GetMatrix() const { return m_MatWorld; }

	void UpdataMatrix(const LPD3DXMATRIX matrix);
	
	// メッシュ情報の取得
	LPD3DXMESH GetOrigMesh()const { return m_pAnimMesh->GetOrigMesh(); }

	// ボーンのアニメーション個数獲得
	int GetAnimationNum() const;

	// アニメーションインデックスによりID3DXAnimationSet獲得
	LPD3DXANIMATIONSET GetAnimationSet(int index) const;
};



#endif // __ANIMINSTANCE_H_