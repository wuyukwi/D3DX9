#include "Main.h"

Menu* Menu::m_instance = NULL;


Menu* Menu::GetInstance()
{
	if(!m_instance)
		m_instance = new Menu();
	return m_instance;
}


void UpdateSessionsList( HWND window )
{

}



Menu::Menu() : State( STATE_MENU )
{
	// Does nothing but set's the state's ID.
	m_instance = this;
}


void Menu::Update( float elapsed )
{

}