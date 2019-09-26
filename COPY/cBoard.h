#pragma once

class cRay;


class cBoard
{
public:
	cBoard();
	~cBoard();
	
public:
	void Setup(D3DXVECTOR3 scale);
	void Render();

private:
	vector<ST_PNT_VERTEX>				m_vecVertex;
	D3DXMATRIXA16						m_matWorld, m_matS, m_matT;

public:
	D3DXVECTOR3 progress(cRay * ray);
};