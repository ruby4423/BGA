#pragma once
#include <d3dx9.h>
#define D3DFVF_MYVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class SphereCreator
{
public:
	struct MY_VERTEX //점 데이터형
	{
		float x, y, z, rhw;
		DWORD color;
	};
	LPDIRECT3DDEVICE9 g_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;


	SphereCreator(LPDIRECT3DDEVICE9 pD3DDevice);
	HRESULT initVertexBuffer();
	bool Render();
};