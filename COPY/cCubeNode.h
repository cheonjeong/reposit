#pragma once
#include "cCubePNT.h"

class cCubeNode : public cCubePNT
{
public:
	cCubeNode();
	virtual ~cCubeNode();

protected:
	float m_fRotX;
	D3DXVECTOR3 m_vLocalPos;
	D3DXMATRIXA16 m_matLocalTM;
	D3DXMATRIXA16 m_matWorldTM;
	vector<cCubeNode*> m_vecChild;
	SYNTHESIZE(D3DXMATRIXA16*, m_pParentWorldTM, ParentWorldTM);
	SYNTHESIZE(float, m_fRotDeltaX, RotDeltaX);

public:
	virtual void AddChild(cCubeNode* pChild);
	virtual void Destroy();
	void cCubeNode::SetVertex(float sx, float sy, float xg, float yg, float uxg, float uyg, float sxg);
	


	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};