// Panel.h: interface for the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_)
#define AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"

#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CPanel : public CBase  
{
private:
	//Define a custom vertex for our panel
	struct PANEL_CUSTOMVERTEX
	{
		FLOAT x, y, z;		//Position of vertex
		DWORD colour;		//Colour of vertex
		FLOAT u, v;			//Texture coordinates
	};

public:
	void MoveTo(int x, int y);
	DWORD Render();
	CPanel(LPDIRECT3DDEVICE9 pD3DDevice, int nWidth, int nHeight, int nScreenWidth, int nScreenHeight, DWORD dwColour = -1);
	virtual ~CPanel();
	bool SetTexture(const char *szTextureFilePath, DWORD dwKeyColour = 0);

private:
	bool UpdateVertices();
	bool CreateVertexBuffer();
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nWidth;
	int m_nHeight;
	int m_nScreenWidth;
	int m_nScreenHeight;
	DWORD m_dwColour;

};

#endif // !defined(AFX_PANEL_H__22B0FAAD_937B_41CF_848E_C899BC8CEF50__INCLUDED_)
