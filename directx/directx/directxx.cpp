// directx.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "directx.h"
#include <d3d9.h>
#include <d3dx9.h>

LPDIRECT3D9 g_pD3D = NULL;//LP(������)
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //�ϵ���� ����, ����̽� �޸�
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPD3DXMESH g_pMesh = NULL;//���� ������
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

HRESULT initVertexBuffer()
{
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
	//return D3DXCreateTeapot(g_pD3DDevice, &g_pMesh, NULL);//������
	return D3DXCreateSphere(g_pD3DDevice, 1.0f, 50, 50, &g_pMesh, NULL);
	//return D3DXCreateBox(g_pD3DDevice, 1.0f, 1.0f, 1.0f, &g_pMesh, NULL);
}

void makeCamera()
{
	D3DXMATRIX matView;//
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 50, -150.0f), 
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

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 2);
	
	g_pD3DDevice->BeginScene();//

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	makeCamera();

	D3DXMATRIX temp;
	float rotate = -2000;
	//��ġ, ����, ����, ũ��, ������
	SetPosition(0, 0, 0, 4, nullptr);
	g_pMesh->DrawSubset(0);//�¾�
	SetPosition(7, 0, rotate, 1, nullptr);
	g_pMesh->DrawSubset(0);//��
	SetPosition(14, 0, rotate, 1.5, nullptr);
	g_pMesh->DrawSubset(0);//��
	temp=SetPosition(21, 500, rotate, 2, nullptr);
	g_pMesh->DrawSubset(0);//��
	SetPosition(1.7, 0, 0, 0.3, &temp);
	g_pMesh->DrawSubset(0);//��, ���� ���� �������
	SetPosition(28, 0, rotate, 2, nullptr);
	g_pMesh->DrawSubset(0);//ȭ
	SetPosition(42, 0, rotate, 3, nullptr);
	g_pMesh->DrawSubset(0);//��
	SetPosition(56, 0, rotate, 3, nullptr);
	g_pMesh->DrawSubset(0);//��

	//g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MY_VERTEX));//�ҽ�
	//g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);//����
	//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);//�׸���

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
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_HWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);//d3ddevice ,�ϵ���� ������ ���̾�, �ڵ�, vertex(3������ǥ ��),  d3d����, d3ddevice)
	initVertexBuffer();//

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
		}
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
