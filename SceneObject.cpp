#include "Engine.h"

//-----------------------------------------------------------------------------
// シーンオブジェクトクラスコンストラクタ。
//-----------------------------------------------------------------------------
SceneObject::SceneObject( unsigned long type, char *meshName, char *meshPath, bool sharedMesh )
{
	// オブジェクトのタイプを設定
	SetType( type );

	// シーンオブジェクトの平行移動と回転をゼロにし
	SetTranslation( 0.0f, 0.0f, 0.0f );
	SetRotation( 0.0f, 0.0f, 0.0f );

	// オブジェクト最初は静止しています
	SetVelocity( 0.0f, 0.0f, 0.0f );
	SetSpin( 0.0f, 0.0f, 0.0f );

	// オブジェクト最初はz軸を向いています。
	m_forward = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	m_right = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );

	// 摩擦力初期化
	m_friction = 0.0f;

	// 衝突スタンプをクリア
	m_collisionStamp = -1;

	// オブジェクトのデフォルト設定、表示され、有効にされ、ソリッドで、当たり判定ある
	m_visible = true;
	m_enabled = true;
	m_ghost = false;
	m_ignoreCollisions = false;

	// オブジェクト最初は地面に触れていない
	m_touchingGround = false;

	// オブジェクトのメッシュを設定
	m_mesh = NULL;
	SetMesh( meshName, meshPath, sharedMesh );
}

//-----------------------------------------------------------------------------
// シーンオブジェクトクラスのデストラクタ。
//-----------------------------------------------------------------------------
SceneObject::~SceneObject()
{
	// オブジェクトのメッシュを破棄します。
	if( m_sharedMesh == true )
		Engine::GetInstance()->GetMeshManager()->Remove( &m_mesh );
	else
		SAFE_DELETE( m_mesh );
}

//-----------------------------------------------------------------------------
// オブジェクトの更新処理
//-----------------------------------------------------------------------------
void SceneObject::Update( float elapsed, bool addVelocity )
{
	// 摩擦力を計算します。
	float friction = 1.0f - m_friction * elapsed;

	// オブジェクトの移動処理
	m_velocity *= friction;
	if( addVelocity == true )
	{
		D3DXVECTOR3 velocity = m_velocity * elapsed;
		AddTranslation( velocity.x, velocity.y, velocity.z );
	}

	// オブジェクトの回転処理
	m_spin *= friction;
	D3DXVECTOR3 spin = m_spin * elapsed;
	AddRotation( spin.x, spin.y, spin.z );

	// オブジェクトのワールドマトリクスを更新します。
	D3DXMatrixMultiply( &m_worldMatrix, &m_rotationMatrix, &m_translationMatrix );

	// オブジェクトのビューマトリックスを作成します。
	D3DXMatrixInverse( &m_viewMatrix, NULL, &m_worldMatrix );

	// オブジェクトの正面方向ベクトルを更新します。
	m_forward.x = (float)sin( m_rotation.y );
	m_forward.y = (float)-tan( m_rotation.x );
	m_forward.z = (float)cos( m_rotation.y );
	D3DXVec3Normalize( &m_forward, &m_forward );

	// オブジェクトの右方向ベクトルを更新します。
	m_right.x = (float)cos( m_rotation.y );
	m_right.y = (float)tan( m_rotation.z );
	m_right.z = (float)-sin( m_rotation.y );
	D3DXVec3Normalize( &m_right, &m_right );

	// 変換行列のみを使用して、オブジェクトのバウンディングボリュームを更新します。
	// これにより、オブジェクトのローカルスペースではなく、
	// ワールドスペース内のオブジェクトの周囲に軸に沿ったバウンディングボックスが維持されます。
	RepositionBoundingVolume( &m_translationMatrix );
}

//-----------------------------------------------------------------------------
// オブジェクトの描画処理
//-----------------------------------------------------------------------------
void SceneObject::Render( D3DXMATRIX *world )
{
	// メッシュがない場合は描画しない
	if( m_mesh == NULL )
		return;

	// オブジェクトのワールド変換行列がオーバーライドされているかどうかを確認します。
	if( world == NULL )
		Engine::GetInstance()->GetDevice()->SetTransform( D3DTS_WORLD, &m_worldMatrix );
	else
		Engine::GetInstance()->GetDevice()->SetTransform( D3DTS_WORLD, world );

	// メッシュの描画処理
	m_mesh->Render();
}

