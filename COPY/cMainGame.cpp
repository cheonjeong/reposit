#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cCubePC.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCubeMan.h"

#include "cObjLoader.h"
#include "cGroup.h"
#include "cobjMap.h"

#include "cAseLoader.h"
#include "cFrame.h"
#include"cMtlTex.h"

#include"cSphere.h"
#include"cPickApp.h"
#include "cRay.h"
#include "func.h"
#include "cBoard.h"
#include"cHeightMap.h"

#include"cSkineedMEsh.h"
#include"cTimerManager.h"

#define RADIUS 0.5f

cMainGame::cMainGame()
	: //m_pCubePC(NULL)
	m_pCubeMan(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pTexture(NULL)
	, m_pMap(NULL)
	, m_pRootFrame(NULL)
	, m_mesh(NULL)
	, m_pSphere(NULL)
	, m_pRay(NULL)
	, m_Picking(NULL)
	, m_pBoard(NULL)
	, m_pickboard(false)
	, m_pSkinnedMesh(NULL)
	, isCulling(false)
	



{
}


cMainGame::~cMainGame()
{
	//SAFE_DELETE(m_pCubePC);
	SAFE_DELETE(m_pSkinnedMesh);

	SAFE_DELETE(m_pCubeMan);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);

	SAFE_DELETE(m_pBoard);

	SAFE_RELEASE(m_pTexture);
	m_pRootFrame->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	SetupHeightMap();

	m_pCubeMan = new cCubeMan();
	m_pCubeMan->Setup();

	//Setup_Obj();
	//Load_Surface();

	/*cAseLoader l;
	m_pRootFrame = l.Load("woman/woman_01_all.ase");*/

	m_pRay = new cRay();
	
	m_Picking = new cPickApp();
	
	{
		for(int y = -20; y <= 20; y += RADIUS * 2)
			for (int z = -20; z <= 20; z += RADIUS * 2)
				for (int x = -20; x <= 20; x += RADIUS * 2)
				{
					m_pSphere = new cSphere(D3DXVECTOR3(x, y, z));
					m_vecpSphere.push_back(m_pSphere);
					m_onlyDraw.push_back(m_pSphere);
				}
	}

		for (int i = 0; i < m_vecpSphere.size(); i++)
		{
			m_vecpSphere[i]->SetUp(RADIUS);
		}
	m_pCamera = new cCamera();
	m_pCamera->Setup(NULL); //&m_pCubeMan->GetPosition()

	g_pZFrustum->BuildViewFrustum();

	m_pGrid = new cGrid();
	m_pGrid->Setup();
	
	m_pBoard = new cBoard();
	m_pBoard->Setup(D3DXVECTOR3(1.0f, 0.1f, 1.0f));

	m_pSkinnedMesh = new cSkineedMEsh;
	m_pSkinnedMesh->Setup("Zealot", "Zealot.x");

	Set_Light();
	// >> :
	D3DXCreateTextureFromFile(g_pD3DDevice, _T("paladogcharacter2.png"), &m_pTexture);
	/*ST_PT_VERTEX v;
	v.p = D3DXVECTOR3(0, 0, 0);
	v.t = D3DXVECTOR2(0, 1);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 5, 0);
	v.t = D3DXVECTOR2(0, 0);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(5, 0, 0);
	v.t = D3DXVECTOR2(1, 1);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 5, 0);
	v.t = D3DXVECTOR2(0, 0);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(5, 5, 0);
	v.t = D3DXVECTOR2(1, 0);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(5, 0, 0);
	v.t = D3DXVECTOR2(1, 1);
	m_vecVertex.push_back(v);*/
	// << :
	
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	
	/*if(m_pCubePC)
		m_pCubePC->Update();*/
	if (m_pCubeMan)
		m_pCubeMan->Update(m_pMap);

	g_pTimeManager->Update();
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->Update();

	if (m_pCamera)
		m_pCamera->Update();

	if (isCulling)
	{
		g_pZFrustum->BuildViewFrustum();
		m_onlyDraw.clear();
		for (int i = 0; i < m_vecpSphere.size(); i++)
		{
			if (g_pZFrustum->SphereInFrustum(&m_vecpSphere[i]->Getpos(), m_vecpSphere[i]->GetRadius()))
				m_onlyDraw.push_back(m_vecpSphere[i]);
		}
		isCulling = false;
	}

	
	/*if (m_pRootFrame)
	{
		m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
	}*/

	if (m_pickboard)
		m_pBoard->progress(m_pRay);


}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(150, 150, 150), 1.0f, 0);
	g_pD3DDevice->BeginScene();

	// : draw something
	if (m_pGrid)
		m_pGrid->Render();

	/*if (m_pBoard)
		m_pBoard->Render();
*/
	/*if (m_pCubePC)
		m_pCubePC->Render();*/

	if (m_pCubeMan)
		m_pCubeMan->Render();

	//if (m_pRootFrame)
	//	m_pRootFrame->Render();

	if (m_pMap)
		m_pMap->Render();

	for (int i = 0; i < m_onlyDraw.size(); i++)
		m_onlyDraw[i]->Render();
		
	g_pZFrustum->Draw(g_pD3DDevice);
	
	//Render_Obj();

	// >> :
	//{
	//	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//	D3DXMATRIXA16 matWorld;
	//	D3DXMatrixIdentity(&matWorld);
	//	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//	g_pD3DDevice->SetTexture(0, m_pTexture);
	//	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	//	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PT_VERTEX));
	//	//g_pD3DDevice->SetTexture(0, NULL);
	//}

	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		if (m_pSkinnedMesh)
			m_pSkinnedMesh->Render(NULL);
	}

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_lParam = lParam;
	cSphere * sphere;
	D3DXVECTOR2 pos;
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_getPos.x = GET_X_LPARAM(lParam);
		m_getPos.y = GET_Y_LPARAM(lParam);

		*m_pRay = m_pRay->CalcPickingRay(m_getPos);
		D3DXMATRIXA16		matview, matInverse;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matview);
		D3DXMatrixInverse(&matInverse, 0, &matview);
		TransFormRay(m_pRay, &matInverse);
		
		Set_Picking();
		m_pickboard = true;
	}
		break;
	case WM_LBUTTONUP:
		//m_pickboard = false;
		break;
	case WM_MBUTTONDOWN:
	{
		static int n = 0;
		m_pSkinnedMesh->SetAnimationIndexBlend(n++);
		isCulling = true;
	}
		break;
	}
}

