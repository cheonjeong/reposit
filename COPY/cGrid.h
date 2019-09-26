#pragma once

class cPyramid;
class cGrid
{
public:
	cGrid();
	~cGrid();

private:
	vector<cPyramid*> m_vecPyramid;
public:
	void Setup(int nNumHalfTile = 15, float fInterval = 1.0f);
	void Render();
private:
	int								m_nNumLine;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
};