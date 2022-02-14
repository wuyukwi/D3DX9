#ifndef GEOMETRY_H
#define GEOMETRY_H

//-----------------------------------------------------------------------------
// Vertex Structure
//-----------------------------------------------------------------------------
struct Vertex
{
	D3DXVECTOR3 translation; // 頂点の平行移動（ワールド空間で）。
	D3DXVECTOR3 normal;	 // 頂点の法線ベクトル。
	float	    tu;		 // テクスチャUV座標。
	float	    tv;		

	Vertex()
	{
		translation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		tu = 0.0f;
		tv = 0.0f;
	}

	Vertex( D3DXVECTOR3 t, D3DXVECTOR3 n, float u, float v )
	{
		translation = t;
		normal = n;
		tu = u;
		tv = v;
	}
};
#define VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define VERTEX_FVF_SIZE D3DXGetFVFVertexSize( VERTEX_FVF )


//-----------------------------------------------------------------------------
// Lit Vertex Structure
//-----------------------------------------------------------------------------
struct LVertex
{
	D3DXVECTOR3 translation; // 頂点の平行移動（ワールド空間で）。
	D3DCOLOR    diffuse;	 // 頂点の色。
	float	    tu;		 // テクスチャUV座標。
	float	    tv;
	
	LVertex()
	{
		translation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		diffuse = 0xFFFFFFFF;
		tu = 0.0f;
		tv = 0.0f;
	}

	LVertex( D3DXVECTOR3 t, D3DCOLOR d, float u, float v )
	{
		translation = t;
		diffuse = d;
		tu = u;
		tv = v;
	}
};
#define L_VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define L_VERTEX_FVF_SIZE D3DXGetFVFVertexSize( L_VERTEX_FVF )


//-----------------------------------------------------------------------------
// Transformed & Lit Vertex Structure
//-----------------------------------------------------------------------------
struct TLVertex
{
	D3DXVECTOR4 translation; // 頂点の平行移動（ビュースペース）。
	D3DCOLOR    diffuse;	 // 頂点の色。
	float	    tu;	         // テクスチャUV座標。
	float	    tv;

	TLVertex()
	{
		translation = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
		diffuse = 0xFFFFFFFF;
		tu = 0.0f;
		tv = 0.0f;
	}

	TLVertex( D3DXVECTOR4 t, D3DCOLOR d, float u, float v )
	{
		translation = t;
		diffuse = d;
		tu = u;
		tv = v;
	}
};
#define TL_VERTEX_FVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define TL_VERTEX_FVF_SIZE D3DXGetFVFVertexSize( TL_VERTEX_FVF )


//-----------------------------------------------------------------------------
// Edge Structure
//-----------------------------------------------------------------------------
struct Edge
{
	Vertex *vertex0; // エッジの最初の頂点。
	Vertex *vertex1; // エッジの2番目の頂点。

	Edge( Vertex *v0, Vertex *v1 )
	{
		vertex0 = v0;
		vertex1 = v1;
	}
};


//-----------------------------------------------------------------------------
// Indexed Edge Structure
//-----------------------------------------------------------------------------
struct IndexedEdge
{
	unsigned short vertex0; // エッジの最初の頂点のインデックス。
	unsigned short vertex1; // エッジの2番目の頂点のインデックス。

};
//-----------------------------------------------------------------------------
// Face Structure
//-----------------------------------------------------------------------------
struct Face
{
	Vertex *vertex0; // 面の最初の頂点。
	Vertex *vertex1; // 面の2番目の頂点。
	Vertex *vertex2; // 面の3番目の頂点。

	Face( Vertex *v0, Vertex *v1, Vertex *v2 )
	{
		vertex0 = v0;
		vertex1 = v1;
		vertex2 = v2;
	}
};


//-----------------------------------------------------------------------------
// Indexed Face Structure
//-----------------------------------------------------------------------------
struct IndexedFace
{
	unsigned short vertex0; // 面の最初のインデックス。
	unsigned short vertex1; // 面の2番目のインデックス。
	unsigned short vertex2; // 面の3番目のインデックス。
};


