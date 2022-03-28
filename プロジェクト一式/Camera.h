#pragma once
#include "GameObject.h"

#define CAMERA_DIST (-5.0f)

class Camera : public GameObject
{
private:

	static class Camera* m_Camera;	//����class�Ƃ���Ɠǂݍ��߂�B�O���錾�Ƃ���
	D3DXVECTOR3 m_Target;		//�����_
	D3DXQUATERNION m_Quaternion;


	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;
	D3DXVECTOR3   m_CameraMove;
	D3DXVECTOR3   m_CameraRayPos;
	float	m_Dist;				//�J�����ƒ����_�̋���
	D3DXVECTOR3 m_OldRot;

public:
	//Polygon2D();	//����͕s�v
	//~Polygon2D();

	void Init();
	void UnInit();
	void Update();
	void Draw();
	void UpdateOBB();

	bool CheckView(D3DXVECTOR3 pos, OBB* obb);

	void SetTargetPos(float posX, float posY, float posZ)
	{
		m_Target.x = posX;
		m_Target.y = posY;
		m_Target.z = posZ;
	}

	D3DXVECTOR3 GetTargetPos()
	{
		return m_Target;
	}

	D3DXVECTOR3 GetCameraRayPos()
	{
		return m_CameraRayPos;
	}

	D3DXVECTOR3 GetCameraForward()
	{
		return *D3DXVec3Normalize(&D3DXVECTOR3(m_Target - m_Position), &D3DXVECTOR3(m_Target - m_Position));
	}

	float GetCameraDist()
	{
		return m_Dist;
	}

	Camera* GetCamera()
	{
		return m_Camera;
	}

	D3DXMATRIX GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	D3DXMATRIX GetProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}
};