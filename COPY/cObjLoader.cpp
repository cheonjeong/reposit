#include "stdafx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"


cObjLoader::cObjLoader()
	: m_pIB(NULL)
	, m_pVB(NULL)
{
}


cObjLoader::~cObjLoader()
{
}

void cObjLoader::Load(OUT std::vector<cGroup*>& vecGroup, IN char * szFolder, IN char * szFile)
{
	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR2> vecVT;
	vector<D3DXVECTOR3> vecVN;
	vector<ST_PNT_VERTEX> vecVertex;

	string sFullPath(szFolder);
	sFullPath += (string("/") + string(szFile));

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;

	while (true)
	{
		if (feof(fp))
			break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			char szMtlFile[1024];
			sscanf_s(szTemp, "%*s %s", szMtlFile, 1024);
			LoadMtlLib(szFolder, szMtlFile);
		}
		else if (szTemp[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertex(vecVertex);
				pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
			}
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{
				if (szTemp[2] == ' ')
				{
					float u, v;
					sscanf_s(szTemp, "%*s %f %f", &u, &v);
					vecVT.push_back(D3DXVECTOR2(u, v));
				}
			}
			else if (szTemp[1] == 'n')
			{
				if (szTemp[2] == ' ')
				{
					float x, y, z;
					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
					vecVN.push_back(D3DXVECTOR3(x, y, z));
				}
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]
				);

			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[nIndex[i][0] - 1];
				v.t = vecVT[nIndex[i][1] - 1];
				v.n = vecVN[nIndex[i][2] - 1];

				vecVertex.push_back(v);
			} // << : for
		} // << : if ~ else
	} // << : while

	fclose(fp);

	for each(auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapMtlTex.clear();
}

void cObjLoader::LoadMtlLib(char * szFolder, char * szFile)
{
	string sFullPath(szFolder);
	sFullPath += (string("/") + string(szFile));

	FILE * fp;
	fopen_s(&fp, sFullPath.c_str(), "r");
	string sMtlName;

	

	while (true)
	{
		if (feof(fp))
			break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		if (szTemp[0] == '#')
			continue;
		else if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
			if (m_mapMtlTex.find(sMtlName) == m_mapMtlTex.end())
			{
				m_mapMtlTex[sMtlName] = new cMtlTex;
			}
		}
		else if (szTemp[0] == 'K')
		{
			if (szTemp[1] == 'a')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.a = 1.0f;
			}
			else if(szTemp[1] == 'd')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.a = 1.0f;
			}
			else if (szTemp[1] == 's')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.a = 1.0f;
			}
		} // << : 'K'
		else if (szTemp[0] == 'd')
		{
			float d;
			sscanf_s(szTemp, "%*s %f", &d);
			m_mapMtlTex[sMtlName]->GetMaterial().Power = d;
		}
		else if (szTemp[0] == 'm')
		{
			char szTexFile[1024];
			sscanf_s(szTemp, "%*s %s", szTexFile, 1024);
			sFullPath = string(szFolder);
			sFullPath += (string("/") + string(szTexFile));
			LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(sFullPath);
			m_mapMtlTex[sMtlName]->SetTexture(pTexture);
		}
	} // << : while

	fclose(fp);
}

void cObjLoader::LoadSurface(OUT std::vector<D3DXVECTOR3>& vecSurface, IN char * szFolder, IN char * szFile, IN D3DXMATRIXA16 * pmat)
{
	vector<D3DXVECTOR3> vecV;

	string sFullPath(szFolder);
	sFullPath += string("/") + string(szFile);

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;

	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			continue;
		}
		else if (szTemp[0] == 'g')
		{
			continue;
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't') continue;
			else if (szTemp[1] == 't') continue;
		}
		else if (szTemp[0] == 'u')
		{
			continue;
		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3];
			sscanf_s(szTemp, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
				&nIndex[0], &nIndex[1], &nIndex[2]);

			for (int i = 0; i < 3; ++i)
			{
				vecSurface.push_back(vecV[nIndex[i] - 1]);
			}
		}
	} // << : while

	fclose(fp);

	if (pmat)
	{
		for (size_t i = 0; i < vecSurface.size(); ++i)
		{
			D3DXVec3TransformCoord(
				&vecSurface[i],
				&vecSurface[i],
				pmat);
		}
	}

}

