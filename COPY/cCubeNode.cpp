#include "stdafx.h"
#include "cCubeNode.h"


cCubeNode::cCubeNode() : m_pParentWorldTM(NULL), m_vLocalPos(0,0,0), m_fRotDeltaX(0.0f), m_fRotX(0.0f)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cCubeNode::~cCubeNode()
{
}

void cCubeNode::AddChild(cCubeNode * pChild)
{
	pChild->m_pParentWorldTM = &m_matWorldTM;
	m_vecChild.push_back(pChild);
}

void cCubeNode::Destroy()
{
	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}
	delete this;
}

void cCubeNode::Setup()
{
	cCubePNT::Setup();
}

void cCubeNode::Update()
{
	cCubePNT::Update();

	// >> :
	m_fRotX += m_fRotDeltaX;
	if (m_fRotX > D3DX_PI / 6.0f)
	{
		m_fRotX = D3DX_PI / 6.0f;
		m_fRotDeltaX *= -1;
	}
	if (m_fRotX < -D3DX_PI / 6.0f)
	{
		m_fRotX = -D3DX_PI / 6.0f;
		m_fRotDeltaX *= -1;
	}
	// << :
	
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	D3DXMatrixRotationX(&matR, m_fRotX);
	D3DXMatrixTranslation(&matT, m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	m_matLocalTM = matR * matT;
	m_matWorldTM = m_matLocalTM;
	if (m_pParentWorldTM)
	{
		m_matWorldTM *= *m_pParentWorldTM;
	}

	for each(auto p in m_vecChild)
	{
		p->Update();
	}
}

void cCubeNode::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	cCubePNT::Render();

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cCubeNode::SetVertex(float sx, float sy, float xg, float yg, float uxg, float uyg, float sxg)
{
	float f = 1.0f/64.0f;

	{
		// : front
		m_vecVertex[0].t = D3DXVECTOR2((sx + xg + sxg) * f, (sy + yg) * f);
		m_vecVertex[1].t = D3DXVECTOR2((sx + xg + sxg) * f, sy * f);
		m_vecVertex[2].t = D3DXVECTOR2((sx + xg + sxg + xg) * f, sy * f);


		m_vecVertex[3].t = D3DXVECTOR2((sx + xg + sxg) * f, (sy + yg) * f);
		m_vecVertex[4].t = D3DXVECTOR2((sx + xg + sxg + xg) * f, sy * f);
		m_vecVertex[5].t = D3DXVECTOR2((sx + xg + sxg + xg) * f, (sy + yg)  * f);
	
		// : back
		m_vecVertex[6].t = D3DXVECTOR2((sx+xg) * f, (sy + yg) * f);
		m_vecVertex[7].t = D3DXVECTOR2(sx * f, sy * f);
		m_vecVertex[8].t = D3DXVECTOR2((sx + xg) * f, sy * f);

		m_vecVertex[9].t = D3DXVECTOR2((sx + xg) * f, (sy + yg) * f);
		m_vecVertex[10].t = D3DXVECTOR2(sx * f, (sy + yg) * f);
		m_vecVertex[11].t = D3DXVECTOR2(sx * f, sy * f);

		// : left
		m_vecVertex[12].t = D3DXVECTOR2((sx + xg) * f, (sy + yg) * f);
		m_vecVertex[13].t = D3DXVECTOR2((sx + xg) * f, sy * f);
		m_vecVertex[14].t = D3DXVECTOR2((sx + xg + sxg) * f, sy * f);

		m_vecVertex[15].t = D3DXVECTOR2((sx + xg) * f, (sy + yg) * f);
		m_vecVertex[16].t = D3DXVECTOR2((sx + xg + sxg)* f, sy * f);
		m_vecVertex[17].t = D3DXVECTOR2((sx + xg+ sxg) * f, (sy + yg)  * f);

		// : right
		m_vecVertex[18].t = D3DXVECTOR2((sx - sxg) * f, (sy + yg) * f);
		m_vecVertex[19].t = D3DXVECTOR2((sx - sxg) * f, sy * f);
		m_vecVertex[20].t = D3DXVECTOR2(sx * f, sy * f);

		m_vecVertex[21].t = D3DXVECTOR2((sx - sxg) * f, (sy + yg) * f);
		m_vecVertex[22].t = D3DXVECTOR2(sx * f, sy * f);
		m_vecVertex[23].t = D3DXVECTOR2(sx * f, (sy + yg)  * f);

		// : top
		m_vecVertex[24].t = D3DXVECTOR2((sx+uxg)* f, (sy-uyg) * f);
		m_vecVertex[25].t = D3DXVECTOR2((sx+ uxg)* f, sy * f);
		m_vecVertex[26].t = D3DXVECTOR2(sx * f, sy * f);

		m_vecVertex[27].t = D3DXVECTOR2((sx+uxg)* f, (sy-uyg) * f);
		m_vecVertex[28].t = D3DXVECTOR2(sx * f, sy * f);
		m_vecVertex[29].t = D3DXVECTOR2(sx * f, (sy-uyg) * f);

		// : bottom
		m_vecVertex[30].t = D3DXVECTOR2((sx + uxg+ uxg) * f, sy * f);
		m_vecVertex[31].t = D3DXVECTOR2((sx + uxg + uxg) * f, (sy-uyg) * f);
		m_vecVertex[32].t = D3DXVECTOR2((sx + uxg) * f, (sy-uyg) * f);

		m_vecVertex[33].t = D3DXVECTOR2((sx + uxg +uxg) * f, sy * f);
		m_vecVertex[34].t = D3DXVECTOR2((sx + uxg) * f, (sy-uyg) * f);
		m_vecVertex[35].t = D3DXVECTOR2((sx + uxg) * f, sy  * f);
	}
}
