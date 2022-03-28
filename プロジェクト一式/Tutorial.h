#pragma once

#include "Scene.h"


class Tutorial : public Scene
{
private:
	class Fade* m_Fade;
public:
	void Init();
	void UnInit();
	void Update();
};