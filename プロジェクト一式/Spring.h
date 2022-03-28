#pragma once

#include "GameObject.h"
#define DEFAULT_POSITION (10.0f)

class Spring : public GameObject
{
private:
	D3DXVECTOR3 m_Velocity;
	float m_Mass;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
};

