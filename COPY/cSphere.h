#pragma once
class cPickApp;
class cSphere
{
public:
	//explicit cSphere();
	~cSphere();
	explicit cSphere(D3DXVECTOR3 pos);

public:
	D3DMATERIAL9			m_Sphere;

public:
	void SetUp(float Radius = 0.5f);
	void Render();

private:
	LPD3DXMESH				m_pMeshSphere;

public:
	D3DXVECTOR3			_Center;
	float				_Radius;

private:
	cPickApp*			m_Picking;

	D3DXVECTOR3			m_pos;
public:
	D3DXVECTOR3	Getpos() { return m_pos; }
	float GetRadius() { return _Radius; }
};

