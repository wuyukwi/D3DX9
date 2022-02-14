#include "Engine.h"

//-----------------------------------------------------------------------------
// �V�[���I�u�W�F�N�g�N���X�R���X�g���N�^�B
//-----------------------------------------------------------------------------
SceneObject::SceneObject( unsigned long type, char *meshName, char *meshPath, bool sharedMesh )
{
	// �I�u�W�F�N�g�̃^�C�v��ݒ�
	SetType( type );

	// �V�[���I�u�W�F�N�g�̕��s�ړ��Ɖ�]���[���ɂ�
	SetTranslation( 0.0f, 0.0f, 0.0f );
	SetRotation( 0.0f, 0.0f, 0.0f );

	// �I�u�W�F�N�g�ŏ��͐Î~���Ă��܂�
	SetVelocity( 0.0f, 0.0f, 0.0f );
	SetSpin( 0.0f, 0.0f, 0.0f );

	// �I�u�W�F�N�g�ŏ���z���������Ă��܂��B
	m_forward = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	m_right = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );

	// ���C�͏�����
	m_friction = 0.0f;

	// �Փ˃X�^���v���N���A
	m_collisionStamp = -1;

	// �I�u�W�F�N�g�̃f�t�H���g�ݒ�A�\������A�L���ɂ���A�\���b�h�ŁA�����蔻�肠��
	m_visible = true;
	m_enabled = true;
	m_ghost = false;
	m_ignoreCollisions = false;

	// �I�u�W�F�N�g�ŏ��͒n�ʂɐG��Ă��Ȃ�
	m_touchingGround = false;

	// �I�u�W�F�N�g�̃��b�V����ݒ�
	m_mesh = NULL;
	SetMesh( meshName, meshPath, sharedMesh );
}

