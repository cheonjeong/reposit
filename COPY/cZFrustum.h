#pragma once
class cZFrustum
{
public:
	cZFrustum();
	~cZFrustum();
private:
	D3DXVECTOR3 m_vtx[8];
};

/*
d3dxplanefromepoints();
d3dxplanedotcoord();

d3dxvec3unproject();
*/

