#pragma once

#include "GameObject.h"

#define ANI_NAME_IDLE	 "Idle"
#define ANI_NAME_SLOWRUN "SlowRun"
#define ANI_NAME_FASTRUN "FastRun"
#define ANI_NAME_BACKRUN "BackRun"
#define ANI_NAME_JUMP	 "Jump"
#define ANI_NAME_FLY	 "Fly"
#define PLAYER_WALKSPEED (0.075f)
#define PLAYER_DASHSPEED (0.125f)
#define PLAYER_WIREACTIONSPEED (0.5f)
#define MAX_WIRE_ACTION_DEGREE (180)
#define PLAYER_ROTATE_VALUE (0.075f)
#define MAX_PLAYER_HP (2)
#define PLAYER_MASS (1.0f)

enum ANIMATION_TYPE
{
	ANI_IDLE,
	ANI_SLOWRUN,
	ANI_FASTRUN,
	ANI_JUMP,
	ANI_BACKRUN,
	ANI_FASTSLOWRUN,
	ANI_FLY
};

class Player : public GameObject
{
private:
	class AnimationModel* m_Model;
	class Audio* m_WireSE;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_OldPosition;
	D3DXVECTOR3 m_SlipVector;
	D3DXVECTOR3	m_TargetPos;
	int m_Frame;
	int m_HP;
	float	m_PlayerSpd;
	float	m_WireActionSpd;
	float	m_GravityForce;
	float	m_BlendRate;
	float	m_JumpRate;
	float	m_JumpValue;
	float	m_DashRate;
	float	m_RecentRayDistance = 1000.0f;
	bool	m_isWireUse;
	bool	m_isJumpActive;
	bool	m_isJumpEnd;
	bool	m_isFly;
	bool	m_isCol;
	bool	m_isGravity;
	bool	m_isOver;
	const char* m_AniName;
	const char* m_OldAniName;

	ANIMATION_TYPE m_Type;
	ANIMATION_TYPE m_OldType;
	D3DXMATRIX m_PlayerMatrix;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	void UpdateHeight();
	void UpdateMove();
	void CheckSlide();
	void CheckAnimation();
	void UpdateAnimation();
	void UpdateOBB();
	void UpdateWire();
	void UpdateTitleMove();

	D3DXMATRIX GetPlayerMatrix()
	{
		return m_PlayerMatrix;
	}

	D3DXVECTOR3 GetOldPos()
	{
		return m_OldPosition;
	}

	D3DXVECTOR3 GetTargetPos()
	{
		return m_TargetPos;
	}

	bool GetisCol()
	{
		return m_isCol;
	}

	bool GetisFly()
	{
		return m_isFly;
	}

	bool GetisGravity()
	{
		return m_isGravity;
	}

	bool GetisWire()
	{
		return m_isWireUse;
	}

	void SetTargetPos(D3DXVECTOR3 pos)
	{
		m_TargetPos = pos;
	}

	void SetSlipVector(D3DXVECTOR3 dir)
	{
		m_SlipVector = dir;
	}

	int GetAniType()
	{
		return (int)m_Type;
	}

	bool GetisOver()
	{
		return m_isOver;
	}

	void SetisOver(bool enable)
	{
		m_isOver = enable;
	}

	int GetHP()
	{
		return m_HP;
	}

	void SetHP(int hp)
	{
		m_HP = hp;
	}

	float GetRecentRayDistance()
	{
		return m_RecentRayDistance;
	}

	void SetRecentRayDistance(float dist)
	{
		m_RecentRayDistance = dist;
	}
};