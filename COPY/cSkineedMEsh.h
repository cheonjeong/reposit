#pragma once
class cSkineedMEsh
{
public:
	cSkineedMEsh();
	~cSkineedMEsh();

protected :
	LPD3DXFRAME							m_pRoot;
	LPD3DXANIMATIONCONTROLLER			m_pAnimController;

public:
	void Setup(char * szFolder, char * szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
	void SetAnimationIndex(int index);

protected:
	float m_fBlendTime;
	float m_fPassedBlendTime;
	bool m_isAnimBlend;

public:
	void SetAnimationIndexBlend(int index);

	void UpdateAnimation();
};

