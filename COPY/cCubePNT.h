#pragma once
class cCubePNT
{
public:
	cCubePNT();
	virtual ~cCubePNT();

protected:
	std::vector<ST_PNT_VERTEX> m_vecVertex;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();

	vector<ST_PNT_VERTEX>	GetVertex() { return m_vecVertex; }
};

