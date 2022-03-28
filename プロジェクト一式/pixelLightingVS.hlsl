#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	//�s��v�Z�p
	matrix wvp;
	//�����Œ��_�ϊ�
	wvp = mul(World, View);//wvp = ���[���h�s�� * �J�����s��
	wvp = mul(wvp, Projection);//wvp = wvp * �v���W�F�N�V�����s��
	//���_���W���o��
	Out.Position = mul(In.Position, wvp);//�ϊ����ʂ��o�͂���

	float4 WorldNormal, normal;
	//�@���x�N�g����w��0�Ƃ��ăR�s�[(���s�ړ����Ȃ�����)
	normal = float4(In.Normal.x, In.Normal.y, In.Normal.z, 0.0f);
	//�@�������[���h�s��ŉ�]����
	WorldNormal = mul(normal, World);
	//��]��̖@���𐳋K������
	WorldNormal = normalize(WorldNormal);
	//��]��̖@���o��
	Out.Normal = WorldNormal;

	//���_�̃f�t���[�Y�o��
	Out.Diffuse = In.Diffuse;

	//�e�N�X�`�����W���o��
	Out.TexCoord = In.TexCoord;//

	Out.WorldPosition = mul(In.Position, World);//���[���h�ϊ��������_���W���o��
}