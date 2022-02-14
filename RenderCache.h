#ifndef RENDER_CACHE_H
#define RENDER_CACHE_H


class RenderCache
{
public:
	RenderCache( IDirect3DDevice9 *device, Material *material );
	virtual ~RenderCache();

	void AddFace();
	void Prepare( unsigned long totalVertices );

	void Begin();
	void RenderFace( unsigned short vertex0, unsigned short vertex1, unsigned short vertex2 );
	void End();

	Material *GetMaterial();

private:
	IDirect3DDevice9*      m_device; 	// Direct3D�f�o�C�X�ւ̃|�C���^�[�B
	Material*	       m_material; 	// �}�e���A���ւ̃|�C���^�[�B

	IDirect3DIndexBuffer9* m_indexBuffer;   // �C���f�b�N�X�o�b�t�@�B
	unsigned short*	       m_indexPointer;  // �C���f�b�N�X�o�b�t�@�ɃA�N�Z�X���邽�߂̃|�C���^�B
	unsigned long	       m_totalIndices;  // �����ł���C���f�b�N�X�̑����B
	unsigned long	       m_faces; 	// �`�悳���ʂ̑����B
	unsigned long	       m_totalVertices; // ���_�̑����B
};

#endif