#pragma once
class xFileLoader
{
public:
	xFileLoader();
	~xFileLoader();
private:
	LPD3DXMESH							m_pMesh;
	vector<D3DMATERIAL9>				m_vecMtl;
	vector<IDirect3DTexture9*>			m_vecTex;
public:
	bool Setup(char * fileName);
	bool Display(float timeDelta = 0);
};

