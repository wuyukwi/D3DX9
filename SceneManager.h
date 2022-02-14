#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

//-----------------------------------------------------------------------------
// Scene Occluder Structure
//-----------------------------------------------------------------------------
struct SceneOccluder : public BoundingVolume
{
	unsigned long	       visibleStamp; // オクルージョンが現在のフレームに表示されているかどうかを示すスタンプ。
	D3DXVECTOR3	       translation;  // オクルージョンの平行変換行列。
	unsigned long	       totalFaces;   // オクルージョンのメッシュ内の面の総数。
	Vertex*		       vertices;     // オクルージョンの頂点を含む配列がワールドスペースに変換されます。
	unsigned short*	       indices;	     // 頂点配列へのインデックスの配列。
	LinkedList<D3DXPLANE>* planes;	     // 遮蔽されたバウンディングボリュームを定義する平面のリスト。
	float		       distance;     // 視点とオクルージョンの間の距離。

	SceneOccluder( D3DXVECTOR3 t, ID3DXMesh *mesh, D3DXMATRIX *world )
	{
		// Clear the visible stamp.
		visibleStamp = -1;

		// Set the translation.
		translation = t;

		// 面の合計を設定し、頂点情報およびインデックス配列を作成します。
		totalFaces = mesh->GetNumFaces();
		vertices = new Vertex[mesh->GetNumVertices()];
		indices = new unsigned short[totalFaces * 3];

		// 指定されたメッシュの頂点バッファとインデックスバッファをロックします。
		Vertex* verticesPtr;
		mesh->LockVertexBuffer( 0, (void**)&verticesPtr );
		unsigned short *indicesPtr;
		mesh->LockIndexBuffer( 0, (void**)&indicesPtr );

		// 頂点とインデックスをコピーします。
		memcpy( vertices, verticesPtr, VERTEX_FVF_SIZE * mesh->GetNumVertices() );
		memcpy( indices, indicesPtr, sizeof( unsigned short ) * totalFaces * 3 );

		// 頂点バッファとインデックスバッファのロックを解除します。
		mesh->UnlockVertexBuffer();
		mesh->UnlockIndexBuffer();

		// 頂点をワールドスペースに変換します。
		for( unsigned long v = 0; v < mesh->GetNumVertices(); v++ )
			D3DXVec3TransformCoord( &vertices[v].translation, &vertices[v].translation, world );

		// オクルージョンを構築するための平面のリストを作成します。
		planes = new LinkedList< D3DXPLANE >;

		// オクルージョンをのメッシュからバウンディングボリュームを作成します。
		BoundingVolumeFromMesh( mesh );

		// バウンディングボリュームをワールドスペースに再配置します。
		D3DXMATRIX location;
		D3DXMatrixTranslation( &location, t.x, t.y, t.z );
		RepositionBoundingVolume( &location );
	}

	virtual ~SceneOccluder()
	{
		SAFE_DELETE_ARRAY( vertices );
		SAFE_DELETE_ARRAY( indices );

		SAFE_DELETE( planes );
	}
};


//-----------------------------------------------------------------------------
// Scene Leaf Structure
//-----------------------------------------------------------------------------
struct SceneLeaf : public BoundingVolume
{
	SceneLeaf*		   children[8];  // Array of child scene leaf pointers.
	unsigned long		   visibleStamp; // Indicates if the scene leaf is visible in the current frame.
	LinkedList<SceneOccluder>* occluders;    // list of scene occluders in the scene leaf.
	unsigned long		   totalFaces;   // Total number of faces in the scene leaf.
	unsigned long*		   faces;	 // Array of indices pointing to the faces in the scene leaf.

	SceneLeaf()
	{
		for( char l = 0; l < 8; l++ )
			children[l] = NULL;
		occluders = new LinkedList< SceneOccluder >;
		totalFaces = 0;
		faces = NULL;
	}

	virtual ~SceneLeaf()
	{
		for( char l = 0; l < 8; l++ )
			SAFE_DELETE( children[l] );
		occluders->ClearPointers();
		SAFE_DELETE( occluders );
		SAFE_DELETE_ARRAY( faces );
	}
};


//-----------------------------------------------------------------------------
// Scene Face Structure
//-----------------------------------------------------------------------------
struct SceneFace : public IndexedFace
{
	RenderCache*  renderCache; // Pointer to the render cache this face belongs to.
	unsigned long renderStamp; // Indicates when the face was last rendered.
};


