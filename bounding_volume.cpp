/********************************************************************************* 

  *FileName: bounding_volume.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/07

**********************************************************************************/
#include "engine.h"

//bool BoundingBox::isPointInside(D3DXVECTOR3& p)
//{
//	if (p.x >= min.x && p.y >= min.y && p.z >= min.z &&
//		p.x <= max.x && p.y <= max.y && p.z <= max.z)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
BoundingVolume::BoundingVolume()
{
	m_box = new BoundingBox;
	m_sphere = new BoundingSphere;

	m_ellipsoidRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


BoundingVolume::~BoundingVolume()
{
	SAFE_DELETE(m_box);
	SAFE_DELETE(m_sphere);
}

//-----------------------------------------------------------------------------
// 指定されたメッシュを囲むバウンディングボリュームを構築します
//-----------------------------------------------------------------------------
void BoundingVolume::BoundingVolumeFromMesh(ID3DXMesh* mesh, D3DXVECTOR3 ellipsoidRadius)
{
	D3DXVECTOR3* vertices;
	if (SUCCEEDED(mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertices)))
	{
		D3DXComputeBoundingBox(vertices, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &m_box->min, &m_box->max);
		D3DXComputeBoundingSphere(vertices, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &m_sphere->centre, &m_sphere->radius);
		mesh->UnlockVertexBuffer();
	}

	m_sphere->centre.x = m_box->min.x + ((m_box->max.x - m_box->min.x) / 2.0f);
	m_sphere->centre.y = m_box->min.y + ((m_box->max.y - m_box->min.y) / 2.0f);
	m_sphere->centre.z = m_box->min.z + ((m_box->max.z - m_box->min.z) / 2.0f);

	m_box->halfSize = (float)max(fabs(m_box->max.x), max(fabs(m_box->max.y), fabs(m_box->max.z)));
	m_box->halfSize = (float)max(m_box->halfSize, max(fabs(m_box->min.x), max(fabs(m_box->min.y), fabs(m_box->min.z))));

	m_originalMin = m_box->min;
	m_originalMax = m_box->max;
	m_originalCentre = m_sphere->centre;

	SetEllipsoidRadius(ellipsoidRadius);
}

//-----------------------------------------------------------------------------
// 指定された頂点を囲むバウンディングボリュームを構築します
//-----------------------------------------------------------------------------
void BoundingVolume::BoundingVolumeFromVertices(D3DXVECTOR3* vertices, unsigned long totalVertices, unsigned long vertexStride, D3DXVECTOR3 ellipsoidRadius)
{
	D3DXComputeBoundingBox(vertices, totalVertices, vertexStride, &m_box->min, &m_box->max);
	D3DXComputeBoundingSphere(vertices, totalVertices, vertexStride, &m_sphere->centre, &m_sphere->radius);

	m_sphere->centre.x = m_box->min.x + ((m_box->max.x - m_box->min.x) / 2.0f);
	m_sphere->centre.y = m_box->min.y + ((m_box->max.y - m_box->min.y) / 2.0f);
	m_sphere->centre.z = m_box->min.z + ((m_box->max.z - m_box->min.z) / 2.0f);

	m_box->halfSize = (float)max(fabs(m_box->max.x), max(fabs(m_box->max.y), fabs(m_box->max.z)));
	m_box->halfSize = (float)max(m_box->halfSize, max(fabs(m_box->min.x), max(fabs(m_box->min.y), fabs(m_box->min.z))));

	m_originalMin = m_box->min;
	m_originalMax = m_box->max;
	m_originalCentre = m_sphere->centre;

	SetEllipsoidRadius(ellipsoidRadius);
}

//-----------------------------------------------------------------------------
// 指定されたバウンディングボリュームからバウンディングボリュームを構築します
//-----------------------------------------------------------------------------
void BoundingVolume::CloneBoundingVolume(BoundingBox* box, BoundingSphere* sphere, D3DXVECTOR3 ellipsoidRadius)
{
	m_box->min = box->min;
	m_box->max = box->max;
	m_sphere->centre = sphere->centre;
	m_sphere->radius = sphere->radius;

	m_box->halfSize = (float)max(fabs(m_box->max.x), max(fabs(m_box->max.y), fabs(m_box->max.z)));
	m_box->halfSize = (float)max(m_box->halfSize, max(fabs(m_box->min.x), max(fabs(m_box->min.y), fabs(m_box->min.z))));

	m_originalMin = m_box->min;
	m_originalMax = m_box->max;
	m_originalCentre = m_sphere->centre;

	SetEllipsoidRadius(ellipsoidRadius);
}

//-----------------------------------------------------------------------------
// 指定された行列によってバウンディングボリュームを再配置します。
//-----------------------------------------------------------------------------
void BoundingVolume::RepositionBoundingVolume(D3DXMATRIX* location)
{
	D3DXVec3TransformCoord(&m_box->min, &m_originalMin, location);
	D3DXVec3TransformCoord(&m_box->max, &m_originalMax, location);
	D3DXVec3TransformCoord(&m_sphere->centre, &m_originalCentre, location);
}

//-----------------------------------------------------------------------------
// バウンディングボックスのプロパティを設定します。
//-----------------------------------------------------------------------------
void BoundingVolume::SetBoundingBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	m_originalMin = m_box->min = min;
	m_originalMax = m_box->max = max;

	m_box->halfSize = (float)max(fabs(m_box->max.x), max(fabs(m_box->max.y), fabs(m_box->max.z)));
	m_box->halfSize = (float)max(m_box->halfSize, max(fabs(m_box->min.x), max(fabs(m_box->min.y), fabs(m_box->min.z))));
}

//-----------------------------------------------------------------------------
// バウンディングボックスを返します。
//-----------------------------------------------------------------------------
BoundingBox* BoundingVolume::GetBoundingBox()
{
	return m_box;
}

//-----------------------------------------------------------------------------
// バウンディング球のプロパティを設定します。
//-----------------------------------------------------------------------------
void BoundingVolume::SetBoundingSphere(D3DXVECTOR3 centre, float radius, D3DXVECTOR3 ellipsoidRadius)
{
	m_originalCentre = m_sphere->centre = centre;
	m_sphere->radius = radius;

	SetEllipsoidRadius(ellipsoidRadius);
}

//-----------------------------------------------------------------------------
// バウンディング球を返します。
//-----------------------------------------------------------------------------
BoundingSphere* BoundingVolume::GetBoundingSphere()
{
	return m_sphere;
}

//-----------------------------------------------------------------------------
// 楕円体の半径を球の半径のパーセンテージに設定します
//-----------------------------------------------------------------------------
void BoundingVolume::SetEllipsoidRadius(D3DXVECTOR3 ellipsoidRadius)
{
	m_ellipsoidRadius = D3DXVECTOR3(m_sphere->radius * ellipsoidRadius.x, m_sphere->radius * ellipsoidRadius.y, m_sphere->radius * ellipsoidRadius.z);
}

//-----------------------------------------------------------------------------
// 楕円体の半径を返します。
//-----------------------------------------------------------------------------
D3DXVECTOR3 BoundingVolume::GetEllipsoidRadius()
{
	return m_ellipsoidRadius;
}