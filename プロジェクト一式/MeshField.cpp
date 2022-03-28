#include "main.h"
#include"renderer.h"
#include "MeshField.h"
#include "Calculation.h"
#include "manager.h"
#include "Player.h"
#include "Camera.h"
#include "WireTarget.h"
#include <DirectXMath.h>
/*
float g_FieldHeight[MAX_MAP_WIDTH + 1][MAX_MAP_HEIGHT + 1] =
{
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 5.0f,  5.0f , 5.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,},
	{0.0f, 1.0f, 2.0f, 4.0f, 7.5f, 3.0f, 1.5f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,}
};

//���̕��RMAP
float g_FieldHeight1[MAX_MAP_WIDTH + 1][MAX_MAP_HEIGHT + 1] =
{
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
	{8.0f, 8.0f, 8.0f, 8.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,}
};
*/
void MeshField::Init()
{
	//���_�o�b�t�@����
	{
		Scene* scene = Manager::GetScene();
		SCENE_NAME name = scene->GetSceneName();

		for (int x = 0; x <= MAX_MAP_WIDTH; x++)
		{
			for (int z = 0; z <= MAX_MAP_HEIGHT; z++)
			{
				m_Vertex[x][z].Position = D3DXVECTOR3((x - (MAX_MAP_WIDTH / 2)) * 5.0f, 0.0f, (z - (MAX_MAP_HEIGHT / 2)) * -5.0f);	//���R�ȏ�
				if (x < 15 && z > 25)
				{
					m_Vertex[x][z].Position.y = 3.0f;
				}				
				if (name == SCENE_TITLE)
				{
					m_Vertex[x][z].Position.y = 0.0f;
				}
				m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//�@���x�N�g��
				m_Vertex[x][z].Diffuse = COLOR_WHITE;
				m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		//�@���x�N�g���Z�o
		for (int x = 1; x <= MAX_MAP_WIDTH - 1; x++)
		{
			for (int z = 1; z <= MAX_MAP_HEIGHT - 1; z++)
			{
				D3DXVECTOR3 vx, vz, vn;

				vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;	//�E���獶
				vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;	//�������O

				D3DXVec3Cross(&vn, &vz, &vx);//�O��
				D3DXVec3Normalize(&vn, &vn);//���K���i����1�ɂ���j
				m_Vertex[x][z].Normal = vn;
			}
		}


		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * (MAX_MAP_WIDTH + 1) * (MAX_MAP_HEIGHT + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//�C���f�b�N�X�o�b�t�@����
	{
		unsigned int index[((MAX_MAP_WIDTH + 2) * 2) * MAX_MAP_HEIGHT - 2];

		int i = 0;
		for (int x = 0; x < MAX_MAP_WIDTH; x++)
		{
			for (int z = 0; z < MAX_MAP_HEIGHT + 1; z++)
			{
				index[i] = x * (MAX_MAP_WIDTH + 1) + z;
				i++;

				index[i] = (x + 1) * (MAX_MAP_HEIGHT + 1) + z;
				i++;
			}

			if (x == MAX_MAP_WIDTH - 1)
				break;

			index[i] = (x + 1) * (MAX_MAP_WIDTH + 1) + MAX_MAP_HEIGHT;
			i++;

			index[i] = (x + 1) * (MAX_MAP_HEIGHT + 1);
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((MAX_MAP_WIDTH + 2) * 2) * MAX_MAP_HEIGHT - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	//�e�N�X�`���̓ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Asphalt.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);//�e�N�X�`������肭�ǂݍ��܂�Ă邩�m�F

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");//�摜���R���p�C�����ĕ\��
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void MeshField::UnInit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void MeshField::Update()
{


}

void MeshField::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�i2D�`��p�|���S���j
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g���|�W�ݒ�i�ǂ̂悤�Ƀ|���S����`�����j
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed(((MAX_MAP_WIDTH + 2) * 2) * MAX_MAP_HEIGHT - 2, 0, 0);//���_��
}

float MeshField::GetHeight(D3DXVECTOR3 pos)
{
	int x, z;

	//�u���b�N�ԍ��Z�o
	x = static_cast<int>(pos.x / 5.0f + (MAX_MAP_WIDTH / 2));
	z = static_cast<int>(pos.z / -5.0f + (MAX_MAP_HEIGHT / 2));

	D3DXVECTOR3 pos0, pos1, pos2, pos3;	//�����4���_�̍��W

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;	//�O�p�`�㑤
	v1p = pos - pos1;	//�O�p�`����

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
		D3DXVec3Normalize(&n, &n);
	}
	else
	{
		//�E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
		D3DXVec3Normalize(&n, &n);
	}

	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

D3DXVECTOR3 MeshField::RightVec(D3DXVECTOR3 pos, D3DXVECTOR3 v1)
{
	int x, z;

	//�u���b�N�ԍ��Z�o
	x = static_cast<int>(pos.x / 5.0f + (MAX_MAP_WIDTH / 2));
	z = static_cast<int>(pos.z / -5.0f + (MAX_MAP_HEIGHT / 2));

	D3DXVECTOR3 pos0, pos1, pos2, pos3;	//�����4���_�̍��W

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;	//�O�p�`�㑤
	v1p = pos - pos1;	//�O�p�`����

	D3DXVec3Cross(&c, &v12, &v1p);

	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
		D3DXVec3Normalize(&n, &n);
	}
	else
	{
		//�E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
		D3DXVec3Normalize(&n, &n);
	}

	D3DXVECTOR3 v = v1;
	D3DXVECTOR3 tmp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//tmp��n�̊p�x�O�܂��͂���ɋ߂��Ȃ�A�ʂȃx�N�g����p��
	if (iAngleOf2Vector(tmp, n) < 1) //0.00174533rad=1degree
	{
		tmp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	//�O�ς����߂�
	D3DXVec3Cross(&v, &n, &tmp);
	D3DXVec3Cross(&v, &v, &n);

	return -v;	//�}�C�i�X�����ō⓹�����
}

//�t�B�[���h�͈͓̔��ɂ���悤�␳����֐�
D3DXVECTOR3 MeshField::MeshFieldWithinRange(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos = pos;

	Pos.x = std::fmax(Pos.x, -(MAX_MAP_WIDTH / 2) * 5.0f + 0.1f);
	Pos.x = std::fmin(Pos.x, (MAX_MAP_WIDTH / 2) * 5.0f - 0.1f);
					  
	Pos.z = std::fmax(Pos.z, -(MAX_MAP_HEIGHT / 2) * 5.0f + 0.1f);
	Pos.z = std::fmin(Pos.z, (MAX_MAP_HEIGHT / 2) * 5.0f - 0.1f);

	return Pos;
}