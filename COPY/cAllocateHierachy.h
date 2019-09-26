#pragma once
#include "d3dx9anim.h"

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16	CombinedTransformationMatrix;
};
struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	vector<D3DMATERIAL9>		vecMtl;
	vector<LPDIRECT3DTEXTURE9>	vecTexture;
	LPD3DXMESH					pOrigMesh;
	D3DXMATRIX**				ppBonMatrixPtrs;
	D3DXMATRIX*					pBoneOffsetMatrices;
	D3DXMATRIX*					pCurrentBoneMatrices;
};

class cAllocateHierachy :
	public ID3DXAllocateHierarchy
{
public:
	cAllocateHierachy();
	~cAllocateHierachy();
protected:
	SYNTHESIZE(string, m_sFolder, Folder);
public:
	STDMETHOD(CreateFrame) (
		THIS_ LPCSTR Name,
		LPD3DXFRAME* ppNewFrame
		) override;

	STDMETHOD(CreateMeshContainer) (
		THIS_ LPCSTR nAME,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL*	pMaterials,
		CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER*	ppNewMeshContainer
		) override;

	STDMETHOD(DestroyFrame) (THIS_ LPD3DXFRAME pRameToTree) override;

	STDMETHOD(DestroyMeshContainer) (THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