void cMainGame::Set_Light()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::Setup_Obj()
{
	cObjLoader l;
	//l.Load(m_vecGroup, "obj", "map.obj");
	m_mesh = l.LoadMesh(m_vecMtlTex, "obj", "map.obj");
}

void cMainGame::Render_Obj()
{
	D3DXMATRIXA16 matWorld, matS, matR;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS * matR;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	/*for each(auto p in m_vecGroup)
	{
		p->Render();
	}*/

	for (size_t i = 0; i < m_vecMtlTex.size(); ++i)
	{
		g_pD3DDevice->SetMaterial(&m_vecMtlTex[i]->GetMaterial());
		g_pD3DDevice->SetTexture(0, m_vecMtlTex[i]->GetTexture());

		m_mesh->DrawSubset(i);
	}
}

void cMainGame::Load_Surface()
{
	/*D3DXMATRIXA16 matWorld, matS, matR;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS * matR;*/
	//m_pMap = new cObjMap("obj", "map_surface.obj", &matWorld);
}

void cMainGame::Set_Picking()
{

	for (int i = 0; i < m_vecpSphere.size(); i++)
	{
		if (m_Picking->raySphereIntersectionTest(m_pRay, m_vecpSphere[i]) == true)
		{
			m_vecpSphere[i]->m_Sphere.Ambient = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_vecpSphere[i]->m_Sphere.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			m_vecpSphere[i]->m_Sphere.Specular = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			m_vecpSphere[i]->m_Sphere.Ambient  = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_vecpSphere[i]->m_Sphere.Diffuse  = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
			m_vecpSphere[i]->m_Sphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
		}
	}
}



void cMainGame::SetupHeightMap()
{
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;
}