//-----------------------------------------------------------------------------
// 最初に指定されたボックスが2番目に指定されたボックス内にある場合はtrueを返します。
//-----------------------------------------------------------------------------
inline bool IsBoxInBox( D3DXVECTOR3 box1Min, D3DXVECTOR3 box1Max, D3DXVECTOR3 box2Min, D3DXVECTOR3 box2Max )
{
	if( box1Min.x > box2Max.x )
		return false;
	if( box1Min.y > box2Max.y )
		return false;
	if( box1Min.z > box2Max.z )
		return false;
	if( box1Max.x < box2Min.x )
		return false;
	if( box1Max.y < box2Min.y )
		return false;
	if( box1Max.z < box2Min.z )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
// 指定された面が指定されたボックス内にある場合はtrueを返します。
//-----------------------------------------------------------------------------
inline bool IsFaceInBox( Vertex *vertex0, Vertex *vertex1, Vertex *vertex2, D3DXVECTOR3 boxMin, D3DXVECTOR3 boxMax )
{
	// x軸に沿った面の最小点と最大点を求めます。 
	// これら2つのポイントがボックスのx軸の範囲内にあるかどうかを確認します。
	float minX = min( vertex0->translation.x, min( vertex1->translation.x, vertex2->translation.x ) );
	float maxX = max( vertex0->translation.x, max( vertex1->translation.x, vertex2->translation.x ) );
	if( maxX < boxMin.x )
		return false;
	if( minX > boxMax.x )
		return false;

	// y軸に沿った面の最小点と最大点を求めます。 
	// これら2つのポイントがボックスのy軸の範囲内にあるかどうかを確認します。
	float minY = min( vertex0->translation.y, min( vertex1->translation.y, vertex2->translation.y ) );
	float maxY = max( vertex0->translation.y, max( vertex1->translation.y, vertex2->translation.y ) );
	if( maxY < boxMin.y )
		return false;
	if( minY > boxMax.y )
		return false;

	// z軸に沿った面の最小点と最大点を求めます。 
	// これら2つのポイントがボックスのz軸の範囲内にあるかどうかを確認します。
	float minZ = min( vertex0->translation.z, min( vertex1->translation.z, vertex2->translation.z ) );
	float maxZ = max( vertex0->translation.z, max( vertex1->translation.z, vertex2->translation.z ) );
	if( maxZ < boxMin.z )
		return false;
	if( minZ > boxMax.z )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
// 指定されたボックスが指定されたバウンディングボリュームで完全に囲まれている場合はtrueを返します。
//-----------------------------------------------------------------------------
inline bool IsBoxEnclosedByVolume( LinkedList< D3DXPLANE > *planes, D3DXVECTOR3 min, D3DXVECTOR3 max )
{
	planes->Iterate( true );
	while( planes->Iterate() )
	{
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( min.x, min.y, min.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( max.x, min.y, min.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( min.x, max.y, min.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( max.x, max.y, min.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( min.x, min.y, max.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( max.x, min.y, max.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( min.x, max.y, max.z ) ) < 0.0f )
			return false;
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &D3DXVECTOR3( max.x, max.y, max.z ) ) < 0.0f )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// 指定された球が指定されたバウンディングボリュームと重なっている場合はtrueを返します。
//-----------------------------------------------------------------------------
inline bool IsSphereOverlappingVolume( LinkedList< D3DXPLANE > *planes, D3DXVECTOR3 translation, float radius )
{
	planes->Iterate( true );
	while( planes->Iterate() )
		if( D3DXPlaneDotCoord( planes->GetCurrent(), &translation ) < -radius )
			return false;

	return true;
}


//-----------------------------------------------------------------------------
// 指定された2つの球が衝突した場合にtrueを返します。
//-----------------------------------------------------------------------------
inline bool IsSphereCollidingWithSphere( float *collisionDistance, D3DXVECTOR3 translation1, D3DXVECTOR3 translation2, D3DXVECTOR3 velocitySum, float radiiSum )
{
	// 2つの球の間の距離を取得します。
	float distanceBetween = D3DXVec3Length( &( translation1 - translation2 ) ) - radiiSum;

	// 2つの球の速度ベクトルの合計の長さを取得します。
	float velocityLength = D3DXVec3Length( &velocitySum );

	// 球が互いに接触しておらず、速度の長さが球間の距離よりも短い場合、球は衝突できません。
	if( distanceBetween > 0.0f && velocityLength < distanceBetween )
		return false;

	// 速度ベクトルの正規化された合計を取得します。
	D3DXVECTOR3 normalizedVelocity;
	D3DXVec3Normalize( &normalizedVelocity, &velocitySum );

	// 2番目の球から最初の球への方向ベクトルを取得します。
	D3DXVECTOR3 direction = translation1 - translation2;

	// 正規化された速度ベクトルと方向ベクトルの間の角度を取得します。
	float angleBetween = D3DXVec3Dot( &normalizedVelocity, &direction );

	// 球が互いに離れているかどうかを確認します。
	if( angleBetween <= 0.0f )
	{
		// それらが互いに接触（または内部）しているかどうかを確認します。 
		// そうでない場合、それらは互いに離れているため、衝突することはできません。
		if( distanceBetween < 0.0f )
		{
			// 速度の長さが球間の距離よりも大きい場合
			// 球は互いに離れる方向に十分に速く移動しているため
			// 移動が完了したときに球は接触しません。
			if( velocityLength > -distanceBetween )
				return false;
		}
		else
			return false;
	}

	// 方向ベクトルの長さを取得します。
	float directionLength = D3DXVec3Length( &direction );

	// 2つの球の間のベクトルと速度ベクトルは、三角形の2つの辺を生成します。 
	// 次に、勾股弦の定理を使用して、三角形の3番目の辺の長さ計算します。
	float hypotenuse = ( directionLength * directionLength ) - ( angleBetween * angleBetween );

	// 球が半径の合計よりも近くなるようにします。
	float radiiSumSquared = radiiSum * radiiSum;
	if( hypotenuse >= radiiSumSquared )
		return false;

	// 球が衝突する速度ベクトルに沿った距離を取得します。 
	// 次に、この距離を使用して、衝突までの距離を計算します。
	float distance = radiiSumSquared - hypotenuse;
	*collisionDistance = angleBetween - (float)sqrt( distance );

	// 球が速度が許す値以上移動しないことを確保
	if( velocityLength < *collisionDistance )
		return false;

	return true;
}

#endif