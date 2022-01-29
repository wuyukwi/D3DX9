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
	CD3DXAnimation* m_pAnimMesh;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	D3DXMATRIX		m_Matrix;
	float m_fSpeed;
	bool  m_bIsLoop;
public:
	CAnimInstance(void);
	~CAnimInstance(void);

	//初始化一个动画网格实例,获得动画网格资源指针，并拷贝出一个单独的动画控制器
	bool Init(CD3DXAnimation* mesh);

	//绘制实例
	void Render();

	//根据名称播放动画
	bool PlayAnimation(LPCTSTR name, bool isLoop = true);

	//更新动画
	void Update(float delayTime);

	//设置动画速度
	void SetSpeed(float speed);

	//获得动画速度
	float GetSpeed()const { return m_fSpeed; }

	//更新矩阵
	void SetMatrix(const LPD3DXMATRIX matrix);

	//获得矩阵
	D3DMATRIX GetMatrix() { return m_Matrix; }

	void UpdataMatrix(const LPD3DXMATRIX matrix);

	//获得骨骼动画个数
	int GetAnimationNum() const;

	//根据动画编号获得动画集
	LPD3DXANIMATIONSET GetAnimationSet(int index) const;
};



#endif // __ANIMINSTANCE_H_