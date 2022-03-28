#pragma once

#define GRAVITY (-0.098f)

class GameObject
{
private:

protected:	//継承先のクラスからはアクセスできる
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Direction;
	float	    m_Speed;
	float		m_Radius;

	bool		m_Destroy;
	class OBB* m_obb = nullptr;

public:

	GameObject() {}	//コンストラクタ
	virtual ~GameObject() {}	//デストラクタ(仮想関数)

	virtual void Init() = 0;	//純粋仮想関数
	virtual void UnInit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	D3DXVECTOR3 GetPos()
	{
		return m_Position;
	}

	D3DXVECTOR3 GetRot()
	{
		return m_Rotation;
	}
	D3DXVECTOR3 GetScale()
	{
		return m_Scale;
	}

	D3DXVECTOR3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}

	D3DXVECTOR3 GetAbusoluteUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, 0.0f, 0.0f);

		D3DXVECTOR3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Right;
		Right.x = rot._11;
		Right.y = rot._12;
		Right.z = rot._13;

		return Right;
	}

	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetDirection()
	{
		return m_Direction;
	}

	float GetRadius()
	{
		return m_Radius;
	}

	void SetPos(float x, float y, float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}

	void SetSpeed(float spd)
	{
		m_Speed = spd;
	}

	void SetAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, float spd, float r)
	{
		m_Position = pos;
		m_Rotation = rot;
		m_Scale = scl;
		m_Speed = spd;
		m_Radius = r;
	}

	void SetRot(float x, float y, float z)
	{
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	}

	void SetScale(float x, float y, float z)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}

	void SetDestroy()
	{
		m_Destroy = true;
	}

	void SetDirection(D3DXVECTOR3 dir)
	{
		m_Direction = dir;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			UnInit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	OBB* GetOBB()
	{
		return m_obb;
	}
};
