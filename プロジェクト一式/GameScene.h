#pragma once

#include "Scene.h"


class GameScene : public Scene
{
private:
	class Fade* m_Fade;
	class Audio* m_BGM;
	bool	m_isClear;
public:
	void Init();
	void UnInit();
	void Update();

	void SetisClearFlg(bool enable)
	{
		m_isClear = enable;
	}
};