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
	// �`�悳��钸�_�̍��v��ݒ肵�܂��B
	m_totalVertices = totalVertices;

	// �C���f�b�N�X�o�b�t�@���쐬���܂��B
	m_device->CreateIndexBuffer( m_totalIndices * sizeof( unsigned short ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, NULL );
}


void RenderCache::Begin()
{
	m_indexBuffer->Lock( 0, 0, (void**)&m_indexPointer, 0 );

	m_faces = 0;
}

//-----------------------------------------------------------------------------
// �`�悷���̃C���f�b�N�X��ǉ����܂��B
//-----------------------------------------------------------------------------
void RenderCache::RenderFace( unsigned short vertex0, unsigned short vertex1, unsigned short vertex2 )
{
	*m_indexPointer++ = vertex0;
	*m_indexPointer++ = vertex1;
	*m_indexPointer++ = vertex2;

	m_faces++;
}

//-----------------------------------------------------------------------------
// �L���b�V�������ł����A�ʂ�`�悵�܂��B
//-----------------------------------------------------------------------------
void RenderCache::End()
{
	// �C���f�b�N�X�o�b�t�@�̃��b�N���������܂��B
	m_indexBuffer->Unlock();

	// �`�悷��ʂ����邩�ǂ������m�F���܂��B
	if( m_faces == 0 )
		return;

	// �}�e���A�����t�H�O�𖳎�����K�v�����邩�ǂ������m�F���܂��B
	if( m_material->GetIgnoreFog() == true )
		m_device->SetRenderState( D3DRS_FOGENABLE, false );

	// �}�e���A���ƃe�N�X�`����ݒ肵�܂��B
	m_device->SetMaterial( m_material->GetLighting() );
	m_device->SetTexture( 0, m_material->GetTexture() );

	// �C���f�b�N�X��ݒ肵�܂��B
	m_device->SetIndices( m_indexBuffer );

	// �ʂ�`�悵�܂��B
	m_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_totalVertices, 0, m_faces );

	// �ύX���ꂽ�ꍇ�́A�t�H�O�ݒ�𕜌����܂��B
	if( m_material->GetIgnoreFog() == true )
		m_device->SetRenderState( D3DRS_FOGENABLE, true );
}

//-----------------------------------------------------------------------------
// �g�p����Ă���}�e���A���ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------
Material *RenderCache::GetMaterial()
{
	return m_material;
}