#include "Engine.h"


RenderCache::RenderCache( IDirect3DDevice9 *device, Material *material )
{
	m_device = device;

	m_material = material;

	m_indexBuffer = NULL;
	m_totalIndices = 0;
}


RenderCache::~RenderCache()
{
	SAFE_RELEASE( m_indexBuffer );
}


void RenderCache::AddFace()
{
	m_totalIndices += 3;
}


void RenderCache::Prepare( unsigned long totalVertices )
{
	// 描画される頂点の合計を設定します。
	m_totalVertices = totalVertices;

	// インデックスバッファを作成します。
	m_device->CreateIndexBuffer( m_totalIndices * sizeof( unsigned short ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, NULL );
}


void RenderCache::Begin()
{
	m_indexBuffer->Lock( 0, 0, (void**)&m_indexPointer, 0 );

	m_faces = 0;
}

//-----------------------------------------------------------------------------
// 描画する顔のインデックスを追加します。
//-----------------------------------------------------------------------------
void RenderCache::RenderFace( unsigned short vertex0, unsigned short vertex1, unsigned short vertex2 )
{
	*m_indexPointer++ = vertex0;
	*m_indexPointer++ = vertex1;
	*m_indexPointer++ = vertex2;

	m_faces++;
}

//-----------------------------------------------------------------------------
// キャッシュ準備できた、面を描画します。
//-----------------------------------------------------------------------------
void RenderCache::End()
{
	// インデックスバッファのロックを解除します。
	m_indexBuffer->Unlock();

	// 描画する面があるかどうかを確認します。
	if( m_faces == 0 )
		return;

	// マテリアルがフォグを無視する必要があるかどうかを確認します。
	if( m_material->GetIgnoreFog() == true )
		m_device->SetRenderState( D3DRS_FOGENABLE, false );

	// マテリアルとテクスチャを設定します。
	m_device->SetMaterial( m_material->GetLighting() );
	m_device->SetTexture( 0, m_material->GetTexture() );

	// インデックスを設定します。
	m_device->SetIndices( m_indexBuffer );

	// 面を描画します。
	m_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_totalVertices, 0, m_faces );

	// 変更された場合は、フォグ設定を復元します。
	if( m_material->GetIgnoreFog() == true )
		m_device->SetRenderState( D3DRS_FOGENABLE, true );
}

//-----------------------------------------------------------------------------
// 使用されているマテリアルへのポインタを返します。
//-----------------------------------------------------------------------------
Material *RenderCache::GetMaterial()
{
	return m_material;
}