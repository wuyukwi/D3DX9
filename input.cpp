#include "Engine.h"

//-----------------------------------------------------------------------------
// Input �N���X�R���X�g���N�^�[�B
//-----------------------------------------------------------------------------
Input::Input( HWND window )
{
	// �e�E�B���h�E�ւ̃n���h����ۑ����܂��B
	m_window = window;

	// DirectInput�C���^�[�t�F�[�X���쐬���܂��B
	DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_di, NULL );

	// �L�[�{�[�h�f�o�C�X���쐬�A�����A����ю擾���܂��B
	m_di->CreateDevice( GUID_SysKeyboard, &m_keyboard, NULL );
	m_keyboard->SetDataFormat( &c_dfDIKeyboard );
	m_keyboard->SetCooperativeLevel( m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_keyboard->Acquire();

	// �}�E�X�f�o�C�X���쐬�A�����A����ю擾���܂��B
	m_di->CreateDevice( GUID_SysMouse, &m_mouse, NULL );
	m_mouse->SetDataFormat( &c_dfDIMouse );
	m_mouse->SetCooperativeLevel( m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_mouse->Acquire();

	// �v���X�X�^���v������
	m_pressStamp = 0;
}

//-----------------------------------------------------------------------------
// Input �N���X �f�X�g���N�^�B
//-----------------------------------------------------------------------------
Input::~Input()
{
	SAFE_RELEASE( m_di );
	SAFE_RELEASE( m_keyboard );
	SAFE_RELEASE( m_mouse );
}

//-----------------------------------------------------------------------------
// �L�[�{�[�h�ƃ}�E�X�f�o�C�X�̏�Ԃ��X�V���܂��B
//-----------------------------------------------------------------------------
void Input::Update()
{
	static HRESULT result;

	// �������邩�A�s���ȃG���[���Ԃ����܂ŁA�L�[�{�[�h���|�[�����O���܂��B
	while( true )
	{
		m_keyboard->Poll();
		if( SUCCEEDED( result = m_keyboard->GetDeviceState( 256, (LPVOID)&m_keyState ) ) )
			break;
		if( result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED )
			return;

		// �t�H�[�J�X������ꂽ�ꍇ�́A�f�o�C�X���Ď擾���܂��B
		if( FAILED( m_keyboard->Acquire() ) )
			return;
	}

	// �������邩�A�s���ȃG���[���Ԃ����܂ŁA�}�E�X���|�[�����O���܂��B
	while( true )
	{
		m_mouse->Poll();
		if( SUCCEEDED( result = m_mouse->GetDeviceState( sizeof( DIMOUSESTATE ), &m_mouseState ) ) )
			break;
		if( result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED )
			return;

		// �t�H�[�J�X������ꂽ�ꍇ�́A�f�o�C�X���Ď擾���܂��B
		if( FAILED( m_mouse->Acquire() ) )
			return;
	}

	// �}�E�X�̑��Έʒu���擾���܂��B
	GetCursorPos( &m_position );
	ScreenToClient( m_window, &m_position );

	// �v���X�X�^���v����
	m_pressStamp++;
}

//-----------------------------------------------------------------------------
// �w�肳�ꂽ�L�[�������ꂽ�ꍇ��true��Ԃ��܂��B
// �v���X�X�^���v���g�p����ƁA��������false��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool Input::GetKeyPress( char key, bool ignorePressStamp )
{
	if( ( m_keyState[key] & 0x80 ) == false )
		return false;

	bool pressed = true;

	if( ignorePressStamp == false )
		if( m_keyPressStamp[key] == m_pressStamp - 1 || m_keyPressStamp[key] == m_pressStamp )
			pressed = false;

	m_keyPressStamp[key] = m_pressStamp;

	return pressed;
}

//-----------------------------------------------------------------------------
// �w�肳�ꂽ�L�[�������ꂽ�ꍇ��true��Ԃ��܂��B
// �v���X�X�^���v���g�p����ƁA��������false��Ԃ��܂��B
//-----------------------------------------------------------------------------
bool Input::GetButtonPress( char button, bool ignorePressStamp )
{
	if( ( m_mouseState.rgbButtons[button] & 0x80 ) == false )
		return false;

	bool pressed = true;

	if( ignorePressStamp == false )
		if( m_buttonPressStamp[button] == m_pressStamp - 1 || m_buttonPressStamp[button] == m_pressStamp )
			pressed = false;

	m_buttonPressStamp[button] = m_pressStamp;

	return pressed;
}

//-----------------------------------------------------------------------------
// �}�E�X��x���W��Ԃ��܂��B
//-----------------------------------------------------------------------------
long Input::GetPosX()
{
	return m_position.x;
}

//-----------------------------------------------------------------------------
// �}�E�X��y���W��Ԃ��܂��B
//-----------------------------------------------------------------------------
long Input::GetPosY()
{
	return m_position.y;
}

//-----------------------------------------------------------------------------
// �}�E�X��x���W�̕ω���Ԃ��܂��B
//-----------------------------------------------------------------------------
long Input::GetDeltaX()
{
	return m_mouseState.lX;
}

//-----------------------------------------------------------------------------
// �}�E�X��y���W�̕ω���Ԃ��܂��B
//-----------------------------------------------------------------------------
long Input::GetDeltaY()
{
	return m_mouseState.lY;
}

//-----------------------------------------------------------------------------
// �}�E�X�̃X�N���[���z�C�[���̕ω���Ԃ��܂��B
//-----------------------------------------------------------------------------
long Input::GetDeltaWheel()
{
	return m_mouseState.lZ;
}

