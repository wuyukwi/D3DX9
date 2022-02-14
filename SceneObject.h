#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H


#define TYPE_SCENE_OBJECT 0


class SceneObject : public BoundingVolume
{
protected:
	D3DXVECTOR3 m_forward;     // 前方方向のベクトル
	D3DXVECTOR3 m_right;	   // 右方向のベクトル

	D3DXMATRIX  m_worldMatrix; // ワールドマトリックス
	D3DXMATRIX  m_viewMatrix;  // ビューマトリックス

private:
	D3DXVECTOR3 m_translation; // 3D空間でのオブジェクトの平行移動。
	D3DXVECTOR3 m_rotation;    // ラジアン単位のオブジェクトの回転。

	D3DXVECTOR3 m_velocity;    // オブジェクトの移動速度 単位/秒
	D3DXVECTOR3 m_spin;	   // オブジェクトのスピン（ラジアン/秒）。

	D3DXMATRIX m_translationMatrix; // 変換行列
	D3DXMATRIX m_rotationMatrix;    // 回転行列

	unsigned long m_type;		  // シーンオブジェクトの親クラスを識別用
	float	      m_friction;	  // オブジェクトの速度とスピンに適用される摩擦力
	unsigned long m_collisionStamp;   // 当たり判定が発生した最後のフレーム
	bool	      m_visible;	  // オブジェクトが表示されているどうか。 非表示のオブジェクトは描画されません。
	bool	      m_enabled;	  // オブジェクトが有効されているどうか。 非表示のオブジェクトは更新されません。
	bool	      m_ghost;		  // オブジェクトがゴーストされているどうか。 ゴーストオブジェクトは当たり判定なし。
	bool	      m_ignoreCollisions; // 当たり判定を無視するかどうか。 当たり判定は引き続き発生する可能性があり、登録されていないだけ
	bool	      m_touchingGround;   // オブジェクトが地面に接触しているかどうか
	bool	      m_sharedMesh;	  // オブジェクトがメッシュを共有しているか
	Mesh*	      m_mesh;		  // オブジェクトのメッシュへのポインタ。

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