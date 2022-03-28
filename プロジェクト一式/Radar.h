#pragma once

#define RADAR_MAIN_TEX "asset/texture/Radar_main.png"
#define RADAR_DOT_TEX "asset/texture/white.jpg"

#include "GameObject.h"
#include "RadarMain.h"
#include "RadarDot.h"


class Radar : public GameObject
{
private:
	bool m_isSetCoin;
public:
	
	void Init();
	void UnInit();
	void Update();
	void Draw();

	void UpdateDotPos();
	void SetCoinDot();
};