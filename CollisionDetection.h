//------------------------------------------------
// �����蔻��
//------------------------------------------------

#ifndef COLLISION_H
#define COLLISION_H


struct CollisionData
{
	float	      scale;		  // �V�[���}�l�[�W���[�ɂ��̃X�P�[���B
	float	      elapsed;	  	  // ���݂̃t���[���̌o�ߎ���
	unsigned long frameStamp; 	  // �V�[���}�l�[�W���[�ɂ�錻�݂̃t���[���X�^���v�B

	SceneObject *object;		  // �����蔻������s����I�u�W�F�N�g�ւ̃|�C���^�B

	D3DXVECTOR3 translation;	  // �ȉ~�̋�Ԃł̈ړ�
	D3DXVECTOR3 velocity;		  // �ȉ~�̋�Ԃ̑��x
	D3DXVECTOR3 normalizedVelocity;   // �ȉ~�̋�Ԃł̐��K�����ꂽ���x�B

	D3DXVECTOR3 gravity;		  // �ȉ~�̋�Ԃɕϊ������d�̓x�N�g���B

	bool	    collisionFound; 	  //  �Փ˂������������ǂ���
	float	    distance;		  // �Փ˓_�܂ł̋���
	D3DXVECTOR3 intersection;	  // �Փ˂������������ۂ̌�_�B
};


inline float GetLowestRoot( float a, float b, float c, float max )
{
	// �s�񎮂��v�Z���A�L���ȏꍇ�͂��̕��������擾���܂��B
	float determinant = b * b - a * c;
	if( determinant < 0.0f )
		return 0.0f;
	determinant = (float)sqrt( determinant );

	// �ŏ��̃��[�g���v�Z���A���ꂪ�͈͓��ɂ��邱�Ƃ��m�F���܂��B
	float root1 = ( b + determinant ) / a;
	if( root1 <= 0.0f || root1 > max )
		root1 = max + 1.0f;

	// 2�Ԗڂ̃��[�g���v�Z���A���ꂪ�͈͓��ɂ��邱�Ƃ��m�F���܂��B
	float root2 = ( b - determinant ) / a;
	if( root2 <= 0.0f || root2 > max )
		root2 = max + 1.0f;

	// �ł��߂����[�g���擾
	float root = min( root1, root2 );

	// ���[�g���L���ł��邱�Ƃ��m�F
	if( root == max + 1.0f )
		return 0.0f;

	return root;
}


