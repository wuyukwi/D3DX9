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
	IDirect3DDevice9*      m_device; 	// Direct3Dデバイスへのポインター。
	Material*	       m_material; 	// マテリアルへのポインター。

	IDirect3DIndexBuffer9* m_indexBuffer;   // インデックスバッファ。
	unsigned short*	       m_indexPointer;  // インデックスバッファにアクセスするためのポインタ。
	unsigned long	       m_totalIndices;  // 処理できるインデックスの総数。
	unsigned long	       m_faces; 	// 描画される面の総数。
	unsigned long	       m_totalVertices; // 頂点の総数。
};

#endif