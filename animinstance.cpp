/********************************************************************************* 

  *FileName: .cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  
#include "engine.h"

CAnimInstance::CAnimInstance(void)
{
	
}


CAnimInstance::~CAnimInstance(void)
{
}

bool CAnimInstance::Init(CD3DXAnimation* mesh)
{
	m_pAnimMesh = mesh;
	return m_pAnimController = mesh->CloneAnimCtrl();
}

void CAnimInstance::Render()
{
	m_pAnimMesh->Render(&m_MatWorld);
}

bool CAnimInstance::PlayAnimation(LPCTSTR name, bool isLoop)
{
	LPD3DXANIMATIONSET pAnimationSet = NULL;
	m_pAnimController->GetAnimationSetByName((LPCSTR)name, &pAnimationSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimationSet);
	return true;
}

void CAnimInstance::Update(float delayTime)
{
	m_pAnimController->AdvanceTime(delayTime, NULL);
}

void CAnimInstance::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

void CAnimInstance::SetMatrix(const LPD3DXMATRIX matrix)
{
	m_MatWorld = *matrix;
}

void CAnimInstance::UpdataMatrix(const LPD3DXMATRIX matrix)
{
	m_MatWorld *= *matrix;
}



int CAnimInstance::GetAnimationNum() const
{
	return m_pAnimController->GetMaxNumAnimationSets();
}

LPD3DXANIMATIONSET CAnimInstance::GetAnimationSet(int index) const
{
	if (index >= 0 && index < GetAnimationNum())
	{
		LPD3DXANIMATIONSET pAnimSet = NULL;
		m_pAnimController->GetAnimationSet(index, &pAnimSet);
		return pAnimSet;
	}
	return NULL;
}
