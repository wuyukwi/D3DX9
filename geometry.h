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
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2	uv;		 // Texture UV coordinates.

	Vertex()
	{
		translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		uv = D3DXVECTOR2(0.0f, 0.0f);
	}

	Vertex(D3DXVECTOR3 pos, D3DXVECTOR3 nor, D3DXVECTOR2 uv)
	{
		this->translation = pos;
		this->normal = nor;
		this->col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		this->uv = uv;
	}

	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		this->translation = D3DXVECTOR3(x, y, z);
		this->normal = D3DXVECTOR3(nx, ny, nz);
		this->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		this->uv = D3DXVECTOR2(u, v);
	}
};
#define VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define VERTEX_FVF_SIZE D3DXGetFVFVertexSize( VERTEX_FVF )

//-----------------------------------------------------------------------------
// Lit Vertex Structure
//-----------------------------------------------------------------------------
struct LVertex
{
	D3DXVECTOR3 translation; // Translation of the vertex (in world space).
	D3DCOLOR	diffuse;
	float	    tu;		 // Texture UV coordinates.
	float	    tv;

	LVertex()
	{
		translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		diffuse = 0xFFFFFFFF;
		tu = 0.0f;
		tv = 0.0f;
	}

	LVertex(D3DXVECTOR3 t, D3DCOLOR d, float u, float v)
	{
		translation = t;
		diffuse = d;
		tu = u;
		tv = v;
	}

	LVertex(float x, float y, float z, float u, float v)
	{
		translation = D3DXVECTOR3(x, y, z);
		diffuse = 0xFFFFFFFF;
		tu = u;
		tv = v;
	}
};
#define L_VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//-----------------------------------------------------------------------------
// Transformed & Color Vertex Structure
//-----------------------------------------------------------------------------
struct TCVertex
{
	D3DXVECTOR3 translation; // Translation of the vertex (in world space).
	D3DCOLOR	 cor;		 

	TCVertex()
	{
		translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	TCVertex(D3DXVECTOR3 t)
	{
		translation = t;
		cor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	TCVertex(D3DXVECTOR3 t, D3DCOLOR cor)
	{
		translation = t;
		this->cor = cor;
	}

};
#define TC_VERTEX_FVF ( D3DFVF_XYZ  | D3DFVF_DIFFUSE )

struct  CUSTOMVERTEX
{
	float x, y, z;   
	float u, v;		 
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)

//
// Colors
//
const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));


//
// Materials
//

inline D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

inline D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

struct BoundingBox
{
	BoundingBox()
	{
		// infinite small 
		_min.x = FLT_MAX;
		_min.y = FLT_MAX;
		_min.z = FLT_MAX;

		_max.x = FLT_MIN;
		_max.y = FLT_MIN;
		_max.z = FLT_MIN;
	}

	inline bool isPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
			p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};

struct BoundingSphere
{
	BoundingSphere() { _radius = 0.0f; }

	D3DXVECTOR3 _center;
	float		_radius;
};



#endif //GEOMETRY_H
