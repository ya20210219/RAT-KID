#pragma once
#include "Input.h"
#include "GameObject.h"

class DebugSystem : public GameObject
{
private:
	bool m_DebugUse;

public:
	void Init();
	void Update();
	void UnInit();
	void Draw();

	bool GetDebug() { return m_DebugUse; }
	void SetDebug(bool use)
	{
		m_DebugUse = use;
	}
};
