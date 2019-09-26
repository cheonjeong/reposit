#include "stdafx.h"
#include "func.h"
#include"cRay.h"

void TransFormRay(cRay * ray, D3DXMATRIXA16 * t)
{
	D3DXVec3TransformCoord(
		&ray->_Origin,
		&ray->_Origin,
		t
	);

	D3DXVec3TransformNormal(
		&ray->_direction,
		&ray->_direction,
		t
	);

	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

D3DXVECTOR3 maked_world_position(vector<ST_PNT_VERTEX> vecVertex, D3DXVECTOR2 uv)
{
	D3DXVECTOR3 result;
	for (int i = 0; i < vecVertex.size(); i += 3)
	{
		result =
			vecVertex[i].p +
			uv.x * (vecVertex[i + 1].p - vecVertex[i].p) +
			uv.y * (vecVertex[i + 2].p - vecVertex[i].p);
	}
	return result;
}
