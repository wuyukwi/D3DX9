//------------------------------------------------
// 当たり判定
//------------------------------------------------

#ifndef COLLISION_H
#define COLLISION_H


struct CollisionData
{
	float	      scale;		  // シーンマネージャーによるのスケール。
	float	      elapsed;	  	  // 現在のフレームの経過時間
	unsigned long frameStamp; 	  // シーンマネージャーによる現在のフレームスタンプ。

	SceneObject *object;		  // 当たり判定を実行するオブジェクトへのポインタ。

	D3DXVECTOR3 translation;	  // 楕円体空間での移動
	D3DXVECTOR3 velocity;		  // 楕円体空間の速度
	D3DXVECTOR3 normalizedVelocity;   // 楕円体空間での正規化された速度。

	D3DXVECTOR3 gravity;		  // 楕円体空間に変換される重力ベクトル。

	bool	    collisionFound; 	  //  衝突が見つかったかどうか
	float	    distance;		  // 衝突点までの距離
	D3DXVECTOR3 intersection;	  // 衝突が発生した実際の交点。
};


inline float GetLowestRoot( float a, float b, float c, float max )
{
	// 行列式を計算し、有効な場合はその平方根を取得します。
	float determinant = b * b - a * c;
	if( determinant < 0.0f )
		return 0.0f;
	determinant = (float)sqrt( determinant );

	// 最初のルートを計算し、それが範囲内にあることを確認します。
	float root1 = ( b + determinant ) / a;
	if( root1 <= 0.0f || root1 > max )
		root1 = max + 1.0f;

	// 2番目のルートを計算し、それが範囲内にあることを確認します。
	float root2 = ( b - determinant ) / a;
	if( root2 <= 0.0f || root2 > max )
		root2 = max + 1.0f;

	// 最も近いルートを取得
	float root = min( root1, root2 );

	// ルートが有効であることを確認
	if( root == max + 1.0f )
		return 0.0f;

	return root;
}