//-----------------------------------------------------------------------------
// Ray Intersection Result Struction
//-----------------------------------------------------------------------------
struct RayIntersectionResult
{
	Material*    material;  // Pointer to the material of the intersected face.
	float		 distance;  // Distance the ray can travel until intersection occurs.
	D3DXVECTOR3  point;		// Intersection point in 3D space.
	SceneObject* hitObject; // Pointer to the hit scene object (if one was hit).

	RayIntersectionResult()
	{
		material = NULL;
		distance = 0.0f;
		point = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		hitObject = NULL;
	}
};


//-----------------------------------------------------------------------------
// Scene Manager Class
//-----------------------------------------------------------------------------
class SceneManager
{
private:
	char*	      m_name;		 // シーンの名前。
	float         m_scale;		 // メートル単位のシーンスケール。
	ViewFrustum   m_viewFrustum; 	 // 視界カリングのために錐台を表示します。
	D3DXVECTOR3   m_gravity;	 // 重力
	bool	      m_loaded;		 // シーンがロードされているかどうか
	Mesh*	      m_mesh;		 // シーンのメッシュ。
	unsigned long m_maxFaces;	 // Maximum number of faces per scene leaf.
	float	      m_maxHalfSize; 	 // Maximum half size of a scene leaf.
	unsigned long m_frameStamp;  	 // 現在のフレームのタイムスタンプ。

	LinkedList<SceneObject>*   m_dynamicObjects;    // Linked list of dynamic objects.
	LinkedList<SceneOccluder>* m_occludingObjects;  // Linked list of occluding objects.
	LinkedList<SceneOccluder>* m_visibleOccluders;  // Linked list of visible occluders each frame.
	LinkedList<SceneObject>*   m_playerSpawnPoints; // Linked list of player spawn points.
	LinkedList<SpawnerObject>* m_objectSpawners;    // Linked list of object spawners.

	char*	   m_spawnerPath; // Path used for loading the spawner object scripts.

	SceneLeaf* m_firstLeaf;   // The first scene leaf in the scene hierarchy.

	IDirect3DVertexBuffer9*  m_sceneVertexBuffer; // Vertex buffer for all the vertices in the scene.
	Vertex*			 m_vertices;	      // Pointer for accessing the vertices in the vertex buffer.
	unsigned long		 m_totalVertices;     // Total number of vertices in the scene.

	LinkedList<RenderCache>* m_renderCaches;      // Linked list of render caches.

	unsigned long m_totalFaces;	     // シーン内の面の総数。
	SceneFace*    m_faces;	  	     // シーン内の面の配列。
	unsigned long m_totalCollisionFaces; // 可能な衝突面の総数。
	SceneFace**   m_collisionFaces;	     // オブジェクトが衝突する可能性のある面の配列を追跡するために使用されます。

public:
	SceneManager( float scale, char *spawnerPath );
	virtual ~SceneManager();

	void LoadScene( char *name, char *path = "./" );
	void DestroyScene();
	bool IsLoaded();

	void Update( float elapsed, D3DXMATRIX *view = NULL );
	void Render( float elapsed, D3DXVECTOR3 viewer );

	SceneObject* AddObject( SceneObject *object );
	void	     RemoveObject( SceneObject **object );

	SceneObject* GetRandomPlayerSpawnPoint();
	SceneObject* GetSpawnPointByID( long id );
	long	     GetSpawnPointID( SceneObject *point );

	LinkedList<SpawnerObject>* GetSpawnerObjectList();

	bool RayIntersectScene( RayIntersectionResult *result, D3DXVECTOR3 rayPosition, D3DXVECTOR3 rayDirection, bool checkScene = true, SceneObject *thisObject = NULL, bool checkObjects = false );

private:
	void BuildOcclusionVolume( SceneOccluder *occluder, D3DXVECTOR3 viewer );

	void RecursiveSceneBuild( SceneLeaf *leaf, D3DXVECTOR3 translation, float halfSize );
	bool RecursiveSceneFrustumCheck( SceneLeaf *leaf, D3DXVECTOR3 viewer );
	void RecursiveSceneOcclusionCheck( SceneLeaf *leaf );
	void RecursiveSceneRayCheck( SceneLeaf *leaf, RayIntersectionResult *result, D3DXVECTOR3 rayPosition, D3DXVECTOR3 rayDirection, float *hitDistance );
	void RecursiveBuildCollisionArray( SceneLeaf *leaf, SceneObject *object );
};

#endif