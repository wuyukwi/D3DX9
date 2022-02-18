/********************************************************************************* 

  *FileName: sceneobject.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/07

**********************************************************************************/  

#include "engine.h"

bool ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_center,
		&sphere->_radius);

	mesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}

bool ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox* box)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max);

	mesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// The scene object class constructor.
//-----------------------------------------------------------------------------
SceneObject::SceneObject(unsigned long type,LPCTSTR meshPath)
{
	// Set the object's type;
	SetType(type);

	// Zero the scene object's translation and rotation.
	SetTranslation(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);

	// The object is visible, enabled, solid, and registering collisons by default.
	m_visible = true;
	m_enabled = true;
	m_hit = false;

	// Initially the object is not touching the ground.
	m_touchingGround = false;

	// Clear the collision stamp.
	m_collisionStamp = -1;

	m_hp = 100;

	// Set the object's mesh.
	SetMesh(meshPath);

}

//-----------------------------------------------------------------------------
// The scene object class destructor.
//-----------------------------------------------------------------------------
SceneObject::~SceneObject()
{
	//SAFE_DELETE(m_animation)
	SAFE_DELETE(m_animInstance);
	//SAFE_DELETE(m_BoxMesh);
	//SAFE_DELETE(m_SphereMesh);
}

//-----------------------------------------------------------------------------
// Updates the object.
//-----------------------------------------------------------------------------
void SceneObject::Update()
{
	D3DXMatrixTranslation(&m_translationMatrix, m_translation.x, m_translation.y, m_translation.z);
	// Update the object's world matrix.
	D3DXMatrixMultiply(&m_worldMatrix, &m_rotationMatrix, &m_translationMatrix);

	m_animInstance->SetMatrix(&m_worldMatrix);
	m_animInstance->Update(m_animspeed);

	//// Update the object's bounding volume using the translation matrix only.
	//// This will maintain an axis aligned bounding box around the object in
	//// world space rather than the object's local space.
	//RepositionBoundingVolume(&m_translationMatrix);
}

//-----------------------------------------------------------------------------
// Renders the object.
//-----------------------------------------------------------------------------
void SceneObject::Render()
{

	//Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &m_worldMatrix);

	// Render the object's mesh.
	m_animInstance->Render();

	//D3DMATERIAL9 blue = BLUE_MTRL;
	//blue.Diffuse.a = 0.10f;

	//Engine::GetInstance()->GetDevice()->SetMaterial(&blue);
	//Engine::GetInstance()->GetDevice()->SetTexture(0, 0);

	//Engine::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//Engine::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//Engine::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	//m_BoxMesh->DrawSubset(0);
	//
	//
	//m_SphereMesh->DrawSubset(0);
	//

	//Engine::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	//Engine::GetInstance()->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}

//-----------------------------------------------------------------------------
// Called when something collides with the object.
//-----------------------------------------------------------------------------
void SceneObject::CollisionOccurred(SceneObject* object, unsigned long collisionStamp)
{
	// Set the collision stamp.
	m_collisionStamp = collisionStamp;
}


//-----------------------------------------------------------------------------
// Sets the object's translation.
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation(float x, float y, float z)
{
	m_translation.x = x;
	m_translation.y = y;
	m_translation.z = z;

	D3DXMatrixTranslation(&m_translationMatrix, m_translation.x, m_translation.y, m_translation.z);
}

//-----------------------------------------------------------------------------
// Sets the object's translation.
//-----------------------------------------------------------------------------
void SceneObject::SetTranslation(D3DXVECTOR3 translation)
{
	m_translation = translation;

	D3DXMatrixTranslation(&m_translationMatrix, m_translation.x, m_translation.y, m_translation.z);
}

//-----------------------------------------------------------------------------
// Adds the given translation to the object's current translation.
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation(float x, float y, float z)
{
	m_translation.x += x;
	m_translation.y += y;
	m_translation.z += z;

	D3DXMatrixTranslation(&m_translationMatrix, m_translation.x, m_translation.y, m_translation.z);
}

//-----------------------------------------------------------------------------
// Adds the given translation to the object's current translation.
//-----------------------------------------------------------------------------
void SceneObject::AddTranslation(D3DXVECTOR3 translation)
{
	m_translation += translation;

	D3DXMatrixTranslation(&m_translationMatrix, m_translation.x, m_translation.y, m_translation.z);
}

//-----------------------------------------------------------------------------
// Returns the object's translation.
//-----------------------------------------------------------------------------
D3DXVECTOR3* SceneObject::GetTranslation()
{
	return &m_translation;
}

//-----------------------------------------------------------------------------
// Sets the object's rotation.
//-----------------------------------------------------------------------------
void SceneObject::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX(&rotationX, m_rotation.x);
	D3DXMatrixRotationY(&rotationY, m_rotation.y);
	D3DXMatrixRotationZ(&m_rotationMatrix, m_rotation.z);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationX);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationY);
}

//-----------------------------------------------------------------------------
// Sets the object's rotation.
//-----------------------------------------------------------------------------
void SceneObject::SetRotation(D3DXVECTOR3 rotation)
{
	m_rotation = rotation;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX(&rotationX, m_rotation.x);
	D3DXMatrixRotationY(&rotationY, m_rotation.y);
	D3DXMatrixRotationZ(&m_rotationMatrix, m_rotation.z);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationX);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationY);
}

//-----------------------------------------------------------------------------
// Adds the given rotation to the object's current rotation.
//-----------------------------------------------------------------------------
void SceneObject::AddRotation(float x, float y, float z)
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX(&rotationX, m_rotation.x);
	D3DXMatrixRotationY(&rotationY, m_rotation.y);
	D3DXMatrixRotationZ(&m_rotationMatrix, m_rotation.z);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationX);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationY);
}

