#include "stdafx.h"
#include "Circle.h"
#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if(pObject != NULL) {delete pObject; pObject=NULL;}


CCircle::CCircle(LPDIRECT3DDEVICE9 pD3DDevice)
{
	g_pD3DDevice = pD3DDevice;

	m_dwNumOfVertices = 50;
	radian = 1.0f;

	CreateVertexBuffer();

	//Set material default values (R, G, B, A)
	D3DCOLORVALUE rgbaDiffuse = { 1.0, 1.0, 1.0, 0.0, };
	D3DCOLORVALUE rgbaAmbient = { 1.0, 1.0, 1.0, 0.0, };
	D3DCOLORVALUE rgbaSpecular = { 0.0, 0.0, 0.0, 0.0, };
	D3DCOLORVALUE rgbaEmissive = { 1.0, 1.0, 1.0, 1.0, };
	SetMaterial(rgbaDiffuse, rgbaAmbient, rgbaSpecular, rgbaEmissive, 0.0f);
}
CCircle::~CCircle()
{
	SafeRelease(g_pVertexBuffer);
}
HRESULT CCircle::CreateVertexBuffer()
{
	g_pD3DDevice->CreateVertexBuffer(m_dwNumOfVertices * sizeof(CIRCLE_CUSTOMVERTEX),
		0, CIRCLE_D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVertexBuffer, 0);

	VOID* pVertices;
	CIRCLE_CUSTOMVERTEX ver[50];
	for (int i = 0; i < 50; i++)
	{
		ver[i] = { sinf(i * 2 * D3DX_PI / 50), 0, cosf(i * 2 * D3DX_PI / 50) };
	}

	g_pVertexBuffer->Lock(0, sizeof(ver), &pVertices, 0);
	memcpy(pVertices, ver, sizeof(ver));
	g_pVertexBuffer->Unlock();


	return S_OK;
}
DWORD CCircle::Render()
{
	g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CIRCLE_CUSTOMVERTEX));
	g_pD3DDevice->SetFVF(CIRCLE_D3DFVF_CUSTOMVERTEX);

	g_pD3DDevice->SetMaterial(&m_matMaterial);

	g_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, m_dwNumOfVertices);
	return S_OK;
}
bool CCircle::setSize(float radian)
{
	this->radian = radian;
	return true;
}
bool CCircle::CreateIndexBuffer()
{
	return true;
}
bool CCircle::SetMaterial(D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower)
{
	//Set the RGBA for diffuse light reflected from this material. 
	m_matMaterial.Diffuse = rgbaDiffuse;

	//Set the RGBA for ambient light reflected from this material. 
	m_matMaterial.Ambient = rgbaAmbient;

	//Set the color and sharpness of specular highlights for the material. 
	m_matMaterial.Specular = rgbaSpecular;
	m_matMaterial.Power = rPower;

	//Set the RGBA for light emitted from this material. 
	m_matMaterial.Emissive = rgbaEmissive;

	return true;
}