inline void CheckFace( CollisionData *data, D3DXVECTOR3 vertex0, D3DXVECTOR3 vertex1, D3DXVECTOR3 vertex2 )
{
	// 面の頂点から平面を作成します。
	D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &vertex0, &vertex1, &vertex2 );

	// 平面法線と速度ベクトルの角度を取得します。
	float angle = D3DXPlaneDotNormal( &plane, &data->normalizedVelocity );

	// 平面が速度ベクトルに面していることを確認します（つまり、背面を無視します）。
	if( angle > 0.0f )
		return;

	// 平面の法線ベクトルを取得します。
	D3DXVECTOR3 planeNormal;
	D3DXVec3Cross( &planeNormal, &( vertex0 - vertex1 ), &( vertex1 - vertex2 ) );
	D3DXVec3Normalize( &planeNormal, &planeNormal );

	// 球から平面までの距離を計算します。
	float signedPlaneDistance = D3DXVec3Dot( &data->translation, &planeNormal ) + plane.d;

	// 平面交差の間隔を取得する
	float time0, time1;
	bool embedded = false;

	float normalDotVelocity = D3DXVec3Dot( &planeNormal, &data->velocity );

	// 球が平面に平行に移動しているかどうかを確認します。
	if( normalDotVelocity == 0.0f )
	{
		// 球が平面に埋め込まれていない場合、球は衝突できません。
		if( fabs( signedPlaneDistance ) >= 1.0f )
			return;
		else
		{
			// 球は平面に埋め込まれているため、衝突します
			embedded = true;
			time0 = 0.0f;
			time1 = 1.0f;
		}
	}
	else
	{
		// 交差の時間を計算します。
		time0 = ( -1.0f - signedPlaneDistance ) / normalDotVelocity;
		time1 = ( 1.0f - signedPlaneDistance ) / normalDotVelocity;

		// time0がtime1よりも小さいことを確認
		if( time0 > time1 )
		{
			float swap = time1;
			time1 = time0;
			time0 = swap;
		}

		// 交差の時間枠が範囲外の場合、衝突することはできません。
		if( time0 > 1.0f || time1 < 0.0f )
			return;

		// 時間枠を正規化します。
		if( time0 < 0.0f ) time0 = 0.0f;
		if( time1 < 0.0f ) time1 = 0.0f;
		if( time0 > 1.0f ) time0 = 1.0f;
		if( time1 > 1.0f ) time1 = 1.0f;
	}

	// 交差点が発生したかどうか、発生した場所、および発生した時期を追跡するために使用される変数。
	bool intersectFound = false;
	D3DXVECTOR3 intersection;
	float intersectTime = 1.0f;

	// 球が平面に埋め込まれているかどうかを確認します。
	if( embedded == false )
	{
		// time0での平面交点を取得します。
		D3DXVECTOR3 planeIntersectionPoint = ( data->translation - planeNormal ) + data->velocity * time0;

		// 面の2つのエッジのベクトルを取得します。
		D3DXVECTOR3 edge0 = vertex1 - vertex0;
		D3DXVECTOR3 edge1 = vertex2 - vertex0;

		// エッジの角度を取得し、結合します
		float angle0 = D3DXVec3Dot( &edge0, &edge0 );
		float angle1 = D3DXVec3Dot( &edge0, &edge1 );
		float angle2 = D3DXVec3Dot( &edge1, &edge1 );
		float combined = ( angle0 * angle2 ) - ( angle1 * angle1 );

		// 2つのエッジ間の角度を取得します。
		D3DXVECTOR3 split = planeIntersectionPoint - vertex0;
		float splitAngle0 = D3DXVec3Dot( &split, &edge0 );
		float splitAngle1 = D3DXVec3Dot( &split, &edge1 );

		float x = ( splitAngle0 * angle2 ) - ( splitAngle1 * angle1 );
		float y = ( splitAngle1 * angle0 ) - ( splitAngle0 * angle1 );
		float z = x + y - combined;

		// z のビット単位の AND と x と y の包含 OR の補数をとる。 
		// 結果を0x80000000とビットごとにANDして返します。
		// ビット毎の結果はゼロの値は偽となり、それ以外は真となる。
		if( ( ( (unsigned int&)z & ~( (unsigned int&)x | (unsigned int&)y ) ) & 0x80000000 ) != 0 )
		{
			intersectFound = true;
			intersection = planeIntersectionPoint;
			intersectTime = time0;
		}
	}

	// 衝突が見つかっているかどうか
	if( intersectFound == false )
	{
		// 速度ベクトルの長さの2乗を取得します。
		float squaredVelocityLength = D3DXVec3LengthSq( &data->velocity );

		// 二次方程式は、面の頂点とエッジごとに解く必要があります。
		// 次の変数は、各2次方程式を作成するために使用されます。
		float a, b, c;

		// 各二次方程式の結果を格納するために使用されます。
		float newTime;

		// まず、頂点を確認します。
		a = squaredVelocityLength;

		// Check vertex 0.
		b = 2.0f * D3DXVec3Dot( &data->velocity, &( data->translation - vertex0 ) );
		c = D3DXVec3LengthSq( &( vertex0 - data->translation ) ) - 1.0f;
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			intersectFound = true;
			intersection = vertex0;
			intersectTime = newTime;
		}

		// Check vertex 1.
		b = 2.0f * D3DXVec3Dot( &data->velocity, &( data->translation - vertex1 ) );
		c = D3DXVec3LengthSq( &( vertex1 - data->translation ) ) - 1.0f;
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			intersectFound = true;
			intersection = vertex1;
			intersectTime = newTime;
		}

		// Check vertex 2.
		b = 2.0f * D3DXVec3Dot( &data->velocity, &( data->translation - vertex2 ) );
		c = D3DXVec3LengthSq( &( vertex2 - data->translation ) ) - 1.0f;
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			intersectFound = true;
			intersection = vertex2;
			intersectTime = newTime;
		}

		// 頂点0から頂点1までのエッジを確認します。
		D3DXVECTOR3 edge = vertex1 - vertex0;
		D3DXVECTOR3 vectorSphereVertex = vertex0 - data->translation;
		float squaredEdgeLength = D3DXVec3LengthSq( &edge );
		float angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		float angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// 二次方程式の変数を取得します。
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		// 球がエッジと交差するかどうかを確認します。
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// エッジの境界内で交点が発生したことを確認します。
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex0 + edge * point;
				intersectTime = newTime;
			}
		}

		// 頂点1から頂点2までのエッジを確認します。
		edge = vertex2 - vertex1;
		vectorSphereVertex = vertex1 - data->translation;
		squaredEdgeLength = D3DXVec3LengthSq( &edge );
		angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// 二次方程式のパラメータを取得します。
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		// 球がエッジと交差するかどうかを確認します。
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// エッジの境界内で交点が発生したことを確認します。
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex1 + edge * point;
				intersectTime = newTime;
			}
		}

		// 頂点2から頂点0までのエッジを確認します。
		edge = vertex0 - vertex2;
		vectorSphereVertex = vertex2 - data->translation;
		squaredEdgeLength = D3DXVec3LengthSq( &edge );
		angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// 二次方程式のパラメータを取得します
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		//  球がエッジと交差するかどうかを確認します。
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// エッジの境界内で交点が発生したことを確認します。
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex2 + edge * point;
				intersectTime = newTime;
			}
		}
	}

	// 交差点が発生していないか
	if( intersectFound == true )
	{
		// 衝突までの距離（つまり、速度ベクトルに沿った時間）を取得します。
		float collisionDistance = intersectTime * D3DXVec3Length( &data->velocity );

		// 必要の時に、衝突のデータを保存します。
		if( data->collisionFound == false || collisionDistance < data->distance )
		{
			data->distance = collisionDistance;
			data->intersection = intersection;
			data->collisionFound = true;
		}
	}
}


