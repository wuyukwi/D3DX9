/********************************************************************************* 

  *FileName: Bounding_Volume.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/07

**********************************************************************************/  


#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H



class BoundingVolume
{
public:
	BoundingVolume();
	virtual ~BoundingVolume();

	void BoundingVolumeFromMesh(ID3DXMesh* mesh, D3DXVECTOR3 ellipsoidRadius = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void BoundingVolumeFromVertices(D3DXVECTOR3* vertices, unsigned long totalVertices, unsigned long vertexStride, D3DXVECTOR3 ellipsoidRadius = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void CloneBoundingVolume(BoundingBox* box, BoundingSphere* sphere, D3DXVECTOR3 ellipsoidRadius = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void RepositionBoundingVolume(D3DXMATRIX* location);

	void			SetBoundingBox(D3DXVECTOR3 min, D3DXVECTOR3 max);
	BoundingBox* GetBoundingBox();

	void			SetBoundingSphere(D3DXVECTOR3 centre, float radius, D3DXVECTOR3 ellipsoidRadius = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	BoundingSphere* GetBoundingSphere();

	void			SetEllipsoidRadius(D3DXVECTOR3 ellipsoidRadius);
	D3DXVECTOR3		GetEllipsoidRadius();

private:
	BoundingBox* m_box;
	BoundingSphere* m_sphere;

	D3DXVECTOR3		m_originalMin;
	D3DXVECTOR3		m_originalMax;
	D3DXVECTOR3		m_originalCentre;

	D3DXVECTOR3		m_ellipsoidRadius;
};

#endif // BOUNDING_VOLUME_H