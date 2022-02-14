#ifndef INPUT_H
#define INPUT_H

//-----------------------------------------------------------------------------
// Input Class
//-----------------------------------------------------------------------------
class Input
{
public:
	Input( HWND window );
	virtual ~Input();

	void Update();

	bool GetKeyPress( char key, bool ignorePressStamp = false );
	bool GetButtonPress( char button, bool ignorePressStamp = false );

	long GetPosX();
	long GetPosY();
	long GetDeltaX();
	long GetDeltaY();
	long GetDeltaWheel();

private:
	HWND	       m_window;		   // �e�E�B���h�E�̃n���h���B
	IDirectInput8* m_di;			   // DirectInput�I�u�W�F�N�g�B
	unsigned long  m_pressStamp; 		   // ���݂̃v���X�X�^���v�i�t���[�����ƂɃC���N�������g�j�B

	IDirectInputDevice8* m_keyboard;	   // DirectInput�L�[�{�[�h�f�o�C�X�B
	char		     m_keyState[256];	   // �L�[�{�[�h�L�[�̏�Ԃ�ۑ��p�B
	unsigned long	     m_keyPressStamp[256]; // �e�L�[�����O�ɃV�[�h���ꂽ�Ō�̃t���[���ɃX�^���v��t���܂��B

	IDirectInputDevice8* m_mouse;		    // DirectInput�}�E�X�f�o�C�X�B
	DIMOUSESTATE	     m_mouseState;	    // �}�E�X�{�^���̏�Ԃ�ۑ����܂��B
	unsigned long	     m_buttonPressStamp[3]; // �e�{�^�����v���V�[�h���ꂽ�Ō�̃t���[���ɃX�^���v�������܂��B
	POINT		     m_position;	    // ��ʏ�̃}�E�X�̈ʒu��ۑ��p�B
};

#endif