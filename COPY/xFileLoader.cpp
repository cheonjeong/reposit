#include "stdafx.h"
#include "xFileLoader.h"


xFileLoader::xFileLoader()
{
}


xFileLoader::~xFileLoader()
{
}

bool xFileLoader::Setup(char * fileName)
{
	HRESULT hr = 0;

	ID3DXBuffer * adjBuffer = 0;
	ID3DXBuffer * mtlBuffer = 0;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromXA(
		fileName,
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&adjBuffer,
		&mtlBuffer,
		0,
		&numMtrls,
		&m_pMesh
	);

	if (FAILED(hr))
	{
		::MessageBox(0, _T("D3DXLoadMeshFromX() - FAILED"), 0, 0);
		return false;
	}

	if (mtlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL*	mtrls = (D3DXMATERIAL *)mtlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; ++i)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			m_vecMtl.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9*	tex = 0;
				D3DXCreateTextureFromFile(
					g_pD3DDevice,
					(LPCWSTR)mtrls[i].pTextureFilename,
					&tex
				);
				m_vecTex.push_back(tex);
			}
			else
			{
				m_vecTex.push_back(0);
			}
		}
	}
	return true;
}

bool xFileLoader::Display(float timeDelta)
{
	if (g_pD3DDevice)
	{
		static float y = 0.0f;
		D3DXMATRIXA16	yRot;
		D3DXMatrixRotationY(&yRot, y);

		if (y >= 6.28f)
			y = 0.0f;

		D3DXMATRIXA16 matWorld = yRot;
		
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		g_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xffffffff, 1.0f, 0);

		g_pD3DDevice->BeginScene();

		for (int i = 0; i < m_vecTex.size(); i++)
		{
			g_pD3DDevice->SetMaterial(&m_vecMtl[i]);
			g_pD3DDevice->SetTexture(0, m_vecTex[i]);
		}
		g_pD3DDevice->EndScene();
		g_pD3DDevice->Present(0, 0, 0, 0);
	}
	return true;
}
