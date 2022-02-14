#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H


#define TYPE_SCENE_OBJECT 0


class SceneObject : public BoundingVolume
{
protected:
	D3DXVECTOR3 m_forward;     // �O�������̃x�N�g��
	D3DXVECTOR3 m_right;	   // �E�����̃x�N�g��

	D3DXMATRIX  m_worldMatrix; // ���[���h�}�g���b�N�X
	D3DXMATRIX  m_viewMatrix;  // �r���[�}�g���b�N�X

private:
	D3DXVECTOR3 m_translation; // 3D��Ԃł̃I�u�W�F�N�g�̕��s�ړ��B
	D3DXVECTOR3 m_rotation;    // ���W�A���P�ʂ̃I�u�W�F�N�g�̉�]�B

	D3DXVECTOR3 m_velocity;    // �I�u�W�F�N�g�̈ړ����x �P��/�b
	D3DXVECTOR3 m_spin;	   // �I�u�W�F�N�g�̃X�s���i���W�A��/�b�j�B

	D3DXMATRIX m_translationMatrix; // �ϊ��s��
	D3DXMATRIX m_rotationMatrix;    // ��]�s��

	unsigned long m_type;		  // �V�[���I�u�W�F�N�g�̐e�N���X�����ʗp
	float	      m_friction;	  // �I�u�W�F�N�g�̑��x�ƃX�s���ɓK�p����門�C��
	unsigned long m_collisionStamp;   // �����蔻�肪���������Ō�̃t���[��
	bool	      m_visible;	  // �I�u�W�F�N�g���\������Ă���ǂ����B ��\���̃I�u�W�F�N�g�͕`�悳��܂���B
	bool	      m_enabled;	  // �I�u�W�F�N�g���L������Ă���ǂ����B ��\���̃I�u�W�F�N�g�͍X�V����܂���B
	bool	      m_ghost;		  // �I�u�W�F�N�g���S�[�X�g����Ă���ǂ����B �S�[�X�g�I�u�W�F�N�g�͓����蔻��Ȃ��B
	bool	      m_ignoreCollisions; // �����蔻��𖳎����邩�ǂ����B �����蔻��͈���������������\��������A�o�^����Ă��Ȃ�����
	bool	      m_touchingGround;   // �I�u�W�F�N�g���n�ʂɐڐG���Ă��邩�ǂ���
	bool	      m_sharedMesh;	  // �I�u�W�F�N�g�����b�V�������L���Ă��邩
	Mesh*	      m_mesh;		  // �I�u�W�F�N�g�̃��b�V���ւ̃|�C���^�B

public:
	SceneObject( unsigned long type = TYPE_SCENE_OBJECT, char *meshName = NULL, char *meshPath = "./", bool sharedMesh = true );
	virtual ~SceneObject();

	virtual void Update( float elapsed, bool addVelocity = true );
	virtual void Render( D3DXMATRIX *world = NULL );

	virtual void CollisionOccurred( SceneObject *object, unsigned long collisionStamp );

	void Drive( float force, bool lockYAxis = true );
	void Strafe( float force, bool lockYAxis = true );
	void Stop();

	void SetTranslation( float x, float y, float z );
	void SetTranslation( D3DXVECTOR3 translation );
	void AddTranslation( float x, float y, float z );
	void AddTranslation( D3DXVECTOR3 translation );
	D3DXVECTOR3 GetTranslation();

	void SetRotation( float x, float y, float z );
	void SetRotation( D3DXVECTOR3 rotation );
	void AddRotation( float x, float y, float z );
	void AddRotation( D3DXVECTOR3 rotation );
	D3DXVECTOR3 GetRotation();

	void SetVelocity( float x, float y, float z );
	void SetVelocity( D3DXVECTOR3 velocity );
	void AddVelocity( float x, float y, float z );
	void AddVelocity( D3DXVECTOR3 velocity );
	D3DXVECTOR3 GetVelocity();

	void SetSpin( float x, float y, float z );
	void SetSpin( D3DXVECTOR3 spin );
	void AddSpin( float x, float y, float z );
	void AddSpin( D3DXVECTOR3 spin );
	D3DXVECTOR3 GetSpin();

	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetRightVector();

	D3DXMATRIX* GetTranslationMatrix();
	D3DXMATRIX* GetRotationMatrix();
	D3DXMATRIX* GetWorldMatrix();
	D3DXMATRIX* GetViewMatrix();

	void SetType( unsigned long type );
	unsigned long GetType();

	void SetFriction( float friction );

	unsigned long GetCollisionStamp();

	void SetVisible( bool visible );
	bool GetVisible();

	void SetEnabled( bool enabled );
	bool GetEnabled();

	void SetGhost( bool ghost );
	bool GetGhost();

	void SetIgnoreCollisions( bool ignoreCollisions );
	bool GetIgnoreCollisions();

	void SetTouchingGroundFlag( bool touchingGround );
	bool IsTouchingGround();

	void  SetMesh( char *meshName = NULL, char *meshPath = "./", bool sharedMesh = true );
	Mesh *GetMesh();
};

#endif