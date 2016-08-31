#include "stdafx.h"
#include "SphereCreator.h"

SphereCreator::SphereCreator(LPDIRECT3DDEVICE9 pD3DDevice)
{
	g_pD3DDevice = pD3DDevice;
	g_pVertexBuffer = NULL;
}

HRESULT SphereCreator::initVertexBuffer()
{
	VOID* pVertices;
	MY_VERTEX ver[] = { 0, };
	g_pD3DDevice->CreateVertexBuffer(sizeof(MY_VERTEX) * 1, 0, D3DFVF_MYVERTEX, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);//�׷���ī�忡 ����ߵ�
	g_pVertexBuffer->Lock(0, sizeof(ver), &pVertices, 0); //�׷���ī�� �޸� �����ͼ� �� pVertices�� ����
	memcpy(pVertices, ver, sizeof(ver));
	g_pVertexBuffer->Unlock();
	return S_OK;
}
bool SphereCreator::Render()
{
	

	return true;
}