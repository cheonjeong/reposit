#include "stdafx.h"
#include "cSphere.h"
#include"cPickApp.h"




cSphere::~cSphere()
{
}

cSphere::cSphere(D3DXVECTOR3 pos)
	: m_pos(pos)
	, m_pMeshSphere(NULL)
{
}

void cSphere::SetUp()
{
	_Radius = 0.5f;
	D3DXCreateSphere(
		g_pD3DDevice,
		_Radius, 10, 10, &m_pMeshSphere, NULL);

	ZeroMemory(&m_Sphere, sizeof(D3DMATERIAL9));
	m_Sphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_Sphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_Sphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
}

void cSphere::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetTexture(0, NULL);

	D3DXMATRIXA16	matWorld, matS, matR, matT;
	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixTranslation(&matWorld, m_pos.x, m_pos.y, m_pos.z);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	g_pD3DDevice->SetMaterial(&m_Sphere);
	m_pMeshSphere->DrawSubset(0);
}

