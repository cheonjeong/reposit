#include "stdafx.h"
#include "cAllocateHierachy.h"


cAllocateHierachy::cAllocateHierachy()
{
}


cAllocateHierachy::~cAllocateHierachy()
{
}

STDMETHODIMP cAllocateHierachy::CreateFrame
		(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	ST_BONE*	pBone = new ST_BONE;
	ZeroMemory(pBone, sizeof(ST_BONE));

	if (Name)
	{
		pBone->Name = new char[lstrlenA(Name) + 1];
		UINT length = lstrlenA(Name) + 1;
		strcpy_s(pBone->Name, length * sizeof(CHAR), Name);
	} // << Name

	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);

	*ppNewFrame = pBone;


	return S_OK;
}

STDMETHODIMP cAllocateHierachy::CreateMeshContainer
		(LPCSTR nAME, 
			CONST D3DXMESHDATA * pMeshData, 
			CONST D3DXMATERIAL * pMaterials, 
			CONST D3DXEFFECTINSTANCE * pEffectInstances, 
			DWORD NumMaterials, 
			CONST DWORD * pAdjacency, 
			LPD3DXSKININFO pSkinInfo, 
			LPD3DXMESHCONTAINER * ppNewMeshContainer
		)
{
	ST_BONE_MESH*	pBoneMesh = new ST_BONE_MESH;
	ZeroMemory(pBoneMesh, sizeof(ST_BONE_MESH));

	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		pBoneMesh->vecMtl.push_back(pMaterials[i].MatD3D);
		string sFullPath = m_sFolder;
		sFullPath = sFullPath + string("/") + string(pMaterials[i].pTextureFilename);

		pBoneMesh->vecTexture.push_back(g_pTextureManager->GetTexture(sFullPath));
	} // << for DWORD
	pSkinInfo->AddRef();
	pBoneMesh->pSkinInfo = pSkinInfo;

	pMeshData->pMesh->AddRef();
	pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
	pMeshData->pMesh->CloneMeshFVF(
		pMeshData->pMesh->GetOptions(),
		pMeshData->pMesh->GetFVF(),
		g_pD3DDevice,
		&pBoneMesh->pOrigMesh
	);

	// : 
	DWORD dwNumBones = pSkinInfo->GetNumBones();
	pBoneMesh->ppBonMatrixPtrs = new D3DXMATRIX*[dwNumBones];
	pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
	pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];



	for (DWORD i = 0; i < dwNumBones; ++i)
	{
		pBoneMesh->pBoneOffsetMatrices[i] =
			*(pSkinInfo->GetBoneOffsetMatrix(i));
	}
	*ppNewMeshContainer = pBoneMesh;



	return S_OK;
}

STDMETHODIMP cAllocateHierachy::DestroyFrame
		(LPD3DXFRAME pFrameToTree)
{
	ST_BONE* pBone = (ST_BONE*)pFrameToTree;
	SAFE_DELETE_ARRAY(pBone->Name);
	SAFE_DELETE(pFrameToTree);
	return S_OK;
}

STDMETHODIMP cAllocateHierachy::DestroyMeshContainer
		(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;
	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);

	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBonMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);

	SAFE_DELETE(pBoneMesh);

	return S_OK;
}
