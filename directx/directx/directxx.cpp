// directx.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "directx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Cuboid.h"
#include "Terrain.h"
#include "Sphere.h"
#include "Circle.h"
#include "Panel.h"
#include <time.h>
#include <sstream>

LPDIRECT3D9 g_pD3D = NULL;//LP(������)
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //�ϵ���� ����, ����̽� �޸�
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPD3DXMESH g_pMesh = NULL;//���� ������
D3DMATERIAL9 g_material;
CCuboid* g_pCube = NULL;
CTerrain* g_pTerrain = NULL;
CSphere* g_pSphere[10];
float zValue = 1.0f;
float xValue = 0.0f;
CCircle* g_pCircle = NULL;
CPanel* g_pPanel = NULL;
LPD3DXFONT g_pFont = NULL;

time_t current_time;
time_t before_time;
int frame;
DWORD dwTime=0, dwPreCount=0, dwCount=0;
//�۲�, ����, ����, ũ��, ���ڸ�, �������, ���� ���
//BYTE, WORD, DWORD ��������

struct MY_VERTEX //�� ��������
{
	float x, y, z, rhw;
	DWORD color;
};
//flexed vertex format
#define D3DFVF_MYVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

HWND g_HWnd = NULL; //������ ������ �޸� �ּ�

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Setup2DCamera()
{


	D3DXMATRIX matOrtho;
	D3DXMATRIX matIdentity;
	D3DXMatrixOrthoLH(&matOrtho, 800, 600, 0, 1.0f);
	D3DXMatrixIdentity(&matIdentity);

	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT initVertexBuffer()
{
	
	g_pPanel = new CPanel(g_pD3DDevice,265, 64, 800, 700);
	g_pPanel->SetTexture("EnergyBar.bmp", D3DCOLOR_XRGB(0, 0, 0));

	//VOID* pVertices;
	//MY_VERTEX ver[] =
	//{
	//	//{300.0f, 300.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0)},
	//	{240.0f, 196.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
	//	{360.0f, 196.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255)},
	//	{180.0f, 300.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 255) },
	//	{420.0f, 300.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 0)},
	//	{240.0f, 404.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 0) },
	//	{360.0f, 404.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 255) }
	//};
	//g_pD3DDevice->CreateVertexBuffer(sizeof(MY_VERTEX) * 6, 0, D3DFVF_MYVERTEX, D3DPOOL_DEFAULT,
	//	&g_pVertexBuffer, NULL);//�׷���ī�忡 ����ߵ�
	//g_pVertexBuffer->Lock(0, sizeof(ver), &pVertices, 0); //�׷���ī�� �޸� �����ͼ� �� pVertices�� ����
	//memcpy(pVertices, ver, sizeof(ver));
	//g_pVertexBuffer->Unlock();
	//return S_OK;
	//�۾�
	//HFONT hFont = CreateFont(FW_NORMAL, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");//�ʿ����
	D3DXCreateFont(g_pD3DDevice, 60, 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, L"Arial", &g_pFont);
	//�ð�
	dwTime = GetTickCount();
	time(&before_time);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);//zbuffer
	//g_pD3DDevice->set
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//////////LIGHT///////
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//��
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(32, 32, 32));
	D3DLIGHT9 light;
	memset(&light, 0, sizeof(light));//����ü �޸� �ʱ�ȭ
	//DIRECTIONAL���⸸ ����//POINT��ġ����//SPOT���������ġ����
	light.Type = D3DLIGHT_POINT;//FORCE �Ⱦ�
	light.Diffuse.r = 5.0f;
	light.Diffuse.g = 5.0f;
	light.Diffuse.b = 5.0f;

	light.Ambient.r = 0.2f;
	light.Ambient.g = 0.2f;
	light.Ambient.b = 0.2f;

	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;

	//light.Direction.x = 0.0f;
	//light.Direction.y = -1.0f;
	//light.Direction.z = 1.0f;
	light.Position.x = 0.0f;//
	light.Position.y = 0.0f;
	light.Position.z = 0.0f;

	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Range = 100.0f;
	//light.Phi = D3DX_PI*2;
	//light.Theta = D3DX_PI / 2.0f;
	//light.Falloff = D3DX_PI;

	g_pD3DDevice->SetLight(0, &light);
	g_pD3DDevice->LightEnable(0, TRUE);
	//////////LIGHT////////////////
	D3DCOLORVALUE dif = { 1.0f, 1.0f, 1.0f, 0.0f };
	D3DCOLORVALUE amb = { 1.0f, 1.0f, 1.0f, 0.0f };
	D3DCOLORVALUE spe = { 1.0f, 1.0f, 1.0f, 0.0f };
	D3DCOLORVALUE emi = { 0.0f, 0.0f, 0.0f, 0.0f };

	g_material.Diffuse = dif;//�ݻ��ϴ� ����
	g_material.Ambient = amb;
	g_material.Specular = spe;
	g_material.Power = 0;//����ŧ�� ����
	D3DXCreateTorus(g_pD3DDevice, 0.1f, 1.5f, 3, 50, &g_pMesh, NULL);

	//return D3DXCreateTeapot(g_pD3DDevice, &g_pMesh, NULL);//������
	//return D3DXCreateSphere(g_pD3DDevice, 1.0f, 50, 50, &g_pMesh, NULL);
	//return D3DXCreateBox(g_pD3DDevice, 1.0f, 1.0f, 1.0f, &g_pMesh, NULL);
	//g_pCube = new CCuboid(g_pD3DDevice);
	//g_pCube->SetSize(2, 2, 2);
	//g_pCube->SetTexture("1.bmp");

	//g_pTerrain = new CTerrain(g_pD3DDevice, 20, 20, 10.0f, 15);
	//g_pTerrain->SetTexture("Grass.bmp");
	for(int i=0;i<10;i++)
		g_pSphere[i] = new CSphere(g_pD3DDevice, 20, 20);
	g_pSphere[0]->SetTexture("sun.bmp");
	g_pSphere[0]->SetMaterial({ 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, 0.1f);
	g_pSphere[1]->SetTexture("mercury.bmp");
	g_pSphere[2]->SetTexture("venus.bmp");
	g_pSphere[3]->SetTexture("earth.bmp");
	g_pSphere[4]->SetTexture("mars.bmp");
	g_pSphere[5]->SetTexture("jupiter.bmp");
	g_pSphere[6]->SetTexture("saturn.bmp");
	g_pSphere[7]->SetTexture("uranus.bmp");
	g_pSphere[8]->SetTexture("neptune.bmp");
	g_pSphere[9]->SetTexture("moon.bmp");

	g_pCircle = new CCircle(g_pD3DDevice);

	return S_OK;
}

