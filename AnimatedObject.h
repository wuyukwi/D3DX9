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
	ID3DXAnimationController* m_animationController; // メッシュアニメーションの再生を管理するためのコントローラー。
	unsigned int		  m_currentTrack;	 // 再生中のアニメーションのトラック。
	float			  m_currentTime;	 // アニメーション用タイマー。
};

#endif