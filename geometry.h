/********************************************************************************* 

  *FileName: geometry.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#ifndef GEOMETRY_H
#define GEOMETRY_H

//-----------------------------------------------------------------------------
// Vertex Structure
//-----------------------------------------------------------------------------
struct Vertex
{
	D3DXVECTOR3 translation; // Translation of the vertex (in world space).
	D3DXVECTOR3 normal;	 // Vertex's normal vector.
	D3DXVECTOR2	uv;		 // Texture UV coordinates.

	Vertex()
	{
		translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		uv = D3DXVECTOR2(0.0f, 0.0f);
	}

	Vertex(D3DXVECTOR3 pos, D3DXVECTOR3 nor, D3DXVECTOR2 uv)
	{
		this->translation = pos;
		this->normal = nor;
		this->uv = uv;
	}

	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		this->translation = D3DXVECTOR3(x, y, z);
		this->normal = D3DXVECTOR3(nx, ny, nz);
		this->uv = D3DXVECTOR2(u, v);
	}
};
#define VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define VERTEX_FVF_SIZE D3DXGetFVFVertexSize( VERTEX_FVF )

//-----------------------------------------------------------------------------
// Lit Vertex Structure
//-----------------------------------------------------------------------------
struct LVertex
{
	D3DXVECTOR3 translation; // Translation of the vertex (in world space).
	float	    tu;		 // Texture UV coordinates.
	float	    tv;

	LVertex()
	{
		translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tu = 0.0f;
		tv = 0.0f;
	}

	LVertex(D3DXVECTOR3 t, D3DCOLOR d, float u, float v)
	{
		translation = t;
		tu = u;
		tv = v;
	}

	LVertex(float x, float y, float z, float u, float v)
	{
		translation = D3DXVECTOR3(x, y, z);
		tu = u;
		tv = v;
	}
};
#define L_VERTEX_FVF ( D3DFVF_XYZ  | D3DFVF_TEX1 )
#define L_VERTEX_FVF_SIZE D3DXGetFVFVertexSize( L_VERTEX_FVF )


struct  CUSTOMVERTEX
{
	float x, y, z;   
	float u, v;		 
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)
#endif //GEOMETRY_H