inline void CollideWithScene( CollisionData *data, Vertex *vertices, SceneFace **faces, unsigned long totalFaces, LinkedList< SceneObject > *objects, unsigned long recursion = 5 )
{
	// 距離を計算します（スケールを入れて）。
	// 無視できるほど短い距離とされています。
	float epsilon = 0.5f * data->scale;

	// 衝突が検出されなかったことを示します。
	data->collisionFound = false;

	// 正規化された速度ベクトルを取得します。
	D3DXVec3Normalize( &data->normalizedVelocity, &data->velocity );

	// すべての面を確認する
	D3DXVECTOR3 vertex0, vertex1, vertex2;
	for( unsigned long f = 0; f < totalFaces; f++ )
	{
		// マテリアルが光線を無視するように設定されている場合は、この面をスキップします。
		if( faces[f]->renderCache->GetMaterial()->GetIgnoreRay() == true )
			continue;

		// 楕円体空間でこの面の頂点のコピーを取得します。
		vertex0.x = vertices[faces[f]->vertex0].translation.x / data->object->GetEllipsoidRadius().x;
		vertex0.y = vertices[faces[f]->vertex0].translation.y / data->object->GetEllipsoidRadius().y;
		vertex0.z = vertices[faces[f]->vertex0].translation.z / data->object->GetEllipsoidRadius().z;
		vertex1.x = vertices[faces[f]->vertex1].translation.x / data->object->GetEllipsoidRadius().x;
		vertex1.y = vertices[faces[f]->vertex1].translation.y / data->object->GetEllipsoidRadius().y;
		vertex1.z = vertices[faces[f]->vertex1].translation.z / data->object->GetEllipsoidRadius().z;
		vertex2.x = vertices[faces[f]->vertex2].translation.x / data->object->GetEllipsoidRadius().x;
		vertex2.y = vertices[faces[f]->vertex2].translation.y / data->object->GetEllipsoidRadius().y;
		vertex2.z = vertices[faces[f]->vertex2].translation.z / data->object->GetEllipsoidRadius().z;

		// この面との衝突を確認する
		CheckFace( data, vertex0, vertex1, vertex2 );
	}

	// ヒットゴーストオブジェクトのリストとそれらまでの距離のリストを作成します。
	LinkedList< SceneObject > *ghostHits = new LinkedList< SceneObject >;
	LinkedList< float > *ghostDistances = new LinkedList< float >;

	// 次のオブジェクトの衝突チェックに使用される変数。
	D3DXVECTOR3 translation, velocity, vectorColliderObject, vectorObjectCollider, vectorObjectRadius;
	float distToCollision, colliderRadius, objectRadius;

	// オブジェクトのリストを確認します。
	SceneObject *hitObject = NULL;
	SceneObject *nextObject = objects->GetFirst();
	while( nextObject != NULL )
	{
		// コライダーの場合は、このオブジェクトをスキップする。 自分自身をチェックすることはできません。
		if( nextObject != data->object )
		{
			// 楕円体空間でのこのオブジェクトの平行移動と速度を取得します。
			translation.x = nextObject->GetTranslation().x / data->object->GetEllipsoidRadius().x;
			translation.y = nextObject->GetTranslation().y / data->object->GetEllipsoidRadius().y;
			translation.z = nextObject->GetTranslation().z / data->object->GetEllipsoidRadius().z;
			velocity.x = nextObject->GetVelocity().x / data->object->GetEllipsoidRadius().x;
			velocity.y = nextObject->GetVelocity().y / data->object->GetEllipsoidRadius().y;
			velocity.z = nextObject->GetVelocity().z / data->object->GetEllipsoidRadius().z;
			velocity *= data->elapsed;

			// コライダーからこのオブジェクトへ、またはその逆方向の正規化されたベクトルを取得します。
			D3DXVec3Normalize( &vectorColliderObject, &( translation - data->translation ) );
			D3DXVec3Normalize( &vectorObjectCollider, &( data->translation - translation ) );

			// 他の方向への各楕円体の半径を計算します。
			colliderRadius = D3DXVec3Length( &vectorColliderObject );
			vectorObjectRadius.x = vectorObjectCollider.x * nextObject->GetEllipsoidRadius().x / data->object->GetEllipsoidRadius().x;
			vectorObjectRadius.y = vectorObjectCollider.y * nextObject->GetEllipsoidRadius().y / data->object->GetEllipsoidRadius().y;
			vectorObjectRadius.z = vectorObjectCollider.z * nextObject->GetEllipsoidRadius().z / data->object->GetEllipsoidRadius().z;
			objectRadius = D3DXVec3Length( &vectorObjectRadius );

			// 2つの球の間の衝突を確認します。
			if( IsSphereCollidingWithSphere( &distToCollision, data->translation, translation, velocity - data->velocity, colliderRadius + objectRadius ) == true )
			{
				// ヒットしたオブジェクトがゴーストかどうかを確認します。
				if( nextObject->GetGhost() == true )
				{
					// 両方のオブジェクトが衝突できる場合は、ヒットしたオブジェクトへのポインタとヒットする距離を格納します。
					if( nextObject->GetIgnoreCollisions() == false && data->object->GetIgnoreCollisions() == false )
					{
						ghostHits->Add( nextObject );
						ghostDistances->Add( &distToCollision );
					}
				}
				else
				{
					// 必要の時に衝突のデータを保存します。
					if( data->collisionFound == false || distToCollision < data->distance )
					{
						data->distance = distToCollision;
						data->intersection = data->normalizedVelocity * distToCollision;
						data->collisionFound = true;

						// ヒットオブジェクトへのポインタを格納します。
						hitObject = nextObject;
					}
				}
			}
		}

		// 次のオブジェクトに
		nextObject = objects->GetNext( nextObject );
	}

	// ヒットしたゴーストオブジェクトとそれらの衝突距離のリストを繰り返し処理します。
	ghostHits->Iterate( true );
	ghostDistances->Iterate( true );
	while( ghostHits->Iterate() != NULL && ghostDistances->Iterate() != NULL )
	{
		// ゴーストオブジェクトに当たる距離がクローゼットの実際の衝突までの距離よりも短い場合、ゴーストオブジェクトに当たっています。
		if( *ghostDistances->GetCurrent() < data->distance )
		{
			// 両方のオブジェクト間の衝突を登録します。
			ghostHits->GetCurrent()->CollisionOccurred( data->object, data->frameStamp );
			data->object->CollisionOccurred( ghostHits->GetCurrent(), data->frameStamp );
		}
	}

	// ゴーストヒットと距離リストを廃棄します。
	ghostHits->ClearPointers();
	SAFE_DELETE( ghostHits );
	ghostDistances->ClearPointers();
	SAFE_DELETE( ghostDistances );

	// 衝突が発生しなかった場合は、全速移動するだけです。
	if( data->collisionFound == false )
	{
		data->translation = data->translation + data->velocity;
		return;
	}

	// 目的地（つまり、オブジェクトが到達しようとしていたポイント）を計算します。
	D3DXVECTOR3 destination = data->translation + data->velocity;

	// 新しい移動は、オブジェクトが実際に行き着く地点となります。
	D3DXVECTOR3 newTranslation = data->translation;

	// オブジェクトがすでに目的地に非常に近い場合は、移動を無視します。
	if( data->distance >= epsilon )
	{
		// 距離を移動するために必要な新しい速度を計算します。
		D3DXVECTOR3 newVelocity = data->normalizedVelocity * ( data->distance - epsilon );

		// 新しい移動算出する
		newTranslation = data->translation + newVelocity;

		// ポリゴンの交点は、その点を考慮して調整します。
		// 実際の交点まで移動しない。
		D3DXVec3Normalize( &newVelocity, &newVelocity );
		data->intersection = data->intersection - newVelocity * epsilon;
	}

	// オブジェクトとの衝突が発生したかどうかを確認
	if( hitObject != NULL )
	{
		// オブジェクトの新しい移動を設定します。
		data->translation = newTranslation;

		// オブジェクトが互いにプッシュできるように、プッシュ速度を計算して適用します。
		D3DXVECTOR3 push = ( hitObject->GetVelocity() + data->object->GetVelocity() ) / 10.0f;
		hitObject->SetVelocity( push );
		data->object->SetVelocity( push );

		// 許可されている場合は、両方のオブジェクト間の衝突を登録します。
		if( hitObject->GetIgnoreCollisions() == false && data->object->GetIgnoreCollisions() == false )
		{
			hitObject->CollisionOccurred( data->object, data->frameStamp );
			data->object->CollisionOccurred( hitObject, data->frameStamp );
		}

		return;
	}

	// スライド平面として機能する平面を作成します。
	D3DXVECTOR3 slidePlaneOrigin = data->intersection;
	D3DXVECTOR3 slidePlaneNormal;
	D3DXVec3Normalize( &slidePlaneNormal, &( newTranslation - data->intersection ) );
	D3DXPLANE slidingPlane;
	D3DXPlaneFromPointNormal( &slidingPlane, &slidePlaneOrigin, &slidePlaneNormal );

	// スライディングを考慮した新しい宛先を計算します。
	D3DXVECTOR3 newDestination = destination - slidePlaneNormal * ( D3DXVec3Dot( &destination, &slidePlaneNormal ) + slidingPlane.d );
	newDestination += slidePlaneNormal * epsilon;

	// スライドのベクトルである新しい速度を計算します。
	D3DXVECTOR3 newVelocity = newDestination - data->intersection;

	// 新しい速度が少なすぎるかどうかを確認します。
	if( D3DXVec3Length( &newVelocity ) <= epsilon )
	{
		// 速度が少すぎるため、続行する必要はありません
		// 衝突検出を実行します。 したがって、新しい平行移動と速度を設定してから戻ります
		data->translation = newTranslation;
		data->velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		return;
	}

	// 新しい平行移動と速度を設定します。
	data->translation = newTranslation;
	data->velocity = newVelocity;

	// 許可されている場合は、別の衝突検出の繰り返しを実行します。
	recursion--;
	if( recursion > 0 )
		CollideWithScene( data, vertices, faces, totalFaces, objects, recursion );
}


