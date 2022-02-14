#ifndef SCRIPTING_H
#define SCRIPTING_H

//-----------------------------------------------------------------------------
// Variable Type Enumeration
//-----------------------------------------------------------------------------
enum{ VARIABLE_BOOL, VARIABLE_COLOUR, VARIABLE_FLOAT, VARIABLE_NUMBER, VARIABLE_STRING, VARIABLE_VECTOR, VARIABLE_UNKNOWN };



//-----------------------------------------------------------------------------
// Variable Class
//-----------------------------------------------------------------------------
class Variable
{
public:
	Variable( char *name, FILE *file );
	Variable( char *name, char type, void *value );
	virtual ~Variable();

	char  GetType();
	char* GetName();
	void* GetData();

private:
	char  m_type; // 変数に格納されているデータのタイプ。
	char* m_name; // 変数の名前。
	void* m_data; // 変数に格納されているデータ。
};


//-----------------------------------------------------------------------------
// Script Class
//-----------------------------------------------------------------------------
class Script : public Resource< Script >
{

private:
	LinkedList< Variable > *m_variables; // スクリプト内の変数のリンクリスト。

public:
	Script( char *name, char *path = "./" );
	virtual ~Script();

	void AddVariable( char *name, char type, void *value );
	void SetVariable( char *name, void *value );

	void SaveScript( char *filename = NULL );

	bool*	       GetBoolData( char *variable );
	D3DCOLORVALUE* GetColourData( char *variable );
	float*	       GetFloatData( char *variable );
	long*	       GetNumberData( char *variable );
	char*	       GetStringData( char *variable );
	D3DXVECTOR3*   GetVectorData( char *variable );
	void*	       GetUnknownData( char *variable );
};

#endif