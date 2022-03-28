#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4		normal = normalize(In.Normal);	//ピクセルの法線を正規化
	float		light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);	//明るさを計算

	//このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);	//テクスチャ＊明るさ＊頂点色
	outDiffuse.a *= In.Diffuse.a;				//頂点色のαをそのまま使う

	//スペキュラー（フォン鏡面反射）
	float3	eyev = In.WorldPosition.xyz - CameraPosition.xyz;	//視線ベクトル
	eyev = normalize(eyev);										//正規化する

	float3	refv = reflect(Light.Direction.xyz, normal.xyz);	//光の反射ベクトル
	refv = normalize(refv);										//正規化

	float	specular = -dot(eyev, refv);						//視線と反射ベクトルの内積
	specular = clamp(specular, 0.0f, 0.5f);

	outDiffuse.rgb += specular;									//スペキュラ値を加算する
	outDiffuse.rgb += Material.Emission;
}