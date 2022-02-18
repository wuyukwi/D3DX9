/********************************************************************************* 

  *FileName: camera.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/02/1

**********************************************************************************/  


#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
public:
	enum CameraType { FOLLOW, FREE };

	Camera();
	Camera(CameraType cameraType);
	~Camera();
	
	void Update(const float& elapsed);
	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	
	D3DXVECTOR3* getPosition();
	D3DXVECTOR3* getRight();
	D3DXVECTOR3* getUp();
	D3DXVECTOR3* getLook();

	D3DXMATRIX* GetViewMatrix();

private:
	D3DXMATRIXA16 _matView;
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;

	void imguiUpdate(bool* p_open);
};

#endif // CAMERA_H_