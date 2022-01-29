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
	D3DXVECTOR3 pos; // Translation of the vertex (in world space).
	D3DXVECTOR3 nor;	 // Vertex's normal vector.
	float	    tu;		 // Texture UV coordinates.
	float	    tv;

	Vertex()
	{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tu = 0.0f;
		tv = 0.0f;
	}

	Vertex(D3DXVECTOR3 t, D3DXVECTOR3 n, float u, float v)
	{
		pos = t;
		nor = n;
		tu = u;
		tv = v;
	}
};
#define VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define VERTEX_FVF_SIZE D3DXGetFVFVertexSize( VERTEX_FVF )

//-----------------------------------------------------------------------------
// Desc: CUSTOMVERTEX Structure
//-----------------------------------------------------------------------------
struct  CUSTOMVERTEX
{
	float x, y, z;    //pos
	float u, v;		  //tex
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)
#endif // _H_