inline void CheckFace( CollisionData *data, D3DXVECTOR3 vertex0, D3DXVECTOR3 vertex1, D3DXVECTOR3 vertex2 )
{
	// �ʂ̒��_���畽�ʂ��쐬���܂��B
	D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &vertex0, &vertex1, &vertex2 );

	// ���ʖ@���Ƒ��x�x�N�g���̊p�x���擾���܂��B
	float angle = D3DXPlaneDotNormal( &plane, &data->normalizedVelocity );

	// ���ʂ����x�x�N�g���ɖʂ��Ă��邱�Ƃ��m�F���܂��i�܂�A�w�ʂ𖳎����܂��j�B
	if( angle > 0.0f )
		return;

	// ���ʂ̖@���x�N�g�����擾���܂��B
	D3DXVECTOR3 planeNormal;
	D3DXVec3Cross( &planeNormal, &( vertex0 - vertex1 ), &( vertex1 - vertex2 ) );
	D3DXVec3Normalize( &planeNormal, &planeNormal );

	// �����畽�ʂ܂ł̋������v�Z���܂��B
	float signedPlaneDistance = D3DXVec3Dot( &data->translation, &planeNormal ) + plane.d;

	// ���ʌ����̊Ԋu���擾����
	float time0, time1;
	bool embedded = false;

	float normalDotVelocity = D3DXVec3Dot( &planeNormal, &data->velocity );

	// �������ʂɕ��s�Ɉړ����Ă��邩�ǂ������m�F���܂��B
	if( normalDotVelocity == 0.0f )
	{
		// �������ʂɖ��ߍ��܂�Ă��Ȃ��ꍇ�A���͏Փ˂ł��܂���B
		if( fabs( signedPlaneDistance ) >= 1.0f )
			return;
		else
		{
			// ���͕��ʂɖ��ߍ��܂�Ă��邽�߁A�Փ˂��܂�
			embedded = true;
			time0 = 0.0f;
			time1 = 1.0f;
		}
	}
	else
	{
		// �����̎��Ԃ��v�Z���܂��B
		time0 = ( -1.0f - signedPlaneDistance ) / normalDotVelocity;
		time1 = ( 1.0f - signedPlaneDistance ) / normalDotVelocity;

		// time0��time1�������������Ƃ��m�F
		if( time0 > time1 )
		{
			float swap = time1;
			time1 = time0;
			time0 = swap;
		}

		// �����̎��Ԙg���͈͊O�̏ꍇ�A�Փ˂��邱�Ƃ͂ł��܂���B
		if( time0 > 1.0f || time1 < 0.0f )
			return;

		// ���Ԙg�𐳋K�����܂��B
		if( time0 < 0.0f ) time0 = 0.0f;
		if( time1 < 0.0f ) time1 = 0.0f;
		if( time0 > 1.0f ) time0 = 1.0f;
		if( time1 > 1.0f ) time1 = 1.0f;
	}

	// �����_�������������ǂ����A���������ꏊ�A����є�������������ǐՂ��邽�߂Ɏg�p�����ϐ��B
	bool intersectFound = false;
	D3DXVECTOR3 intersection;
	float intersectTime = 1.0f;

	// �������ʂɖ��ߍ��܂�Ă��邩�ǂ������m�F���܂��B
	if( embedded == false )
	{
		// time0�ł̕��ʌ�_���擾���܂��B
		D3DXVECTOR3 planeIntersectionPoint = ( data->translation - planeNormal ) + data->velocity * time0;

		// �ʂ�2�̃G�b�W�̃x�N�g�����擾���܂��B
		D3DXVECTOR3 edge0 = vertex1 - vertex0;
		D3DXVECTOR3 edge1 = vertex2 - vertex0;

		// �G�b�W�̊p�x���擾���A�������܂�
		float angle0 = D3DXVec3Dot( &edge0, &edge0 );
		float angle1 = D3DXVec3Dot( &edge0, &edge1 );
		float angle2 = D3DXVec3Dot( &edge1, &edge1 );
		float combined = ( angle0 * angle2 ) - ( angle1 * angle1 );

		// 2�̃G�b�W�Ԃ̊p�x���擾���܂��B
		D3DXVECTOR3 split = planeIntersectionPoint - vertex0;
		float splitAngle0 = D3DXVec3Dot( &split, &edge0 );
		float splitAngle1 = D3DXVec3Dot( &split, &edge1 );

		float x = ( splitAngle0 * angle2 ) - ( splitAngle1 * angle1 );
		float y = ( splitAngle1 * angle0 ) - ( splitAngle0 * angle1 );
		float z = x + y - combined;

		// z �̃r�b�g�P�ʂ� AND �� x �� y �̕�� OR �̕␔���Ƃ�B 
		// ���ʂ�0x80000000�ƃr�b�g���Ƃ�AND���ĕԂ��܂��B
		// �r�b�g���̌��ʂ̓[���̒l�͋U�ƂȂ�A����ȊO�͐^�ƂȂ�B
		if( ( ( (unsigned int&)z & ~( (unsigned int&)x | (unsigned int&)y ) ) & 0x80000000 ) != 0 )
		{
			intersectFound = true;
			intersection = planeIntersectionPoint;
			intersectTime = time0;
		}
	}

	// �Փ˂��������Ă��邩�ǂ���
	if( intersectFound == false )
	{
		// ���x�x�N�g���̒�����2����擾���܂��B
		float squaredVelocityLength = D3DXVec3LengthSq( &data->velocity );

		// �񎟕������́A�ʂ̒��_�ƃG�b�W���Ƃɉ����K�v������܂��B
		// ���̕ϐ��́A�e2�����������쐬���邽�߂Ɏg�p����܂��B
		float a, b, c;

		// �e�񎟕������̌��ʂ��i�[���邽�߂Ɏg�p����܂��B
		float newTime;

		// �܂��A���_���m�F���܂��B
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

		// ���_0���璸�_1�܂ł̃G�b�W���m�F���܂��B
		D3DXVECTOR3 edge = vertex1 - vertex0;
		D3DXVECTOR3 vectorSphereVertex = vertex0 - data->translation;
		float squaredEdgeLength = D3DXVec3LengthSq( &edge );
		float angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		float angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// �񎟕������̕ϐ����擾���܂��B
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		// �����G�b�W�ƌ������邩�ǂ������m�F���܂��B
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// �G�b�W�̋��E���Ō�_�������������Ƃ��m�F���܂��B
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex0 + edge * point;
				intersectTime = newTime;
			}
		}

		// ���_1���璸�_2�܂ł̃G�b�W���m�F���܂��B
		edge = vertex2 - vertex1;
		vectorSphereVertex = vertex1 - data->translation;
		squaredEdgeLength = D3DXVec3LengthSq( &edge );
		angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// �񎟕������̃p�����[�^���擾���܂��B
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		// �����G�b�W�ƌ������邩�ǂ������m�F���܂��B
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// �G�b�W�̋��E���Ō�_�������������Ƃ��m�F���܂��B
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex1 + edge * point;
				intersectTime = newTime;
			}
		}

		// ���_2���璸�_0�܂ł̃G�b�W���m�F���܂��B
		edge = vertex0 - vertex2;
		vectorSphereVertex = vertex2 - data->translation;
		squaredEdgeLength = D3DXVec3LengthSq( &edge );
		angleEdgeVelocity = D3DXVec3Dot( &edge, &data->velocity );
		angleEdgeSphereVertex = D3DXVec3Dot( &edge, &vectorSphereVertex );

		// �񎟕������̃p�����[�^���擾���܂�
		a = squaredEdgeLength * -squaredVelocityLength + angleEdgeVelocity * angleEdgeVelocity;
		b = squaredEdgeLength * ( 2.0f * D3DXVec3Dot( &data->velocity, &vectorSphereVertex ) ) - 2.0f * angleEdgeVelocity * angleEdgeSphereVertex;
		c = squaredEdgeLength * ( 1.0f - D3DXVec3LengthSq( &vectorSphereVertex ) ) + angleEdgeSphereVertex * angleEdgeSphereVertex;

		//  �����G�b�W�ƌ������邩�ǂ������m�F���܂��B
		if( newTime = GetLowestRoot( a, b, c, intersectTime ) > 0.0f )
		{
			// �G�b�W�̋��E���Ō�_�������������Ƃ��m�F���܂��B
			float point = ( angleEdgeVelocity * newTime - angleEdgeSphereVertex ) / squaredEdgeLength;
			if( point >= 0.0f && point <= 1.0f )
			{
				intersectFound = true;
				intersection = vertex2 + edge * point;
				intersectTime = newTime;
			}
		}
	}

	// �����_���������Ă��Ȃ���
	if( intersectFound == true )
	{
		// �Փ˂܂ł̋����i�܂�A���x�x�N�g���ɉ��������ԁj���擾���܂��B
		float collisionDistance = intersectTime * D3DXVec3Length( &data->velocity );

		// �K�v�̎��ɁA�Փ˂̃f�[�^��ۑ����܂��B
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
	// �������v�Z���܂��i�X�P�[�������āj�B
	// �����ł���قǒZ�������Ƃ���Ă��܂��B
	float epsilon = 0.5f * data->scale;

	// �Փ˂����o����Ȃ��������Ƃ������܂��B
	data->collisionFound = false;

	// ���K�����ꂽ���x�x�N�g�����擾���܂��B
	D3DXVec3Normalize( &data->normalizedVelocity, &data->velocity );

	// ���ׂĂ̖ʂ��m�F����
	D3DXVECTOR3 vertex0, vertex1, vertex2;
	for( unsigned long f = 0; f < totalFaces; f++ )
	{
		// �}�e���A���������𖳎�����悤�ɐݒ肳��Ă���ꍇ�́A���̖ʂ��X�L�b�v���܂��B
		if( faces[f]->renderCache->GetMaterial()->GetIgnoreRay() == true )
			continue;

		// �ȉ~�̋�Ԃł��̖ʂ̒��_�̃R�s�[���擾���܂��B
		vertex0.x = vertices[faces[f]->vertex0].translation.x / data->object->GetEllipsoidRadius().x;
		vertex0.y = vertices[faces[f]->vertex0].translation.y / data->object->GetEllipsoidRadius().y;
		vertex0.z = vertices[faces[f]->vertex0].translation.z / data->object->GetEllipsoidRadius().z;
		vertex1.x = vertices[faces[f]->vertex1].translation.x / data->object->GetEllipsoidRadius().x;
		vertex1.y = vertices[faces[f]->vertex1].translation.y / data->object->GetEllipsoidRadius().y;
		vertex1.z = vertices[faces[f]->vertex1].translation.z / data->object->GetEllipsoidRadius().z;
		vertex2.x = vertices[faces[f]->vertex2].translation.x / data->object->GetEllipsoidRadius().x;
		vertex2.y = vertices[faces[f]->vertex2].translation.y / data->object->GetEllipsoidRadius().y;
		vertex2.z = vertices[faces[f]->vertex2].translation.z / data->object->GetEllipsoidRadius().z;

		// ���̖ʂƂ̏Փ˂��m�F����
		CheckFace( data, vertex0, vertex1, vertex2 );
	}

	// �q�b�g�S�[�X�g�I�u�W�F�N�g�̃��X�g�Ƃ����܂ł̋����̃��X�g���쐬���܂��B
	LinkedList< SceneObject > *ghostHits = new LinkedList< SceneObject >;
	LinkedList< float > *ghostDistances = new LinkedList< float >;

	// ���̃I�u�W�F�N�g�̏Փ˃`�F�b�N�Ɏg�p�����ϐ��B
	D3DXVECTOR3 translation, velocity, vectorColliderObject, vectorObjectCollider, vectorObjectRadius;
	float distToCollision, colliderRadius, objectRadius;

	// �I�u�W�F�N�g�̃��X�g���m�F���܂��B
	SceneObject *hitObject = NULL;
	SceneObject *nextObject = objects->GetFirst();
	while( nextObject != NULL )
	{
		// �R���C�_�[�̏ꍇ�́A���̃I�u�W�F�N�g���X�L�b�v����B �������g���`�F�b�N���邱�Ƃ͂ł��܂���B
		if( nextObject != data->object )
		{
			// �ȉ~�̋�Ԃł̂��̃I�u�W�F�N�g�̕��s�ړ��Ƒ��x���擾���܂��B
			translation.x = nextObject->GetTranslation().x / data->object->GetEllipsoidRadius().x;
			translation.y = nextObject->GetTranslation().y / data->object->GetEllipsoidRadius().y;
			translation.z = nextObject->GetTranslation().z / data->object->GetEllipsoidRadius().z;
			velocity.x = nextObject->GetVelocity().x / data->object->GetEllipsoidRadius().x;
			velocity.y = nextObject->GetVelocity().y / data->object->GetEllipsoidRadius().y;
			velocity.z = nextObject->GetVelocity().z / data->object->GetEllipsoidRadius().z;
			velocity *= data->elapsed;

			// �R���C�_�[���炱�̃I�u�W�F�N�g�ցA�܂��͂��̋t�����̐��K�����ꂽ�x�N�g�����擾���܂��B
			D3DXVec3Normalize( &vectorColliderObject, &( translation - data->translation ) );
			D3DXVec3Normalize( &vectorObjectCollider, &( data->translation - translation ) );

			// ���̕����ւ̊e�ȉ~�̂̔��a���v�Z���܂��B
			colliderRadius = D3DXVec3Length( &vectorColliderObject );
			vectorObjectRadius.x = vectorObjectCollider.x * nextObject->GetEllipsoidRadius().x / data->object->GetEllipsoidRadius().x;
			vectorObjectRadius.y = vectorObjectCollider.y * nextObject->GetEllipsoidRadius().y / data->object->GetEllipsoidRadius().y;
			vectorObjectRadius.z = vectorObjectCollider.z * nextObject->GetEllipsoidRadius().z / data->object->GetEllipsoidRadius().z;
			objectRadius = D3DXVec3Length( &vectorObjectRadius );

			// 2�̋��̊Ԃ̏Փ˂��m�F���܂��B
			if( IsSphereCollidingWithSphere( &distToCollision, data->translation, translation, velocity - data->velocity, colliderRadius + objectRadius ) == true )
			{
				// �q�b�g�����I�u�W�F�N�g���S�[�X�g���ǂ������m�F���܂��B
				if( nextObject->GetGhost() == true )
				{
					// �����̃I�u�W�F�N�g���Փ˂ł���ꍇ�́A�q�b�g�����I�u�W�F�N�g�ւ̃|�C���^�ƃq�b�g���鋗�����i�[���܂��B
					if( nextObject->GetIgnoreCollisions() == false && data->object->GetIgnoreCollisions() == false )
					{
						ghostHits->Add( nextObject );
						ghostDistances->Add( &distToCollision );
					}
				}
				else
				{
					// �K�v�̎��ɏՓ˂̃f�[�^��ۑ����܂��B
					if( data->collisionFound == false || distToCollision < data->distance )
					{
						data->distance = distToCollision;
						data->intersection = data->normalizedVelocity * distToCollision;
						data->collisionFound = true;

						// �q�b�g�I�u�W�F�N�g�ւ̃|�C���^���i�[���܂��B
						hitObject = nextObject;
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��
		nextObject = objects->GetNext( nextObject );
	}

	// �q�b�g�����S�[�X�g�I�u�W�F�N�g�Ƃ����̏Փˋ����̃��X�g���J��Ԃ��������܂��B
	ghostHits->Iterate( true );
	ghostDistances->Iterate( true );
	while( ghostHits->Iterate() != NULL && ghostDistances->Iterate() != NULL )
	{
		// �S�[�X�g�I�u�W�F�N�g�ɓ����鋗�����N���[�[�b�g�̎��ۂ̏Փ˂܂ł̋��������Z���ꍇ�A�S�[�X�g�I�u�W�F�N�g�ɓ������Ă��܂��B
		if( *ghostDistances->GetCurrent() < data->distance )
		{
			// �����̃I�u�W�F�N�g�Ԃ̏Փ˂�o�^���܂��B
			ghostHits->GetCurrent()->CollisionOccurred( data->object, data->frameStamp );
			data->object->CollisionOccurred( ghostHits->GetCurrent(), data->frameStamp );
		}
	}

	// �S�[�X�g�q�b�g�Ƌ������X�g��p�����܂��B
	ghostHits->ClearPointers();
	SAFE_DELETE( ghostHits );
	ghostDistances->ClearPointers();
	SAFE_DELETE( ghostDistances );

	// �Փ˂��������Ȃ������ꍇ�́A�S���ړ����邾���ł��B
	if( data->collisionFound == false )
	{
		data->translation = data->translation + data->velocity;
		return;
	}

	// �ړI�n�i�܂�A�I�u�W�F�N�g�����B���悤�Ƃ��Ă����|�C���g�j���v�Z���܂��B
	D3DXVECTOR3 destination = data->translation + data->velocity;

	// �V�����ړ��́A�I�u�W�F�N�g�����ۂɍs�������n�_�ƂȂ�܂��B
	D3DXVECTOR3 newTranslation = data->translation;

	// �I�u�W�F�N�g�����łɖړI�n�ɔ��ɋ߂��ꍇ�́A�ړ��𖳎����܂��B
	if( data->distance >= epsilon )
	{
		// �������ړ����邽�߂ɕK�v�ȐV�������x���v�Z���܂��B
		D3DXVECTOR3 newVelocity = data->normalizedVelocity * ( data->distance - epsilon );

		// �V�����ړ��Z�o����
		newTranslation = data->translation + newVelocity;

		// �|���S���̌�_�́A���̓_���l�����Ē������܂��B
		// ���ۂ̌�_�܂ňړ����Ȃ��B
		D3DXVec3Normalize( &newVelocity, &newVelocity );
		data->intersection = data->intersection - newVelocity * epsilon;
	}

	// �I�u�W�F�N�g�Ƃ̏Փ˂������������ǂ������m�F
	if( hitObject != NULL )
	{
		// �I�u�W�F�N�g�̐V�����ړ���ݒ肵�܂��B
		data->translation = newTranslation;

		// �I�u�W�F�N�g���݂��Ƀv�b�V���ł���悤�ɁA�v�b�V�����x���v�Z���ēK�p���܂��B
		D3DXVECTOR3 push = ( hitObject->GetVelocity() + data->object->GetVelocity() ) / 10.0f;
		hitObject->SetVelocity( push );
		data->object->SetVelocity( push );

		// ������Ă���ꍇ�́A�����̃I�u�W�F�N�g�Ԃ̏Փ˂�o�^���܂��B
		if( hitObject->GetIgnoreCollisions() == false && data->object->GetIgnoreCollisions() == false )
		{
			hitObject->CollisionOccurred( data->object, data->frameStamp );
			data->object->CollisionOccurred( hitObject, data->frameStamp );
		}

		return;
	}

	// �X���C�h���ʂƂ��ċ@�\���镽�ʂ��쐬���܂��B
	D3DXVECTOR3 slidePlaneOrigin = data->intersection;
	D3DXVECTOR3 slidePlaneNormal;
	D3DXVec3Normalize( &slidePlaneNormal, &( newTranslation - data->intersection ) );
	D3DXPLANE slidingPlane;
	D3DXPlaneFromPointNormal( &slidingPlane, &slidePlaneOrigin, &slidePlaneNormal );

	// �X���C�f�B���O���l�������V����������v�Z���܂��B
	D3DXVECTOR3 newDestination = destination - slidePlaneNormal * ( D3DXVec3Dot( &destination, &slidePlaneNormal ) + slidingPlane.d );
	newDestination += slidePlaneNormal * epsilon;

	// �X���C�h�̃x�N�g���ł���V�������x���v�Z���܂��B
	D3DXVECTOR3 newVelocity = newDestination - data->intersection;

	// �V�������x�����Ȃ����邩�ǂ������m�F���܂��B
	if( D3DXVec3Length( &newVelocity ) <= epsilon )
	{
		// ���x���������邽�߁A���s����K�v�͂���܂���
		// �Փˌ��o�����s���܂��B ���������āA�V�������s�ړ��Ƒ��x��ݒ肵�Ă���߂�܂�
		data->translation = newTranslation;
		data->velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		return;
	}

	// �V�������s�ړ��Ƒ��x��ݒ肵�܂��B
	data->translation = newTranslation;
	data->velocity = newVelocity;

	// ������Ă���ꍇ�́A�ʂ̏Փˌ��o�̌J��Ԃ������s���܂��B
	recursion--;
	if( recursion > 0 )
		CollideWithScene( data, vertices, faces, totalFaces, objects, recursion );
}


inline void PerformCollisionDetection( CollisionData *data, Vertex *vertices, SceneFace **faces, unsigned long totalFaces, LinkedList< SceneObject > *dynamicObjects )
{
	// �ȉ~�̋�Ԃł̃I�u�W�F�N�g�̕��s�ړ����v�Z���܂��B
	data->translation.x = data->object->GetTranslation().x / data->object->GetEllipsoidRadius().x;
	data->translation.y = data->object->GetTranslation().y / data->object->GetEllipsoidRadius().y;
	data->translation.z = data->object->GetTranslation().z / data->object->GetEllipsoidRadius().z;

	// �ȉ~�̋�Ԃł̃I�u�W�F�N�g�̑��x���v�Z���܂��B
	data->velocity = data->object->GetVelocity() * data->elapsed;
	data->velocity.x /= data->object->GetEllipsoidRadius().x;
	data->velocity.y /= data->object->GetEllipsoidRadius().y;
	data->velocity.z /= data->object->GetEllipsoidRadius().z;

	// �ċA�I�ɏՓˌ��o���J�n���܂��B
	CollideWithScene( data, vertices, faces, totalFaces, dynamicObjects );

	// ���ɁA���x���d�̓x�N�g���i�ȉ~�̋�ԁj�ɐݒ肵�܂��B
	data->velocity.x = data->gravity.x / data->object->GetEllipsoidRadius().x;
	data->velocity.y = data->gravity.y / data->object->GetEllipsoidRadius().y;
	data->velocity.z = data->gravity.z / data->object->GetEllipsoidRadius().z;

	// �ʂ̍ċA�I�ɏՓˌ��o�����s���āA�d�͂�K�p���܂��B
	CollideWithScene( data, vertices, faces, totalFaces, dynamicObjects );

	// �I�u�W�F�N�g�̐V�����ϊ���ȉ~�̋�Ԃ���ϊ��������܂��B
	data->translation.x = data->translation.x * data->object->GetEllipsoidRadius().x;
	data->translation.y = data->translation.y * data->object->GetEllipsoidRadius().y;
	data->translation.z = data->translation.z * data->object->GetEllipsoidRadius().z;

	// �V�[�����̂��ׂĂ̖ʂ�ʉ߂��A�����_���m�F���܂��B
	float hitDistance = -1.0f;
	for( unsigned long f = 0; f < totalFaces; f++ )
	{
		// �}�e���A���������𖳎�����悤�ɐݒ肳��Ă���ꍇ�́A���̖ʂ��X�L�b�v���܂��B
		if( faces[f]->renderCache->GetMaterial()->GetIgnoreRay() == true )
			continue;

		// ���������e�X�g�����s���āA���̖ʂ��I�u�W�F�N�g�̉��ɂ��邩�ǂ������m�F���܂��B
		float distance;
		if( D3DXIntersectTri( (D3DXVECTOR3*)&vertices[faces[f]->vertex0], (D3DXVECTOR3*)&vertices[faces[f]->vertex1], (D3DXVECTOR3*)&vertices[faces[f]->vertex2], &data->translation, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), NULL, NULL, &distance ) == TRUE )
			if( distance < hitDistance || hitDistance == -1.0f )
				hitDistance = distance;
	}

	// �����̌�_�܂ł̋�����y���ɉ������I�u�W�F�N�g�̔��a�����������ꍇ�A�I�u�W�F�N�g�͒n�ʂɖ��ߍ��܂�܂��B 
	// ���������āA�I�u�W�F�N�g��n�ʂ��牟���グ�邾���ł��B
	if( hitDistance != -1.0f )
		if( hitDistance < data->object->GetEllipsoidRadius().y )
			data->translation.y += data->object->GetEllipsoidRadius().y - hitDistance;

	// �I�u�W�F�N�g���n�ʂɐڐG���Ă��邩�ǂ������m�F���܂��B
	if( hitDistance != -1.0f && hitDistance < data->object->GetEllipsoidRadius().y + 0.1f / data->scale )
		data->object->SetTouchingGroundFlag( true );
	else
		data->object->SetTouchingGroundFlag( false );

	// �Փˌ��o��ɃI�u�W�F�N�g�̕��s�ړ��s����X�V���܂��B
	data->object->SetTranslation( data->translation );
}

#endif