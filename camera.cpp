/********************************************************************************* 

  *FileName: camera.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/01

**********************************************************************************/  
#include "engine.h"

Camera::Camera()
{
	_cameraType = FREE;

	D3DXMatrixIdentity(&_matView);
	_pos = D3DXVECTOR3(10.0f, 1000.9, -1243.2f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 0.8f, 0.6f);
	_look = D3DXVECTOR3(0.0f, -0.6f, 0.8f);


}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

D3DXVECTOR3* Camera::getPosition()
{
	return &this->_pos;
}

D3DXVECTOR3* Camera::getRight()
{
	return &this->_right;
}

D3DXVECTOR3* Camera::getUp()
{
	return &this->_up;
}

D3DXVECTOR3* Camera::getLook()
{
	return &this->_look;
}

void Camera::Update()
{
	Input* pInput = Engine::GetInstance()->GetInput();
	float timeDelta = Engine::GetInstance()->GettimeDelta();

	if (pInput->GetKeyPress(DIK_I, true))		
		this->walk(40.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_K, true))
		this->walk(-40.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_J, true))
		this->strafe(-40.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_L, true))
		this->strafe(40.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_R, true))
		this->fly(40.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_F, true))
		this->fly(-40.0f * timeDelta);

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		this->pitch(10.0f * timeDelta);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		this->pitch(-10.0f * timeDelta);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		this->yaw(-10.0f * timeDelta);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		this->yaw(10.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_N, true))
		this->roll(10.0f * timeDelta);

	if (pInput->GetKeyPress(DIK_M, true))
		this->roll(-10.0f * timeDelta);

	this->getViewMatrix(&_matView);
	Engine::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &_matView);

	static bool show_camera_window;

	ImGui::Checkbox("Camera window", &show_camera_window);

	if(show_camera_window)
	imguiUpdate(&show_camera_window);
}

void Camera::imguiUpdate(bool* p_open)
{
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	ImGui::Begin("Camera window", p_open, window_flags);

	ImGui::Text("Camera Position(%.1f ,%.1f ,%.1f)", _pos.x, _pos.y, _pos.z);
	ImGui::Text("Camera Right(%.1f ,%.1f ,%.1f)", _right.x, _right.y, _right.z);
	ImGui::Text("Camera Up(%.1f ,%.1f ,%.1f)", _up.x, _up.y, _up.z);
	ImGui::Text("Camera Look(%.1f ,%.1f ,%.1f)", _look.x, _look.y, _look.z);

	ImGui::Separator();

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
		if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
		if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
		if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
		if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
		if (p_open && ImGui::MenuItem("Close")) *p_open = false;
		ImGui::EndPopup();
	}
	ImGui::End();
}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	if (_cameraType == FOLLOW)
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if (_cameraType == FREE)
		_pos += _look * units;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if (_cameraType == FOLLOW)
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if (_cameraType == FREE)
		_pos += _right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if (_cameraType == FOLLOW)
		_pos.y += units;

	if (_cameraType == FREE)
		_pos += _up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIXA16 T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIXA16 T;

	// rotate around world y (0, 1, 0) always for land object
	if (_cameraType == FOLLOW)
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for FREE
	if (_cameraType == FREE)
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for FREE type
	if (_cameraType == FREE)
	{
		D3DXMATRIXA16 T;
		D3DXMatrixRotationAxis(&T, &_look, angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right, &_right, &T);
		D3DXVec3TransformCoord(&_up, &_up, &T);
	}
}

void Camera::getViewMatrix(D3DXMATRIXA16* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}