//-----------------------------------------------------------------------------
// �V�[���I�u�W�F�N�g�N���X�̃f�X�g���N�^�B
//-----------------------------------------------------------------------------
SceneObject::~SceneObject()
{
	// �I�u�W�F�N�g�̃��b�V����j�����܂��B
	if( m_sharedMesh == true )
		Engine::GetInstance()->GetMeshManager()->Remove( &m_mesh );
	else
		SAFE_DELETE( m_mesh );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------------------
void SceneObject::Update( float elapsed, bool addVelocity )
{
	// ���C�͂��v�Z���܂��B
	float friction = 1.0f - m_friction * elapsed;

	// �I�u�W�F�N�g�̈ړ�����
	m_velocity *= friction;
	if( addVelocity == true )
	{
		D3DXVECTOR3 velocity = m_velocity * elapsed;
		AddTranslation( velocity.x, velocity.y, velocity.z );
	}

	// �I�u�W�F�N�g�̉�]����
	m_spin *= friction;
	D3DXVECTOR3 spin = m_spin * elapsed;
	AddRotation( spin.x, spin.y, spin.z );

	// �I�u�W�F�N�g�̃��[���h�}�g���N�X���X�V���܂��B
	D3DXMatrixMultiply( &m_worldMatrix, &m_rotationMatrix, &m_translationMatrix );

	// �I�u�W�F�N�g�̃r���[�}�g���b�N�X���쐬���܂��B
	D3DXMatrixInverse( &m_viewMatrix, NULL, &m_worldMatrix );

	// �I�u�W�F�N�g�̐��ʕ����x�N�g�����X�V���܂��B
	m_forward.x = (float)sin( m_rotation.y );
	m_forward.y = (float)-tan( m_rotation.x );
	m_forward.z = (float)cos( m_rotation.y );
	D3DXVec3Normalize( &m_forward, &m_forward );

	// �I�u�W�F�N�g�̉E�����x�N�g�����X�V���܂��B
	m_right.x = (float)cos( m_rotation.y );
	m_right.y = (float)tan( m_rotation.z );
	m_right.z = (float)-sin( m_rotation.y );
	D3DXVec3Normalize( &m_right, &m_right );

	// �ϊ��s��݂̂��g�p���āA�I�u�W�F�N�g�̃o�E���f�B���O�{�����[�����X�V���܂��B
	// ����ɂ��A�I�u�W�F�N�g�̃��[�J���X�y�[�X�ł͂Ȃ��A
	// ���[���h�X�y�[�X���̃I�u�W�F�N�g�̎��͂Ɏ��ɉ������o�E���f�B���O�{�b�N�X���ێ�����܂��B
	RepositionBoundingVolume( &m_translationMatrix );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̕`�揈��
//-----------------------------------------------------------------------------
void SceneObject::Render( D3DXMATRIX *world )
{
	// ���b�V�����Ȃ��ꍇ�͕`�悵�Ȃ�
	if( m_mesh == NULL )
		return;

	// �I�u�W�F�N�g�̃��[���h�ϊ��s�񂪃I�[�o�[���C�h����Ă��邩�ǂ������m�F���܂��B
	if( world == NULL )
		Engine::GetInstance()->GetDevice()->SetTransform( D3DTS_WORLD, &m_worldMatrix );
	else
		Engine::GetInstance()->GetDevice()->SetTransform( D3DTS_WORLD, world );

	// ���b�V���̕`�揈��
	m_mesh->Render();
}

//-----------------------------------------------------------------------------
// �������I�u�W�F�N�g�Ɠ����������Ƃ��ɌĂяo����܂��B
//-----------------------------------------------------------------------------
void SceneObject::CollisionOccurred( SceneObject *object, unsigned long collisionStamp )
{
	// �����蔻��X�^���v��ݒ肵�܂��B
	m_collisionStamp = collisionStamp;
}

//-----------------------------------------------------------------------------
// �^����ꂽ�͂��I�u�W�F�N�g�ɏ�����/�t�����ɓK�p���܂��B
//-----------------------------------------------------------------------------
void SceneObject::Drive( float force, bool lockYAxis )
{
	D3DXVECTOR3 realForce = m_forward * force;

	m_velocity.x += realForce.x;
	m_velocity.z += realForce.z;

	if( lockYAxis == false )
		m_velocity.y += realForce.y;
}

//-----------------------------------------------------------------------------
// �^����ꂽ�͂��I�u�W�F�N�g�ɉE/�������ɓK�p���܂��B
//-----------------------------------------------------------------------------
void SceneObject::Strafe( float force, bool lockYAxis )
{
	D3DXVECTOR3 realForce = m_right * force;

	m_velocity.x += realForce.x;
	m_velocity.z += realForce.z;

	if( lockYAxis == false )
		m_velocity.y += realForce.y;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����~���܂��B
//-----------------------------------------------------------------------------
void SceneObject::Stop()
{
	m_velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_spin = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̕��s�ړ��s���ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation( float x, float y, float z )
{
	m_translation.x = x;
	m_translation.y = y;
	m_translation.z = z;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̕��s�ړ��s���ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation( D3DXVECTOR3 translation )
{
	m_translation = translation;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// ���s�ړ��s��̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation( float x, float y, float z )
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// ���s�ړ��s��̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation( D3DXVECTOR3 translation )
{
	m_translation += translation;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̕��s�ړ��x�N�g����Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetTranslation()
{
	return m_translation;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̉�]��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetRotation( float x, float y, float z )
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX( &rotationX, m_rotation.x );
	D3DXMatrixRotationY( &rotationY, m_rotation.y );
	D3DXMatrixRotationZ( &m_rotationMatrix, m_rotation.z );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationX );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationY );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̉�]��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetRotation( D3DXVECTOR3 rotation )
{
	m_rotation = rotation;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX( &rotationX, m_rotation.x );
	D3DXMatrixRotationY( &rotationY, m_rotation.y );
	D3DXMatrixRotationZ( &m_rotationMatrix, m_rotation.z );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationX );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationY );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̉�]�ɁA�^����ꂽ��]��ǉ����܂��B
//-----------------------------------------------------------------------------
void SceneObject::AddRotation( float x, float y, float z )
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX( &rotationX, m_rotation.x );
	D3DXMatrixRotationY( &rotationY, m_rotation.y );
	D3DXMatrixRotationZ( &m_rotationMatrix, m_rotation.z );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationX );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationY );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̉�]�ɁA�^����ꂽ��]��ǉ����܂��B