LPD3DXMESH cObjLoader::LoadMesh(OUT vector<cMtlTex*>& vecMtlTex, IN char * szFolder, IN char * szFile)
{
	vector<D3DXVECTOR3>			vecV;
	vector<D3DXVECTOR2>			vecVT;
	vector<D3DXVECTOR3>			vecVN;
	vector<ST_PNT_VERTEX>		vecVertex;
	vector<DWORD>				vecAttributeBuffer;

	string sFullPath(szFolder);
	sFullPath += (string("/") + string(szFile));

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;

	while (true)
	{
		if (feof(fp))
			break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		if (szTemp[0] == '#')		continue;

		else if (szTemp[0] == 'm')
		{
			char szMtlFile[1024];
			sscanf_s(szTemp, "%*s %s", szMtlFile, 1024);
			LoadMtlLib(szFolder, szMtlFile); // << : 머테리얼 읽어들여서 속성 설정(ID 부여 해주는 코드 추가 하기)
		}
		else if (szTemp[0] == 'g')
		{

		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{
				if (szTemp[2] == ' ')
				{
					float u, v;
					sscanf_s(szTemp, "%*s %f %f", &u, &v);
					vecVT.push_back(D3DXVECTOR2(u, v));
				}
			}
			else if (szTemp[1] == 'n')
			{
				if (szTemp[2] == ' ')
				{
					float x, y, z;
					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
					vecVN.push_back(D3DXVECTOR3(x, y, z));
				}
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]
			);
			
			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[nIndex[i][0] - 1];
				v.t = vecVT[nIndex[i][1] - 1];
				v.n = vecVN[nIndex[i][2] - 1];

				vecVertex.push_back(v);
			} // << : for

			if (sMtlName == "09_-_Default")	 			vecAttributeBuffer.push_back(0);
			else if (sMtlName == "14_-_Default")		vecAttributeBuffer.push_back(1);
			else if (sMtlName == "02_-_Default")		vecAttributeBuffer.push_back(2);
			else if (sMtlName == "Material__2")			vecAttributeBuffer.push_back(3);
			else if (sMtlName == "08_-_Default")		vecAttributeBuffer.push_back(4);
			else if (sMtlName == "13_-_Default")		vecAttributeBuffer.push_back(5);

		} // << : if ~ else
	} // << : while


	// >> 속성 정보 추가

	vecMtlTex.push_back(m_mapMtlTex["09_-_Default"]);
	vecMtlTex.push_back(m_mapMtlTex["14_-_Default"]);
	vecMtlTex.push_back(m_mapMtlTex["02_-_Default"]);
	vecMtlTex.push_back(m_mapMtlTex["Material__2"]);
	vecMtlTex.push_back(m_mapMtlTex["08_-_Default"]);
	vecMtlTex.push_back(m_mapMtlTex["13_-_Default"]);


	// <<

	fclose(fp);
	/*
		1.Create Mesh
		2.Vertex Buffer
		3.Index Buffer
		4.attribute Buffer
		5.Generate
		6.Optimize
	*/
	//CreateMesh
	LPD3DXMESH		Mesh;
	D3DXCreateMeshFVF(vecVertex.size() / 3, 
		vecVertex.size(), 
		D3DPOOL_MANAGED | 
		D3DXMESH_32BIT, 
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice, 
		&Mesh);
	

	
	//vertexBuffer
	ST_PNT_VERTEX* vb;
	{
		Mesh->LockVertexBuffer(0, (LPVOID*)&vb);

		for (size_t i = 0; i < vecVertex.size(); ++i)
			vb[i] = vecVertex[i];

		Mesh->UnlockVertexBuffer();
	}


	//IndexBuffer
	DWORD* index;
	{
		Mesh->LockIndexBuffer(0, (LPVOID*)&index);

		for (size_t i = 0; i < vecVertex.size(); ++i)
			index[i] = i;

		Mesh->UnlockIndexBuffer();
	}


	//Attribute Buffer
	DWORD * attribute;
	{
		Mesh->LockAttributeBuffer(0, &attribute);

		for (size_t i = 0; i < vecAttributeBuffer.size(); ++i)
			attribute[i] = vecAttributeBuffer[i];

		Mesh->UnlockAttributeBuffer();
	}


	//Generate
	vector<DWORD> adjacencyBuffer(Mesh->GetNumFaces() * 3);
	Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);


	//Optimize
	Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&adjacencyBuffer[0],
		0, 0, 0
	);

	return Mesh;
}
