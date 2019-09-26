#include "stdafx.h"
#include "cPickApp.h"
#include "cRay.h"
#include "cSphere.h"


cPickApp::cPickApp()
{
}


cPickApp::~cPickApp()
{
}

bool cPickApp::raySphereIntersectionTest(cRay * pRay, cSphere * pSphere)
{
	D3DXVECTOR3 v = pRay->_Origin - pSphere->Getpos();

	float b = 2.0f * D3DXVec3Dot(&pRay->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (pSphere->_Radius * pSphere->_Radius);

	float discriminant = (b * b) - (4.0f * c);

	if (discriminant < 0.0f)
		return false;

	discriminant = sqrt(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;


	return false;
}
