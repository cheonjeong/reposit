#pragma once
#include "cCharacter.h"

class cCubeNode;
class cGroup;
class cCubeMan :
	public cCharacter
{
public:
	cCubeMan();
	virtual ~cCubeMan();

protected:
	cCubeNode* m_pRoot;
	D3DMATERIAL9 m_stMtl;

	LPDIRECT3DTEXTURE9 m_pTexture;
	//vector<ST_PT_VERTEX> m_vecVertex;

public:
	void Setup() override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;
};

