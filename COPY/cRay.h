#pragma once

class cRay
{
public:
	cRay();
	~cRay();
public:
	cRay CalcPickingRay(D3DXVECTOR2 getpos);

public:
	D3DXVECTOR3	_Origin;
	D3DXVECTOR3	_direction;

public:
	D3DXMATRIXA16	GetProj() { return m_proj; }

private:
	D3DXMATRIXA16	m_proj;

};