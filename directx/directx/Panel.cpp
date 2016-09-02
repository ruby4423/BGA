// Panel.cpp: implementation of the CPanel class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Panel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanel::CPanel(LPDIRECT3DDEVICE9 pD3DDevice, int nWidth, int nHeight, int nScreenWidth, int nScreenHeight, DWORD dwColour)
{
	m_pD3DDevice = pD3DDevice;
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_dwColour = dwColour;

	//Initialize Vertex Buffer
    if(CreateVertexBuffer())
	{
		if(UpdateVertices())
		{
			LogInfo("<li>Panel created OK");
			return;
		}
	}

	LogError("<li>Panel failed to create");
}

CPanel::~CPanel()
{
	SafeRelease(m_pTexture);
	SafeRelease(m_pVertexBuffer);

	LogInfo("<li>Panel destroyed OK");
}

bool CPanel::CreateVertexBuffer()
{
	//Create the vertex buffer from our device.
    if(FAILED(m_pD3DDevice->CreateVertexBuffer(4 * sizeof(PANEL_CUSTOMVERTEX),
                                               0, PANEL_D3DFVF_CUSTOMVERTEX,
                                               D3DPOOL_DEFAULT, &m_pVertexBuffer, 0)))
    {
        LogError("<li>CPanel: Unable to create vertex buffer.");
		return false;
    }

    return true;
}

bool CPanel::UpdateVertices()
{
	PANEL_CUSTOMVERTEX* pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (VOID**)&pVertices, 0);

	if(m_dwColour == -1)
	{
		//No colour was set, so default to white
		m_dwColour = D3DCOLOR_XRGB(255, 255, 255);
	}

	//Set all the vertices to selected colour
	pVertices[0].colour = m_dwColour;
	pVertices[1].colour = m_dwColour;
	pVertices[2].colour = m_dwColour;
	pVertices[3].colour = m_dwColour;

	
	//Set the positions of the vertices
	pVertices[0].x = -(m_nWidth) / 2.0f;
	pVertices[0].y = -(m_nHeight) / 2.0f;
	
	pVertices[1].x = -(m_nWidth) / 2.0f;
	pVertices[1].y = m_nHeight / 2.0f;

	pVertices[2].x = (m_nWidth) / 2.0f;
	pVertices[2].y = -(m_nHeight) / 2.0f;

	pVertices[3].x = (m_nWidth) / 2.0f;
	pVertices[3].y = m_nHeight / 2.0f;
	
	pVertices[0].z = 1.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].z = 1.0f; 
	pVertices[3].z = 1.0f;


	//Set the texture coordinates of the vertices
	pVertices[0].u = 0.0f;
	pVertices[0].v = 1.0f;

	pVertices[1].u = 0.0f;
	pVertices[1].v = 0.0f;

	pVertices[2].u = 1.0f;
	pVertices[2].v = 1.0f;

	pVertices[3].u = 1.0f;
	pVertices[3].v = 0.0f;

	m_pVertexBuffer->Unlock();

	return true;
}

bool CPanel::SetTexture(const char *szTextureFilePath, DWORD dwKeyColour)
{
	
	if(FAILED(D3DXCreateTextureFromFileExA(m_pD3DDevice, szTextureFilePath, 0, 0, 0, 0,
										  D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
										  D3DX_DEFAULT, dwKeyColour, NULL, NULL, &m_pTexture)))
	{
		return false;
	}

	return true;
}

DWORD CPanel::Render()
{
	m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PANEL_CUSTOMVERTEX));
	m_pD3DDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);

	if(m_pTexture != NULL)
	{
		m_pD3DDevice->SetTexture(0, m_pTexture);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}
	else
	{
		m_pD3DDevice->SetTexture(0, NULL);
	}

	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return 2;	//Return the number of polygons rendered
}

void CPanel::MoveTo(int x, int y)
{
	//x and y specify the top left corner of the panel in screen coordinates
	D3DXMATRIX matMove;

	x -= (m_nScreenWidth / 2) - (m_nWidth / 2);
	y -= (m_nScreenHeight / 2) - (m_nHeight / 2);

	D3DXMatrixTranslation(&matMove, (float)x, -(float)y, 0.0f);
	
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matMove);
}
