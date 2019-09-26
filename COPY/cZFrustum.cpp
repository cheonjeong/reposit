#include "stdafx.h"
#include "cZFrustum.h"

#define PLANE_EPSILONE 0.0001f

cZFrustum::cZFrustum()
{
}

cZFrustum::~cZFrustum()
{
}


bool cZFrustum::Draw(LPDIRECT3DDEVICE9 pDev)
{
	WORD index[] = { 
		0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		3, 7, 6,
		3, 6, 2 };

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	typedef struct tagVTX
	{
		D3DXVECTOR3 p;
	} VTX;
	VTX vtx[8];
	for (int i = 0; i<8; i++)
		vtx[i].p = m_vtx[i];
	pDev->SetFVF(D3DFVF_XYZ);
	pDev->SetStreamSource(0, NULL, 0, sizeof(VTX));
	pDev->SetTexture(0, NULL);
	pDev->SetIndices(0);
	pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// 파란색으로 상, 하 평면을 그린다.
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	pDev->SetMaterial(&mtrl);
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
	// 녹색으로 좌, 우 평면을 그린다
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	pDev->SetMaterial(&mtrl);
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
	// 붉은색으로 원, 근 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	pDev->SetMaterial(&mtrl);
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return TRUE;
}

void cZFrustum::BuildViewFrustum()
{
	D3DXMATRIX viewProjection;
	D3DXMATRIXA16 matView, matProj;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixMultiply(&viewProjection, &matView, &matProj);
	
	// Left plane
	m_plane[0].a = viewProjection._14 + viewProjection._11;
	m_plane[0].b = viewProjection._24 + viewProjection._21;
	m_plane[0].c = viewProjection._34 + viewProjection._31;
	m_plane[0].d = viewProjection._44 + viewProjection._41;

	// Right plane
	m_plane[1].a = viewProjection._14 - viewProjection._11;
	m_plane[1].b = viewProjection._24 - viewProjection._21;
	m_plane[1].c = viewProjection._34 - viewProjection._31;
	m_plane[1].d = viewProjection._44 - viewProjection._41;

	// Top plane
	m_plane[2].a = viewProjection._14 - viewProjection._12;
	m_plane[2].b = viewProjection._24 - viewProjection._22;
	m_plane[2].c = viewProjection._34 - viewProjection._32;
	m_plane[2].d = viewProjection._44 - viewProjection._42;

	// Bottom plane
	m_plane[3].a = viewProjection._14 + viewProjection._12;
	m_plane[3].b = viewProjection._24 + viewProjection._22;
	m_plane[3].c = viewProjection._34 + viewProjection._32;
	m_plane[3].d = viewProjection._44 + viewProjection._42;

	// Near plane
	m_plane[4].a = viewProjection._13;
	m_plane[4].b = viewProjection._23;
	m_plane[4].c = viewProjection._33;
	m_plane[4].d = viewProjection._43;

	// Far plane
	m_plane[5].a = viewProjection._14 - viewProjection._13;
	m_plane[5].b = viewProjection._24 - viewProjection._23;
	m_plane[5].c = viewProjection._34 - viewProjection._33;
	m_plane[5].d = viewProjection._44 - viewProjection._43;

	// Normalize planes
	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&m_plane[i], &m_plane[i]);
	}
}

bool cZFrustum::SphereInFrustum(D3DXVECTOR3 * pV, float Radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_plane[i], pV) + Radius < 0)
		{
			// Outside the frustum, reject it!
			return FALSE;
		}
	}
	return TRUE;
}