//-----------------------------------------------------------------------------
void SceneObject::AddRotation( D3DXVECTOR3 rotation )
{
	m_rotation += rotation;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX( &rotationX, m_rotation.x );
	D3DXMatrixRotationY( &rotationY, m_rotation.y );
	D3DXMatrixRotationZ( &m_rotationMatrix, m_rotation.z );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationX );
	D3DXMatrixMultiply( &m_rotationMatrix, &m_rotationMatrix, &rotationY );
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̉�]�l��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetRotation()
{
	return m_rotation;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����x��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetVelocity( float x, float y, float z )
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����x��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetVelocity( D3DXVECTOR3 velocity )
{
	m_velocity = velocity;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����x�̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddVelocity( float x, float y, float z )
{
	m_velocity.x += x;
	m_velocity.y += y;
	m_velocity.z += z;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����x�̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddVelocity( D3DXVECTOR3 velocity )
{
	m_velocity += velocity;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̈ړ����x�̈ړ����x��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetVelocity()
{
	return m_velocity;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃X�s����ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetSpin( float x, float y, float z )
{
	m_spin.x = x;
	m_spin.y = y;
	m_spin.z = z;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃X�s����ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetSpin( D3DXVECTOR3 spin )
{
	m_spin = spin;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃X�s���̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddSpin( float x, float y, float z )
{
	m_spin.x += x;
	m_spin.y += y;
	m_spin.z += z;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃X�s���̉��Z����
//-----------------------------------------------------------------------------
void SceneObject::AddSpin( D3DXVECTOR3 spin )
{
	m_spin += spin;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃X�s����Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetSpin()
{
	return m_spin;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̑O�������x�N�g����Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetForwardVector()
{
	return m_forward;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̉E�����x�N�g����Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetRightVector()
{
	return m_right;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̕ϊ��s��ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetTranslationMatrix()
{
	return &m_translationMatrix;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̉�]�s��ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetRotationMatrix()
{
	return &m_rotationMatrix;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̃��[���h�}�g���b�N�X�ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetWorldMatrix()
{
	return &m_worldMatrix;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̌��݂̃r���[�}�g���b�N�X�ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetViewMatrix()
{
	return &m_viewMatrix;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃^�C�v��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetType( unsigned long type )
{
	m_type = type;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃^�C�v��Ԃ��܂��B
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetType()
{
	return m_type;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̖��C�͂�ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetFriction( float friction )
{
	m_friction = friction;
}

//-----------------------------------------------------------------------------
// �����蔻��̃X�^���v��Ԃ��܂��B
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetCollisionStamp()
{
	return m_collisionStamp;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̕\���t���O��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetVisible( bool visible )
{
	m_visible = visible;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̉��t���O��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool SceneObject::GetVisible()
{
	return m_visible;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̗L���t���O��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̗L���t���O��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool SceneObject::GetEnabled()
{
	return m_enabled;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃S�[�X�g�t���O��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetGhost( bool ghost )
{
	m_ghost = ghost;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̗L���t���O��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool SceneObject::GetGhost()
{
	return m_ghost;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̏Փ˖����t���O��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetIgnoreCollisions( bool ignoreCollisions )
{
	m_ignoreCollisions = ignoreCollisions;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̏Փ˖����t���O��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool SceneObject::GetIgnoreCollisions()
{
	return m_ignoreCollisions;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̒��n�t���O��ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetTouchingGroundFlag( bool touchingGround )
{
	m_touchingGround = touchingGround;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̒��n�t���O��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool SceneObject::IsTouchingGround()
{
	return m_touchingGround;
}

//-----------------------------------------------------------------------------
// �V�[���I�u�W�F�N�g�̃��b�V����ݒ肵�܂��B
//-----------------------------------------------------------------------------
void SceneObject::SetMesh( char *meshName, char *meshPath, bool sharedMesh )
{
	// �I�u�W�F�N�g�̊����̃��b�V����j�����܂��B
	if( m_sharedMesh == true )
		Engine::GetInstance()->GetMeshManager()->Remove( &m_mesh );
	else
		SAFE_DELETE( m_mesh );

	// �I�u�W�F�N�g�����̃��b�V�������L���Ă��邩
	m_sharedMesh = sharedMesh;

	// ���b�V�����w�肳�ꂽ���Ƃ��m�F
	if( meshName != NULL && meshPath != NULL )
	{
		// �I�u�W�F�N�g�̃��b�V����ǂݍ���
		if( m_sharedMesh == true )
			m_mesh = Engine::GetInstance()->GetMeshManager()->Add( meshName, meshPath );
		else
			m_mesh = new Mesh( meshName, meshPath );

		// ���b�V���̃o�E���f�B���O�{�����[���̃N���[�����쐬���܂�
		CloneBoundingVolume( m_mesh->GetBoundingBox(), m_mesh->GetBoundingSphere() );
	}
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��b�V���ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
Mesh *SceneObject::GetMesh()
{
	return m_mesh;
}