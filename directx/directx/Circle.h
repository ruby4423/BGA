#pragma once

#include <d3dx9.h>

#define CIRCLE_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ)

class CCircle
{
public:
	struct CIRCLE_CUSTOMVERTEX
	{
		FLOAT x, y, z;		//Position of vertex in 3D space
	};
	LPDIRECT3DDEVICE9 g_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	D3DMATERIAL9 matMaterial;
	DWORD m_dwNumOfVertices;
	float radian;
	////////////////////////////////////
	HRESULT CreateVertexBuffer();
	CCircle(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CCircle();
	DWORD Render();
	bool setSize(float radian);
	bool CreateIndexBuffer();
	bool SetMaterial(D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower);
};