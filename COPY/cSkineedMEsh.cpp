#include "stdafx.h"
#include "cSkineedMEsh.h"
#include"cAllocateHierachy.h" // <<
#include"cTimerManager.h"


cSkineedMEsh::cSkineedMEsh()
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
{
}


cSkineedMEsh::~cSkineedMEsh()
{
	cAllocateHierachy ah;
	D3DXFrameDestroy(m_pRoot, &ah);
	SAFE_RELEASE(m_pAnimController);
}

void cSkineedMEsh::Setup(char * szFolder, char * szFile)
{
	string sFullPath(szFolder);
	sFullPath = sFullPath + string("/") + string(szFile);

	cAllocateHierachy ah;
	ah.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkineedMEsh::Update()
{
	if (m_isAnimBlend)
	{
		m_fPassedBlendTime += g_pTimeManager->GetEllapsedTime();
		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			m_isAnimBlend = false;
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}

	m_pAnimController->AdvanceTime(g_pTimeManager->GetEllapsedTime(), NULL);
	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkineedMEsh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pParent)
	{
		pBone->CombinedTransformationMatrix *=
			((ST_BONE*)pParent)->CombinedTransformationMatrix;
	}
	if (pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}
	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
}

void cSkineedMEsh::Render(LPD3DXFRAME pFrame)
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMEsh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMEsh->MeshData.pMesh)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
			for (size_t i = 0; i < pBoneMEsh->vecMtl.size(); ++i)
			{
				g_pD3DDevice->SetTexture(0, pBoneMEsh->vecTexture[i]);
				g_pD3DDevice->SetMaterial(&pBoneMEsh->vecMtl[i]);
				pBoneMEsh->MeshData.pMesh->DrawSubset(i);
			}
		}
	}
	if (pFrame->pFrameFirstChild)
	{
		Render(pFrame->pFrameFirstChild);
	}
	if (pFrame->pFrameSibling)
	{
		Render(pFrame->pFrameSibling);
	}
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetTexture(0, NULL);
}

void cSkineedMEsh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH * pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = 
					(ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBonMatrixPtrs[i] =
					&(pBone->CombinedTransformationMatrix);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
	}
	if (pFrame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
	}
}

void cSkineedMEsh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = 
			(ST_BONE_MESH*)pFrame->pMeshContainer;
		
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBonMatrixPtrs[i]);
			}
		}

		BYTE* src = NULL;
		BYTE* dest = NULL;
		pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY,
			(void**)&src);
			pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	}

	if (pFrame->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);
	}
	if (pFrame->pFrameSibling)
	{
		UpdateSkinnedMesh(pFrame->pFrameSibling);
	}
}

void cSkineedMEsh::SetAnimationIndex(int index)
{
	int num = m_pAnimController->GetNumAnimationSets();
	index %= num;

	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(index, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	m_pAnimController->GetPriorityBlend();
	SAFE_RELEASE(pAnimSet);

}

void cSkineedMEsh::SetAnimationIndexBlend(int index)
{
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;

	int num = m_pAnimController->GetNumAnimationSets();
	index % num;


	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);

	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->GetAnimationSet(index, &pNextAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	
	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}

void cSkineedMEsh::UpdateAnimation()
{
	LPD3DXANIMATIONSET pN1_Set = NULL;
	LPD3DXANIMATIONSET pN2_Set = NULL;
	LPD3DXANIMATIONSET pN3_Set = NULL;
	LPD3DXANIMATIONSET pN4_Set = NULL;
	LPD3DXANIMATIONSET pN5_Set = NULL;
	D3DXTRACK_DESC stTrackDesc;
	if (GetAsyncKeyState('1'))
	{
		
	}
	if (GetAsyncKeyState('2'))
	{
		
	}
	if (GetAsyncKeyState('3'))
	{

	}
	if (GetAsyncKeyState('4'))
	{

	}
	if (GetAsyncKeyState('5'))
	{

	}
}


// ex> 공격 모션이 끝난 후 서있는 모션으로 전환하기, m_pAnimController->