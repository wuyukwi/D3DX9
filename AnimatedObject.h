//------------------------------------------------
// Animated Object class
//------------------------------------------------
#ifndef ANIMATED_OBJECT_H
#define ANIMATED_OBJECT_H

#define TYPE_ANIMATED_OBJECT 1


class AnimatedObject : public SceneObject, public ID3DXAnimationCallbackHandler
{
public:
	AnimatedObject(char *meshName, char *meshPath = "./", unsigned long type = TYPE_ANIMATED_OBJECT);
	virtual ~AnimatedObject();

	virtual void		  Update(float elapsed, bool addVelocity = true);

	void			  PlayAnimation(unsigned int animation, float transitionTime, bool loop = true);
	ID3DXAnimationController* GetAnimationController();

private:
	virtual HRESULT CALLBACK  HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);

private:
	ID3DXAnimationController* m_animationController; // ���b�V���A�j���[�V�����̍Đ����Ǘ����邽�߂̃R���g���[���[�B
	unsigned int		  m_currentTrack;	 // �Đ����̃A�j���[�V�����̃g���b�N�B
	float			  m_currentTime;	 // �A�j���[�V�����p�^�C�}�[�B
};

#endif