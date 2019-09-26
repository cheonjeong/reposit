#pragma once
#define g_pZFrustum cZFrustum::GetInstance()
class cZFrustum
{
private:
	D3DXVECTOR3 m_vtx[8];
	D3DXVECTOR3 m_vPos;
	D3DXPLANE	m_plane[6];
public:

	bool	Draw(LPDIRECT3DDEVICE9 pDev);
	D3DXVECTOR3*	Getpos() { return &m_vPos; }
private:
	SINGLETONE(cZFrustum);
public:
	void BuildViewFrustum();
	bool SphereInFrustum(D3DXVECTOR3 * pV, float Radius);
};

/*
d3dxplanefromepoints();
d3dxplanedotcoord();

d3dxvec3unproject();
*/

