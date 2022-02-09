/********************************************************************************* 

  *FileName: sceneobject.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/07

**********************************************************************************/  


#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

class SceneObject : public BoundingVolume
{
protected:
	D3DXMATRIX  m_worldMatrix; // World matrix.

private:
	D3DXVECTOR3 m_translation; // Object's translation in 3D space.
	D3DXVECTOR3 m_rotation;    // Object's rotation in radians.

	D3DXMATRIX m_translationMatrix; // Translation matrix.
	D3DXMATRIX m_rotationMatrix;    // Rotation matrix.

	bool	      m_visible;	  // Indicates it the object is visible. Invisible objects are not rendered.
	bool	      m_enabled;	  // Indicates if the object is enabled. Disabled objects are not updated.
	bool	      m_touchingGround;   // Indicates if the object is touching the ground.
	unsigned long m_type;		  // Identifies the scene object's parent class.
	unsigned long m_collisionStamp;   // Indicates the last frame when a collision occurred.
	float		  m_animspeed;

	CD3DXAnimation* m_animation;	//アニメーションオブジェクト
	CAnimInstance* m_animInstance;	//アニメーションコントロールオブジェクト

public:
	SceneObject(unsigned long type, LPCTSTR meshPath);
	virtual ~SceneObject();

	virtual void Update();
	virtual void Render();

	virtual void CollisionOccurred(SceneObject* object, unsigned long collisionStamp);

	void SetTranslation(float x, float y, float z);
	void SetTranslation(D3DXVECTOR3 translation);
	void AddTranslation(float x, float y, float z);
	void AddTranslation(D3DXVECTOR3 translation);
	D3DXVECTOR3* GetTranslation();

	void SetRotation(float x, float y, float z);
	void SetRotation(D3DXVECTOR3 rotation);
	void AddRotation(float x, float y, float z);
	void AddRotation(D3DXVECTOR3 rotation);
	D3DXVECTOR3 GetRotation();

	D3DXMATRIX* GetTranslationMatrix();
	D3DXMATRIX* GetRotationMatrix();
	D3DXMATRIX* GetWorldMatrix();

	void SetType(unsigned long type);
	unsigned long GetType();
	unsigned long GetCollisionStamp();

	void SetSpeed(float speed);
	float GetSpeed();

	void SetVisible(bool visible);
	bool GetVisible();

	void SetEnabled(bool enabled);
	bool GetEnabled();

	void  SetMesh(LPCTSTR meshPath);
	CAnimInstance* GetMesh();
};

#endif // SCENE_OBJECT_H_