//-----------------------------------------------------------------------------
// Adds the given rotation to the object's current rotation.
//-----------------------------------------------------------------------------
void SceneObject::AddRotation(D3DXVECTOR3 rotation)
{
	m_rotation += rotation;

	D3DXMATRIX rotationX, rotationY;
	D3DXMatrixRotationX(&rotationX, m_rotation.x);
	D3DXMatrixRotationY(&rotationY, m_rotation.y);
	D3DXMatrixRotationZ(&m_rotationMatrix, m_rotation.z);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationX);
	D3DXMatrixMultiply(&m_rotationMatrix, &m_rotationMatrix, &rotationY);
}

//-----------------------------------------------------------------------------
// Returns the object's rotation.
//-----------------------------------------------------------------------------
D3DXVECTOR3 SceneObject::GetRotation()
{
	return m_rotation;
}


//-----------------------------------------------------------------------------
// Returns a pointer to the object's current translation matrix.
//-----------------------------------------------------------------------------
D3DXMATRIX* SceneObject::GetTranslationMatrix()
{
	return &m_translationMatrix;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the object's current rotation matrix.
//-----------------------------------------------------------------------------
D3DXMATRIX* SceneObject::GetRotationMatrix()
{
	return &m_rotationMatrix;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the object's current world matrix.
//-----------------------------------------------------------------------------
D3DXMATRIX* SceneObject::GetWorldMatrix()
{
	return &m_worldMatrix;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the object's current view matrix.
//-----------------------------------------------------------------------------
D3DXMATRIX* SceneObject::GetViewMatrix()
{
	return &m_viewMatrix;
}

//-----------------------------------------------------------------------------
// Sets the object's type.
//-----------------------------------------------------------------------------
void SceneObject::SetType(unsigned long type)
{
	m_type = type;
}

//-----------------------------------------------------------------------------
// Returns the object's type.
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetType()
{
	return m_type;
}


//-----------------------------------------------------------------------------
// Returns the collision stamp.
//-----------------------------------------------------------------------------
unsigned long SceneObject::GetCollisionStamp()
{
	return m_collisionStamp;
}

//-----------------------------------------------------------------------------
// Sets the object's speed.
//-----------------------------------------------------------------------------
void SceneObject::SetSpeed(float speed)
{
	m_animspeed = speed;
}

//-----------------------------------------------------------------------------
// Returns the object's speed.
//-----------------------------------------------------------------------------
float SceneObject::GetSpeed()
{
	return m_animspeed;
}

//-----------------------------------------------------------------------------
// Sets the object's visible flag.
//-----------------------------------------------------------------------------
void SceneObject::SetVisible(bool visible)
{
	m_visible = visible;
}

//-----------------------------------------------------------------------------
// Returns the object's visible flag.
//-----------------------------------------------------------------------------
bool SceneObject::GetVisible()
{
	return m_visible;
}

//-----------------------------------------------------------------------------
// Sets the object's enabled flag.
//-----------------------------------------------------------------------------
void SceneObject::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

//-----------------------------------------------------------------------------
// Returns the object's enabled flag.
//-----------------------------------------------------------------------------
bool SceneObject::GetEnabled()
{
	return m_enabled;
}

//-----------------------------------------------------------------------------
// Returns the object's enabled flag.
//-----------------------------------------------------------------------------
BOOL* SceneObject::SetHit()
{
	return &m_hit;
}

void SceneObject::SetHp(int damege)
{
	m_hp += damege;

}
int SceneObject::GetHp()
{
	return m_hp;
}

//-----------------------------------------------------------------------------
// Sets the mesh for this scene object.
//-----------------------------------------------------------------------------
void SceneObject::SetMesh(LPCTSTR meshPath)
{
	// xファイル読み込み アニメーションオブジェクト作成
	m_animation = new CD3DXAnimation(Engine::GetInstance()->GetDevice());
	m_animation->Init(meshPath);

	m_animInstance = new CAnimInstance();
	m_animInstance->Init(m_animation);
	m_animInstance->SetMatrix(&m_worldMatrix);

	Engine::GetInstance()->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	Engine::GetInstance()->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	ComputeBoundingSphere(m_animation->GetOrigMesh(), &m_boundingSphere);
	ComputeBoundingBox(m_animation->GetOrigMesh(), &m_boundingBox);

	D3DXCreateSphere(
		Engine::GetInstance()->GetDevice(),
		m_boundingSphere._radius,
		20,
		20,
		&m_SphereMesh,
		0);

	D3DXCreateBox(
		Engine::GetInstance()->GetDevice(),
		m_boundingBox._max.x - m_boundingBox._min.x,
		m_boundingBox._max.y - m_boundingBox._min.y,
		m_boundingBox._max.z - m_boundingBox._min.z,
		&m_BoxMesh,
		0);
}

//-----------------------------------------------------------------------------
// Returns a pointer to the object's mesh.
//-----------------------------------------------------------------------------
CAnimInstance* SceneObject::GetMesh()
{
	return m_animInstance;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the box  mesh.
//-----------------------------------------------------------------------------
LPD3DXMESH SceneObject::GetBoxMesh()
{
	return m_BoxMesh;
}

//-----------------------------------------------------------------------------
// Returns a pointer to the spere mesh.
//-----------------------------------------------------------------------------
LPD3DXMESH SceneObject::GetSpereMesh()
{
	return m_SphereMesh;
}