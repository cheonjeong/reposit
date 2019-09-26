#pragma once
class cCubePC
{
public:
	cCubePC();
	~cCubePC();

private:
	std::vector<ST_PC_VERTEX> m_vecVertex;
	D3DXMATRIXA16 m_matWorld;
	float m_fRotY;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vPosition;


public:
	void Setup();
	void Update();
	void Render();
	D3DXVECTOR3& GetPostion();
};

