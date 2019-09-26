#pragma once

//class
class cRay;





// >> func
void TransFormRay(cRay * ray, D3DXMATRIXA16 * t);

D3DXVECTOR3 maked_world_position(vector<ST_PNT_VERTEX> vecVertex, D3DXVECTOR2 uv);