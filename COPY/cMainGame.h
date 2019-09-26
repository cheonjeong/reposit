#pragma once
class cCubePC;
class cGrid;
class cCamera;
class cCubeMan;
class cGroup;
class cFrame;
class cMtlTex;
class cSphere;
class cPickApp;
class cRay;
class cBoard;
class cRawLoader;
class cHeightMap;

class cSkineedMEsh;


class cMainGame
{
public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();

private:
	//cCubePC* m_pCubePC;
	cGrid* m_pGrid;
	cCamera* m_pCamera;
	cCubeMan* m_pCubeMan;

	vector<cGroup*> m_vecGroup;
	cFrame* m_pRootFrame;
	vector<cMtlTex*> m_vecMtlTex;

public:
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Set_Light();

	void Setup_Obj();
	void Render_Obj();
	void Load_Surface();

private:
	iMap* m_pMap;
	// >> :
	LPDIRECT3DTEXTURE9 m_pTexture;
	vector<ST_PT_VERTEX> m_vecVertex;
	// << :

	LPD3DXMESH		m_mesh;

public:
	void Set_Picking();

private:
	cPickApp*				m_Picking;
	vector<cSphere*>		m_vecpSphere;
	vector<cSphere*>		m_onlyDraw;
	cSphere*				m_pSphere;
	cRay*					m_pRay;

private:
	D3DXVECTOR2					m_getPos;
	bool						m_flag;

	LPARAM						m_lParam;
	cBoard*						m_pBoard;

	bool						m_pickboard;

public:
	void SetupHeightMap();
private:
	cSkineedMEsh*				m_pSkinnedMesh;
	bool						isCulling;
};