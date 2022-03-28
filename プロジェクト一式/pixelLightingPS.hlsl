#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4		normal = normalize(In.Normal);	//�s�N�Z���̖@���𐳋K��
	float		light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);	//���邳���v�Z

	//���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);	//�e�N�X�`�������邳�����_�F
	outDiffuse.a *= In.Diffuse.a;				//���_�F�̃������̂܂܎g��

	//�X�y�L�����[�i�t�H�����ʔ��ˁj
	float3	eyev = In.WorldPosition.xyz - CameraPosition.xyz;	//�����x�N�g��
	eyev = normalize(eyev);										//���K������

	float3	refv = reflect(Light.Direction.xyz, normal.xyz);	//���̔��˃x�N�g��
	refv = normalize(refv);										//���K��

	float	specular = -dot(eyev, refv);						//�����Ɣ��˃x�N�g���̓���
	specular = clamp(specular, 0.0f, 0.5f);

	outDiffuse.rgb += specular;									//�X�y�L�����l�����Z����
	outDiffuse.rgb += Material.Emission;
}