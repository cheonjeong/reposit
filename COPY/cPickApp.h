#pragma once
class cRay;
class cSphere;


class cPickApp
{
public:
	cPickApp();
	~cPickApp();
public:
	bool raySphereIntersectionTest(cRay * pRay, cSphere * pSphere);
};

