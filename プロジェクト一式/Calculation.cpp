#include "main.h"
#include "renderer.h"
#include "Calculation.h"

#define	D3DX_PI2	(D3DX_PI * 2.0f)

//�p�x�̐��`���
float LerpRadian(double a, double b, float t)
{
	//t�̕␳
	t = std::min(std::max(t, 0.0f), 1.0f);

	return NormalizeRadian(a + t * NormalizeRadian(b - a));
}

//���K��
float NormalizeRadian(double rad)
{
	//2�΂����
	int num = static_cast<int>(rad / D3DX_PI2);
	float temp = static_cast<float>(rad) - (D3DX_PI2 * num);

	AdjustmentRadian(temp);

	return temp;
}

//�p�x��-3.14�`3.14�̊Ԃɕ␳
float AdjustmentRadian(float value)
{
	if (value < -D3DX_PI)
	{
		value += D3DX_PI2;
	}

	if (value > D3DX_PI)
	{
		value -= D3DX_PI2;
	}

	return value;
}

//�����v��
float MeasureDist(D3DXVECTOR3 my, D3DXVECTOR3 target)
{
	D3DXVECTOR3 dir;
	dir = my - target;
	float length = D3DXVec3Length(&dir);
	return length;
}

//�l�̐��`���
float LerpValue(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}

//�Q�̃x�N�g��AB�̂Ȃ��p�x�Ƃ����߂�
int iAngleOf2Vector(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float v1Length = D3DXVec3Length(&v1);
	float v2Length = D3DXVec3Length(&v2);

	float cosTheta = D3DXVec3Dot(&v1, &v2) / (v1Length * v2Length);

	float Theta = static_cast<float>(acos(cosTheta));

	Theta = static_cast<float>(Theta * 180.0f)/ D3DX_PI;

	return static_cast<int>(Theta);
}

float fAngleOf2Vector(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float v1Length = D3DXVec3Length(&v1);
	float v2Length = D3DXVec3Length(&v2);

	float cosTheta = D3DXVec3Dot(&v1, &v2) / (v1Length * v2Length);

	float Theta = static_cast<float>(acos(cosTheta));

	return Theta;
}

D3DXMATRIX CalcLookAtMatrix(D3DXMATRIX pout, D3DXVECTOR3 pPos, D3DXVECTOR3 pLook, D3DXVECTOR3 pUp)
{
	D3DXVECTOR3 X, Y, Z;
	Z = pLook - pPos;
	D3DXVec3Normalize(&Z, &Z);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &pUp), &Z);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));


	pout._11 = X.x; pout._12 = X.y; pout._13 = X.z; pout._14 = 0;
	pout._21 = Y.x; pout._22 = Y.y; pout._23 = Y.z; pout._24 = 0;
	pout._31 = Z.x; pout._32 = Z.y; pout._33 = Z.z; pout._34 = 0;
	pout._41 = 0.0f; pout._42 = 0.0f; pout._43 = 0.0f; pout._44 = 1.0f;

	return pout;
}