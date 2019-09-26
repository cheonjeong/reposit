#include "stdafx.h"
#include "cRay.h"
#include "func.h"


cRay::cRay()
{
}


cRay::~cRay()
{
}

cRay cRay::CalcPickingRay(D3DXVECTOR2 getpos)
{
	D3DXVECTOR2 pos = { 0.0f, 0.0f };

	D3DVIEWPORT9	vp;
	g_pD3DDevice->GetViewport(&vp);

	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &m_proj);

	pos.x = (((2.0f * getpos.x) / vp.Width) - 1.0f) / m_proj(0, 0);
	pos.y = (((-2.0f * getpos.y) / vp.Height) + 1.0f) / m_proj(1, 1);

	cRay ray;
	ray._Origin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction  = D3DXVECTOR3(pos.x, pos.y, 1.0f);

	return ray;
}