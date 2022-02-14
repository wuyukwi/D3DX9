#ifndef GEOMETRY_H
#define GEOMETRY_H

//-----------------------------------------------------------------------------
// Vertex Structure
//-----------------------------------------------------------------------------
struct Vertex
{
	D3DXVECTOR3 translation; // ���_�̕��s�ړ��i���[���h��ԂŁj�B
	D3DXVECTOR3 normal;	 // ���_�̖@���x�N�g���B
	float	    tu;		 // �e�N�X�`��UV���W�B
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
	D3DXVECTOR3 translation; // ���_�̕��s�ړ��i���[���h��ԂŁj�B
	D3DCOLOR    diffuse;	 // ���_�̐F�B
	float	    tu;		 // �e�N�X�`��UV���W�B
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
	D3DXVECTOR4 translation; // ���_�̕��s�ړ��i�r���[�X�y�[�X�j�B
	D3DCOLOR    diffuse;	 // ���_�̐F�B
	float	    tu;	         // �e�N�X�`��UV���W�B
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
	Vertex *vertex0; // �G�b�W�̍ŏ��̒��_�B
	Vertex *vertex1; // �G�b�W��2�Ԗڂ̒��_�B

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
	unsigned short vertex0; // �G�b�W�̍ŏ��̒��_�̃C���f�b�N�X�B
	unsigned short vertex1; // �G�b�W��2�Ԗڂ̒��_�̃C���f�b�N�X�B

};
//-----------------------------------------------------------------------------
// Face Structure
//-----------------------------------------------------------------------------
struct Face
{
	Vertex *vertex0; // �ʂ̍ŏ��̒��_�B
	Vertex *vertex1; // �ʂ�2�Ԗڂ̒��_�B
	Vertex *vertex2; // �ʂ�3�Ԗڂ̒��_�B

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
	unsigned short vertex0; // �ʂ̍ŏ��̃C���f�b�N�X�B
	unsigned short vertex1; // �ʂ�2�Ԗڂ̃C���f�b�N�X�B
	unsigned short vertex2; // �ʂ�3�Ԗڂ̃C���f�b�N�X�B
};