void makeCamera()
{
	D3DXMATRIX matView;//ī�޶� ȸ��
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(-150.f*zValue*sinf(xValue), 50.0f*zValue, -150.0f*zValue*cos(xValue)),
								&D3DXVECTOR3(0, 0, 0), 
								&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);//�������(VIEW(ī�޶�), PROJECTION(ī�޶���), WORLD(������ǥ))
	
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 4/3, 1.0f, 500.0f);//
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

D3DXMATRIX SetPosition(float trans, float rotate1, float rotate2, float scale, D3DXMATRIX* matC)
{
	D3DXMATRIX matResult, matWorld, matRotate1, matRotate2, matScale;
	D3DXMatrixIdentity(&matResult);
	//��İ��� �����߿�
	if (scale > 0)
	{
		D3DXMatrixScaling(&matScale, scale, scale, scale);
		D3DXMatrixMultiply(&matResult, &matScale, &matResult);
	}
	if (rotate1 != 0)
	{
		D3DXMatrixRotationY(&matRotate1, GetTickCount() / rotate1);//���ȱ���
		D3DXMatrixMultiply(&matResult, &matRotate1, &matResult);
	}
	D3DXMatrixTranslation(&matWorld, trans, 0, 0);
	D3DXMatrixMultiply(&matWorld, &matResult, &matWorld);
	if (rotate2 != 0)
	{
		D3DXMatrixRotationY(&matRotate2, GetTickCount() / rotate2);//���ȱ���
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotate2);
	}
	if (matC != NULL)
	{
		D3DXMatrixMultiply(&matWorld, &matWorld, matC);
	}

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//D3DXMatrixTranslation(&matWorld, 5, 0, 0);
	return matWorld;
}


