#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	//行列計算用
	matrix wvp;
	//ここで頂点変換
	wvp = mul(World, View);//wvp = ワールド行列 * カメラ行列
	wvp = mul(wvp, Projection);//wvp = wvp * プロジェクション行列
	//頂点座標を出力
	Out.Position = mul(In.Position, wvp);//変換結果を出力する

	float4 WorldNormal, normal;
	//法線ベクトルのwを0としてコピー(平行移動しないため)
	normal = float4(In.Normal.x, In.Normal.y, In.Normal.z, 0.0f);
	//法線をワールド行列で回転する
	WorldNormal = mul(normal, World);
	//回転後の法線を正規化する
	WorldNormal = normalize(WorldNormal);
	//回転後の法線出力
	Out.Normal = WorldNormal;

	//頂点のデフューズ出力
	Out.Diffuse = In.Diffuse;

	//テクスチャ座標を出力
	Out.TexCoord = In.TexCoord;//

	Out.WorldPosition = mul(In.Position, World);//ワールド変換した頂点座標を出力
}