//-----------------------------------------------------------------------------
// 何かがオブジェクトと当たったたときに呼び出されます。
//-----------------------------------------------------------------------------
void SceneObject::CollisionOccurred( SceneObject *object, unsigned long collisionStamp )
{
	// 当たり判定スタンプを設定します。
	m_collisionStamp = collisionStamp;
}

//-----------------------------------------------------------------------------
// 与えられた力をオブジェクトに順方向/逆方向に適用します。
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
// 与えられた力をオブジェクトに右/左方向に適用します。
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
// オブジェクトの移動を停止します。
//-----------------------------------------------------------------------------
void SceneObject::Stop()
{
	m_velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_spin = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

//-----------------------------------------------------------------------------
// オブジェクトの平行移動行列を設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation( float x, float y, float z )
{
	m_translation.x = x;
	m_translation.y = y;
	m_translation.z = z;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// オブジェクトの平行移動行列を設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation( D3DXVECTOR3 translation )
{
	m_translation = translation;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// 平行移動行列の加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation( float x, float y, float z )
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// 平行移動行列の加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation( D3DXVECTOR3 translation )
{
	m_translation += translation;

	D3DXMatrixTranslation( &m_translationMatrix, m_translation.x, m_translation.y, m_translation.z );
}

//-----------------------------------------------------------------------------
// オブジェクトの平行移動ベクトルを返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetTranslation()
{
	return m_translation;
}

//-----------------------------------------------------------------------------
// オブジェクトの回転を設定します。
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
// オブジェクトの回転を設定します。
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
// オブジェクトの現在の回転に、与えられた回転を追加します。
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
// オブジェクトの現在の回転に、与えられた回転を追加します。
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
// オブジェクトの回転値を返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetRotation()
{
	return m_rotation;
}

//-----------------------------------------------------------------------------
// オブジェクトの移動速度を設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetVelocity( float x, float y, float z )
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}

//-----------------------------------------------------------------------------
// オブジェクトの移動速度を設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetVelocity( D3DXVECTOR3 velocity )
{
	m_velocity = velocity;
}

//-----------------------------------------------------------------------------
// オブジェクトの移動速度の加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddVelocity( float x, float y, float z )
{
	m_velocity.x += x;
	m_velocity.y += y;
	m_velocity.z += z;
}

//-----------------------------------------------------------------------------
// オブジェクトの移動速度の加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddVelocity( D3DXVECTOR3 velocity )
{
	m_velocity += velocity;
}

//-----------------------------------------------------------------------------
// オブジェクトの移動速度の移動速度を返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetVelocity()
{
	return m_velocity;
}

//-----------------------------------------------------------------------------
// オブジェクトのスピンを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetSpin( float x, float y, float z )
{
	m_spin.x = x;
	m_spin.y = y;
	m_spin.z = z;
}

//-----------------------------------------------------------------------------
// オブジェクトのスピンを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetSpin( D3DXVECTOR3 spin )
{
	m_spin = spin;
}

//-----------------------------------------------------------------------------
// オブジェクトのスピンの加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddSpin( float x, float y, float z )
{
	m_spin.x += x;
	m_spin.y += y;
	m_spin.z += z;
}

//-----------------------------------------------------------------------------
// オブジェクトのスピンの加算処理
//-----------------------------------------------------------------------------
void SceneObject::AddSpin( D3DXVECTOR3 spin )
{
	m_spin += spin;
}

//-----------------------------------------------------------------------------
// オブジェクトのスピンを返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetSpin()
{
	return m_spin;
}

//-----------------------------------------------------------------------------
// オブジェクトの前方方向ベクトルを返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetForwardVector()
{
	return m_forward;
}

//-----------------------------------------------------------------------------
// オブジェクトの右方向ベクトルを返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetRightVector()
{
	return m_right;
}

//-----------------------------------------------------------------------------
// オブジェクトの現在の変換行列へのポインタを返します。
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetTranslationMatrix()
{
	return &m_translationMatrix;
}

//-----------------------------------------------------------------------------
// オブジェクトの現在の回転行列へのポインタを返します。
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetRotationMatrix()
{
	return &m_rotationMatrix;
}

//-----------------------------------------------------------------------------
// オブジェクトの現在のワールドマトリックスへのポインタを返します。
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetWorldMatrix()
{
	return &m_worldMatrix;
}

//-----------------------------------------------------------------------------
// オブジェクトの現在のビューマトリックスへのポインタを返します。
//-----------------------------------------------------------------------------
D3DXMATRIX *SceneObject::GetViewMatrix()
{
	return &m_viewMatrix;
}

//-----------------------------------------------------------------------------
// オブジェクトのタイプを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetType( unsigned long type )
{
	m_type = type;
}

//-----------------------------------------------------------------------------
// オブジェクトのタイプを返します。
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetType()
{
	return m_type;
}

//-----------------------------------------------------------------------------
// オブジェクトの摩擦力を設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetFriction( float friction )
{
	m_friction = friction;
}

//-----------------------------------------------------------------------------
// 当たり判定のスタンプを返します。
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetCollisionStamp()
{
	return m_collisionStamp;
}

//-----------------------------------------------------------------------------
// オブジェクトの表示フラグを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetVisible( bool visible )
{
	m_visible = visible;
}

//-----------------------------------------------------------------------------
// オブジェクトの可視フラグを返します。
//-----------------------------------------------------------------------------
bool SceneObject::GetVisible()
{
	return m_visible;
}

//-----------------------------------------------------------------------------
// オブジェクトの有効フラグを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

//-----------------------------------------------------------------------------
// オブジェクトの有効フラグを返します。
//-----------------------------------------------------------------------------
bool SceneObject::GetEnabled()
{
	return m_enabled;
}

//-----------------------------------------------------------------------------
// オブジェクトのゴーストフラグを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetGhost( bool ghost )
{
	m_ghost = ghost;
}

//-----------------------------------------------------------------------------
// オブジェクトの有効フラグを返します。
//-----------------------------------------------------------------------------
bool SceneObject::GetGhost()
{
	return m_ghost;
}

//-----------------------------------------------------------------------------
// オブジェクトの衝突無視フラグを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetIgnoreCollisions( bool ignoreCollisions )
{
	m_ignoreCollisions = ignoreCollisions;
}

//-----------------------------------------------------------------------------
// オブジェクトの衝突無視フラグを返します。
//-----------------------------------------------------------------------------
bool SceneObject::GetIgnoreCollisions()
{
	return m_ignoreCollisions;
}

//-----------------------------------------------------------------------------
// オブジェクトの着地フラグを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetTouchingGroundFlag( bool touchingGround )
{
	m_touchingGround = touchingGround;
}

//-----------------------------------------------------------------------------
// オブジェクトの着地フラグを返します。
//-----------------------------------------------------------------------------
bool SceneObject::IsTouchingGround()
{
	return m_touchingGround;
}

//-----------------------------------------------------------------------------
// シーンオブジェクトのメッシュを設定します。
//-----------------------------------------------------------------------------
void SceneObject::SetMesh( char *meshName, char *meshPath, bool sharedMesh )
{
	// オブジェクトの既存のメッシュを破棄します。
	if( m_sharedMesh == true )
		Engine::GetInstance()->GetMeshManager()->Remove( &m_mesh );
	else
		SAFE_DELETE( m_mesh );

	// オブジェクトがこのメッシュを共有しているか
	m_sharedMesh = sharedMesh;

	// メッシュが指定されたことを確認
	if( meshName != NULL && meshPath != NULL )
	{
		// オブジェクトのメッシュを読み込み
		if( m_sharedMesh == true )
			m_mesh = Engine::GetInstance()->GetMeshManager()->Add( meshName, meshPath );
		else
			m_mesh = new Mesh( meshName, meshPath );

		// メッシュのバウンディングボリュームのクローンを作成します
		CloneBoundingVolume( m_mesh->GetBoundingBox(), m_mesh->GetBoundingSphere() );
	}
}

//-----------------------------------------------------------------------------
// オブジェクトのメッシュへのポインタを返します。
//-----------------------------------------------------------------------------
Mesh *SceneObject::GetMesh()
{
	return m_mesh;
}