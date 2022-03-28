#pragma once

#include <DirectXCollision.h>
#include <iostream>
#include <algorithm>

#define VALUE_ROTATE (D3DX_PI * 0.01f)


float LerpRadian(double a, double b, float t);
float AdjustmentRadian(float value);
float MeasureDist(D3DXVECTOR3 my, D3DXVECTOR3 target);
float LerpValue(float a, float b, float t);

float NormalizeRadian(double rad);

int iAngleOf2Vector(D3DXVECTOR3 A, D3DXVECTOR3 B);
float fAngleOf2Vector(D3DXVECTOR3 A, D3DXVECTOR3 B);