inline void PerformCollisionDetection( CollisionData *data, Vertex *vertices, SceneFace **faces, unsigned long totalFaces, LinkedList< SceneObject > *dynamicObjects )
{
	// 楕円体空間でのオブジェクトの平行移動を計算します。
	data->translation.x = data->object->GetTranslation().x / data->object->GetEllipsoidRadius().x;
	data->translation.y = data->object->GetTranslation().y / data->object->GetEllipsoidRadius().y;
	data->translation.z = data->object->GetTranslation().z / data->object->GetEllipsoidRadius().z;

	// 楕円体空間でのオブジェクトの速度を計算します。
	data->velocity = data->object->GetVelocity() * data->elapsed;
	data->velocity.x /= data->object->GetEllipsoidRadius().x;
	data->velocity.y /= data->object->GetEllipsoidRadius().y;
	data->velocity.z /= data->object->GetEllipsoidRadius().z;

	// 再帰的に衝突検出を開始します。
	CollideWithScene( data, vertices, faces, totalFaces, dynamicObjects );

	// 次に、速度を重力ベクトル（楕円体空間）に設定します。
	data->velocity.x = data->gravity.x / data->object->GetEllipsoidRadius().x;
	data->velocity.y = data->gravity.y / data->object->GetEllipsoidRadius().y;
	data->velocity.z = data->gravity.z / data->object->GetEllipsoidRadius().z;

	// 別の再帰的に衝突検出を実行して、重力を適用します。
	CollideWithScene( data, vertices, faces, totalFaces, dynamicObjects );

	// オブジェクトの新しい変換を楕円体空間から変換し直します。
	data->translation.x = data->translation.x * data->object->GetEllipsoidRadius().x;
	data->translation.y = data->translation.y * data->object->GetEllipsoidRadius().y;
	data->translation.z = data->translation.z * data->object->GetEllipsoidRadius().z;

	// シーン内のすべての面を通過し、交差点を確認します。
	float hitDistance = -1.0f;
	for( unsigned long f = 0; f < totalFaces; f++ )
	{
		// マテリアルが光線を無視するように設定されている場合は、この面をスキップします。
		if( faces[f]->renderCache->GetMaterial()->GetIgnoreRay() == true )
			continue;

		// 線分交差テストを実行して、この面がオブジェクトの下にあるかどうかを確認します。
		float distance;
		if( D3DXIntersectTri( (D3DXVECTOR3*)&vertices[faces[f]->vertex0], (D3DXVECTOR3*)&vertices[faces[f]->vertex1], (D3DXVECTOR3*)&vertices[faces[f]->vertex2], &data->translation, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), NULL, NULL, &distance ) == TRUE )
			if( distance < hitDistance || hitDistance == -1.0f )
				hitDistance = distance;
	}

	// 線分の交点までの距離がy軸に沿ったオブジェクトの半径よりも小さい場合、オブジェクトは地面に埋め込まれます。 
	// したがって、オブジェクトを地面から押し上げるだけです。
	if( hitDistance != -1.0f )
		if( hitDistance < data->object->GetEllipsoidRadius().y )
			data->translation.y += data->object->GetEllipsoidRadius().y - hitDistance;

	// オブジェクトが地面に接触しているかどうかを確認します。
	if( hitDistance != -1.0f && hitDistance < data->object->GetEllipsoidRadius().y + 0.1f / data->scale )
		data->object->SetTouchingGroundFlag( true );
	else
		data->object->SetTouchingGroundFlag( false );

	// 衝突検出後にオブジェクトの平行移動行列を更新します。
	data->object->SetTranslation( data->translation );
}

#endif