void Render()
{
	if (g_pD3DDevice == NULL)
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER ,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 2);//�ʱ�ȭ ����
	
	g_pD3DDevice->BeginScene();//

	makeCamera();
	D3DXMATRIX temp, tempp;
	float rotate = 0.0f;
	//��ġ, ����, ����, ũ��, ������
	SetPosition(0.0f, 2000.0f, 0.0f, 10.0f, nullptr);
	g_pD3DDevice->SetMaterial(&g_material);
	//g_pMesh->DrawSubset(0);//�¾�
	//g_pCube->Render();
	g_pSphere[0]->Render();
	SetPosition(0.0f, 0.0f, 0.0f, 12.0f, NULL);
	g_pCircle->Render();
	SetPosition(12.0f, 0.0f, rotate, 0.7f, nullptr);
	//g_pMesh->DrawSubset(0);//��
	//g_pCube->Render();
	g_pSphere[1]->Render();
	SetPosition(17.0f, 0.0f, rotate*1.1f, 1.0f, nullptr);
	//g_pMesh->DrawSubset(0);//��
	//g_pCube->Render();
	g_pSphere[2]->Render();
	temp=SetPosition(24.0f, 500.0f, rotate*1.2f, 2.0f, nullptr);
	//g_pMesh->DrawSubset(0);//��
	//g_pCube->Render();
	g_pSphere[3]->Render();
	SetPosition(1.7f, 0.0f, -300.0f, 0.3f, &temp);
	//g_pMesh->DrawSubset(0);//��, ���� ���� �������
	//g_pCube->Render();
	g_pSphere[9]->Render();
	SetPosition(32.0f, 0.0f, rotate*1.4f, 2.0f, nullptr);
	//g_pMesh->DrawSubset(0);//ȭ
	//g_pCube->Render();
	g_pSphere[4]->Render();
	SetPosition(44.0f, 0.0f, rotate*1.7f, 3.0f, nullptr);
	//g_pMesh->DrawSubset(0);//��
	//g_pCube->Render();
	g_pSphere[5]->Render();
	temp = SetPosition(56.0f, 0.0f, rotate*1.8f, 3.0f, nullptr);
	//g_pMesh->DrawSubset(0);//��
	//g_pCube->Render();
	g_pSphere[6]->Render();
	D3DXMatrixRotationX(&tempp, D3DX_PI / 2);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &(tempp*temp));
	g_pMesh->DrawSubset(0);//��
	SetPosition(62.0f, 0.0f, rotate*2.0f, 1.5f, nullptr);
	g_pSphere[7]->Render();//õ
	SetPosition(67.0f, 0.0f, rotate*2.2f, 1.5f, nullptr);
	g_pSphere[8]->Render();//��

	
	//cube//
	//g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MY_VERTEX));//�ҽ�
	//g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);//����
	//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);//�׸���
	//cube//

	//g_pTerrain->Render();
	
	RECT rect;
	rect.left = 100; rect.top = 100; rect.right = 1000; rect.bottom = 1000;
	//g_pFont->DrawTextA(NULL, "�¾��", -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	DWORD dwNowTime = GetTickCount();
	float temp0 = (float)(dwNowTime - dwTime) / 1000.0f;
	temp0 = 1.0f / temp0;
	//if (dwNowTime - dwTime > 1000)
	//{
	//	dwTime = GetTickCount();
	//	dwPreCount = dwCount;
	//	dwCount = 0;
	//}
	//dwCount++;
	dwTime = GetTickCount();
	char text[256] = {};
	sprintf_s(text, "%.2f", temp0);
	g_pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));


	//time(&current_time);
	//std::stringstream stream;
	//float temp = (float)current_time - before_time / 1000.0f;

	//stream << current_time;
	//g_pFont->DrawTextA(NULL, stream.str().c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	//if (current_time - before_time == 1)
	//{
	//	stream << frame;
	//	frame = 0;
	//}
	//g_pFont->DrawTextA(NULL, stream.str().c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	//frame++;
	//before_time = current_time;

	//Setup2DCamera();
	//g_pPanel->MoveTo(300, 500);
	//g_pPanel->Render();
	

	g_pD3DDevice->EndScene();//
	g_pD3DDevice->Present(0, 0, 0, 0);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));//�޸� �ʱ�ȭ
	//ZeroMemory(&d3dpp, sizeof(d3dpp));//����
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ȭ�� ���Ź��(����)
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferWidth = 1024;
	d3dpp.BackBufferHeight = 768;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_HWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);//d3ddevice ,�ϵ���� ������ ���̾�, �ڵ�, vertex(3������ǥ ��),  d3d����, d3ddevice)
	initVertexBuffer();/////////////���ؽ� �ʱ�ȭ

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX));

    MSG msg;
	msg.message = 0;//
	BOOL fMessage = FALSE;//
    // �⺻ �޽��� �����Դϴ�.
    while (msg.message != WM_QUIT)//����޽������� ����
    {
		fMessage = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if (fMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
		{
			Render();
		}
    }

    return (int) msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECTX);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_HWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// �޴� ������ ���� �м��մϴ�.
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYUP:
		{
			switch (wParam)//�Է�Ű
			{
				case VK_ESCAPE://����Ű esc
					DestroyWindow(hWnd);
					return 0;
				case VK_RIGHT:
				{
					xValue += 0.3;
					break;
				}
				case VK_LEFT:
				{
					xValue -= 0.3f;
					break;
				}
				case VK_UP:
					g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE); break;
				case VK_DOWN:
					g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); break;
			}
			break;
		case WM_MOUSEWHEEL:
		{
			if ((short)HIWORD(wParam) < 0)//�ٴٿ�
			{
				zValue += 0.1f;
			}
			else//�پ�
			{
				zValue -= 0.1f;
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
