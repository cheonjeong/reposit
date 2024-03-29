#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cFrame :
	public cObject
{
public:
	cFrame();
	~cFrame();

private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	vector<cFrame*> m_vecChild;

public:
	void Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcOriginalLocalTM(D3DXMATRIXA16* pmatParent);

	// : ani
	void CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT);
	void CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16 & matR);

	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwFrameSpeed;
	DWORD m_dwTickPerFrame;

	int GetKeyFrame();

public:
	void Set();
private:
	int m_nNum;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;

private:
	LPDIRECT3DINDEXBUFFER9			m_pVI;
};