#ifndef RESOURCE_MANAGEMENT_H
#define RESOURCE_MANAGEMENT_H

//-----------------------------------------------------------------------------
// リソースクラス
//-----------------------------------------------------------------------------
template< class Type > 
class Resource
{
private:
	char*	      m_name;     // リソースのファイル名
	char*	      m_path;	  // リソースのパス
	char*	      m_filename; // リソースの(ファイル名⁺パス)
	unsigned long m_refCount; // 参照カウント

public:
	Resource( char *name, char *path = "./" )
	{
		// ファイル名を保存
		if( name != NULL )
		{
			m_name = new char[strlen( name ) + 1];
			memcpy( m_name, name, ( strlen( name ) + 1 ) * sizeof( char ) );
		}

		// パスを保存
		if( path != NULL )
		{
			m_path = new char[strlen( path ) + 1];
			memcpy( m_path, path, ( strlen( path ) + 1 ) * sizeof( char ) );
		}

		// 参照を作成
		if( name != NULL && path != NULL )
		{
			m_filename = new char[strlen( name ) + strlen( path ) + 1];
			sprintf( m_filename, "%s%s", path, name );
		}

		// 参照カウント
		m_refCount = 1;
	}

	virtual ~Resource()
	{
		SAFE_DELETE_ARRAY( m_name );
		SAFE_DELETE_ARRAY( m_path );
		SAFE_DELETE_ARRAY( m_filename );
	}

	char *GetName()
	{
		return m_name;
	}

	char *GetPath()
	{
		return m_path;
	}

	char *GetFilename()
	{
		return m_filename;
	}

	void IncRef()
	{
		m_refCount++;
	}

	void DecRef()
	{
		m_refCount--;
	}

	unsigned long GetRefCount()
	{
		return m_refCount;
	}
};


//-----------------------------------------------------------------------------
// リソースマネージャークラス
//-----------------------------------------------------------------------------
template< class Type > 
class ResourceManager
{
private:
	LinkedList<Type>* m_list; // リソースのリンクリスト

	void (*CreateResource)( Type **resource, char *name, char *path ); // アプリケーション固有のリソース作成

public:
	ResourceManager( void (*CreateResourceFunction)( Type **resource, char *name, char *path ) = NULL )
	{
		m_list = new LinkedList< Type >;

		CreateResource = CreateResourceFunction;
	}

	~ResourceManager()
	{
		SAFE_DELETE( m_list );
	}

	Type *Add( char *name, char *path = "./" )
	{
		// リスト、リソース名、パスが有効であることを確認する。
		if( m_list == NULL || name == NULL || path == NULL )
			return NULL;

		// エレメントが既に存在する場合は、そのポインタを返す
		Type *element = GetElement( name, path );
		if( element != NULL )
		{
			element->IncRef();
			return element;
		}

		// リソースを作成します。可能な場合は、アプリケーション固有の機能を使用
		Type *resource = NULL;
		if( CreateResource != NULL )
			CreateResource( &resource, name, path );
		else
			resource = new Type( name, path );

		// 新しいリソースをマネージャーに追加し、そのリソースへのポインターを返す
		return m_list->Add( resource );
	}

	void Remove( Type **resource )
	{
		// 削除するリソースとリストが有効であることを確認
		if( *resource == NULL || m_list == NULL )
			return;

		// リソースの参照カウントをデクリメントします。
		(*resource)->DecRef();

		// リソースが使用されなくなった場合は、リソースを破棄します。
		if( (*resource)->GetRefCount() == 0 )
			m_list->Remove( resource );
	}

	void EmptyList()
	{
		if( m_list != NULL )
			m_list->Empty();
	}

	LinkedList< Type > *GetList()
	{
		return m_list;
	}

	Type *GetElement( char *name, char *path = "./" )
	{
		// リスト、リソース名、パスが有効であることを確認する。
		if( name == NULL || path == NULL || m_list == NULL )
			return NULL;
		if( m_list->GetFirst() == NULL )
			return NULL;

		// リストを捜査、指定されたリソースを探します。
		m_list->Iterate( true );
		while( m_list->Iterate() )
			if( strcmp( m_list->GetCurrent()->GetName(), name ) == 0 )
				if( strcmp( m_list->GetCurrent()->GetPath(), path ) == 0 )
					return m_list->GetCurrent();

		// リソースが見つからなかった場合はNULLを返します。
		return NULL;
	}
};

#endif