//-----------------------------------------------------------------------------
// �ŏ��Ɏw�肳�ꂽ�{�b�N�X��2�ԖڂɎw�肳�ꂽ�{�b�N�X���ɂ���ꍇ��true��Ԃ��܂��B
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
// �w�肳�ꂽ�ʂ��w�肳�ꂽ�{�b�N�X���ɂ���ꍇ��true��Ԃ��܂��B
//-----------------------------------------------------------------------------
inline bool IsFaceInBox( Vertex *vertex0, Vertex *vertex1, Vertex *vertex2, D3DXVECTOR3 boxMin, D3DXVECTOR3 boxMax )
{
	// x���ɉ������ʂ̍ŏ��_�ƍő�_�����߂܂��B 
	// �����2�̃|�C���g���{�b�N�X��x���͈͓̔��ɂ��邩�ǂ������m�F���܂��B
	float minX = min( vertex0->translation.x, min( vertex1->translation.x, vertex2->translation.x ) );
	float maxX = max( vertex0->translation.x, max( vertex1->translation.x, vertex2->translation.x ) );
	if( maxX < boxMin.x )
		return false;
	if( minX > boxMax.x )
		return false;

	// y���ɉ������ʂ̍ŏ��_�ƍő�_�����߂܂��B 
	// �����2�̃|�C���g���{�b�N�X��y���͈͓̔��ɂ��邩�ǂ������m�F���܂��B
	float minY = min( vertex0->translation.y, min( vertex1->translation.y, vertex2->translation.y ) );
	float maxY = max( vertex0->translation.y, max( vertex1->translation.y, vertex2->translation.y ) );
	if( maxY < boxMin.y )
		return false;
	if( minY > boxMax.y )
		return false;

	// z���ɉ������ʂ̍ŏ��_�ƍő�_�����߂܂��B 
	// �����2�̃|�C���g���{�b�N�X��z���͈͓̔��ɂ��邩�ǂ������m�F���܂��B
	float minZ = min( vertex0->translation.z, min( vertex1->translation.z, vertex2->translation.z ) );
	float maxZ = max( vertex0->translation.z, max( vertex1->translation.z, vertex2->translation.z ) );
	if( maxZ < boxMin.z )
		return false;
	if( minZ > boxMax.z )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
// �w�肳�ꂽ�{�b�N�X���w�肳�ꂽ�o�E���f�B���O�{�����[���Ŋ��S�Ɉ͂܂�Ă���ꍇ��true��Ԃ��܂��B
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
// �w�肳�ꂽ�����w�肳�ꂽ�o�E���f�B���O�{�����[���Əd�Ȃ��Ă���ꍇ��true��Ԃ��܂��B
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
// �w�肳�ꂽ2�̋����Փ˂����ꍇ��true��Ԃ��܂��B
//-----------------------------------------------------------------------------
inline bool IsSphereCollidingWithSphere( float *collisionDistance, D3DXVECTOR3 translation1, D3DXVECTOR3 translation2, D3DXVECTOR3 velocitySum, float radiiSum )
{
	// 2�̋��̊Ԃ̋������擾���܂��B
	float distanceBetween = D3DXVec3Length( &( translation1 - translation2 ) ) - radiiSum;

	// 2�̋��̑��x�x�N�g���̍��v�̒������擾���܂��B
	float velocityLength = D3DXVec3Length( &velocitySum );

	// �����݂��ɐڐG���Ă��炸�A���x�̒��������Ԃ̋��������Z���ꍇ�A���͏Փ˂ł��܂���B
	if( distanceBetween > 0.0f && velocityLength < distanceBetween )
		return false;

	// ���x�x�N�g���̐��K�����ꂽ���v���擾���܂��B
	D3DXVECTOR3 normalizedVelocity;
	D3DXVec3Normalize( &normalizedVelocity, &velocitySum );

	// 2�Ԗڂ̋�����ŏ��̋��ւ̕����x�N�g�����擾���܂��B
	D3DXVECTOR3 direction = translation1 - translation2;

	// ���K�����ꂽ���x�x�N�g���ƕ����x�N�g���̊Ԃ̊p�x���擾���܂��B
	float angleBetween = D3DXVec3Dot( &normalizedVelocity, &direction );

	// �����݂��ɗ���Ă��邩�ǂ������m�F���܂��B
	if( angleBetween <= 0.0f )
	{
		// ����炪�݂��ɐڐG�i�܂��͓����j���Ă��邩�ǂ������m�F���܂��B 
		// �����łȂ��ꍇ�A�����݂͌��ɗ���Ă��邽�߁A�Փ˂��邱�Ƃ͂ł��܂���B
		if( distanceBetween < 0.0f )
		{
			// ���x�̒��������Ԃ̋��������傫���ꍇ
			// ���݂͌��ɗ��������ɏ\���ɑ����ړ����Ă��邽��
			// �ړ������������Ƃ��ɋ��͐ڐG���܂���B
			if( velocityLength > -distanceBetween )
				return false;
		}
		else
			return false;
	}

	// �����x�N�g���̒������擾���܂��B
	float directionLength = D3DXVec3Length( &direction );

	// 2�̋��̊Ԃ̃x�N�g���Ƒ��x�x�N�g���́A�O�p�`��2�̕ӂ𐶐����܂��B 
	// ���ɁA���Ҍ��̒藝���g�p���āA�O�p�`��3�Ԗڂ̕ӂ̒����v�Z���܂��B
	float hypotenuse = ( directionLength * directionLength ) - ( angleBetween * angleBetween );

	// �������a�̍��v�����߂��Ȃ�悤�ɂ��܂��B
	float radiiSumSquared = radiiSum * radiiSum;
	if( hypotenuse >= radiiSumSquared )
		return false;

	// �����Փ˂��鑬�x�x�N�g���ɉ������������擾���܂��B 
	// ���ɁA���̋������g�p���āA�Փ˂܂ł̋������v�Z���܂��B
	float distance = radiiSumSquared - hypotenuse;
	*collisionDistance = angleBetween - (float)sqrt( distance );

	// �������x�������l�ȏ�ړ����Ȃ����Ƃ��m��
	if( velocityLength < *collisionDistance )
		return false;

	return true;
}

#endif