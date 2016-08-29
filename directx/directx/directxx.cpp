// directx.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "directx.h"
#include <d3d9.h>
#include <d3dx9.h>

LPDIRECT3D9 g_pD3D = NULL;//LP(포인터)
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //하드웨어 제어, 디바이스 메모리
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPD3DXMESH g_pMesh = NULL;//모델의 포인터
//BYTE, WORD, DWORD 데이터형

struct MY_VERTEX //점 데이터형
{
	float x, y, z, rhw;
	DWORD color;
};
//flexed vertex format
#define D3DFVF_MYVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

HWND g_HWnd = NULL; //생성한 윈도우 메모리 주소

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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
	//	&g_pVertexBuffer, NULL);//그래픽카드에 써줘야됨
	//g_pVertexBuffer->Lock(0, sizeof(ver), &pVertices, 0); //그래픽카드 메모리 가져와서 락 pVertices에 저장
	//memcpy(pVertices, ver, sizeof(ver));
	//g_pVertexBuffer->Unlock();
	//return S_OK;
	//return D3DXCreateTeapot(g_pD3DDevice, &g_pMesh, NULL);//주전자
	return D3DXCreateSphere(g_pD3DDevice, 1.0f, 50, 50, &g_pMesh, NULL);
	//return D3DXCreateBox(g_pD3DDevice, 1.0f, 1.0f, 1.0f, &g_pMesh, NULL);
}

void makeCamera()
{
	D3DXMATRIX matView;//
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 50, -150.0f), 
								&D3DXVECTOR3(0, 0, 0), 
								&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);//행렬투영(VIEW(카메라), PROJECTION(카메라설정), WORLD(월드좌표))
	
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 4/3, 1.0f, 500.0f);//
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

D3DXMATRIX SetPosition(float trans, float rotate1, float rotate2, float scale, D3DXMATRIX* matC)
{
	D3DXMATRIX matResult, matWorld, matRotate1, matRotate2, matScale;
	D3DXMatrixIdentity(&matResult);
	//행렬곱셈 순서중요
	if (scale > 0)
	{
		D3DXMatrixScaling(&matScale, scale, scale, scale);
		D3DXMatrixMultiply(&matResult, &matScale, &matResult);
	}
	if (rotate1 != 0)
	{
		D3DXMatrixRotationY(&matRotate1, GetTickCount() / rotate1);//라디안기준
		D3DXMatrixMultiply(&matResult, &matRotate1, &matResult);
	}
	D3DXMatrixTranslation(&matWorld, trans, 0, 0);
	D3DXMatrixMultiply(&matWorld, &matResult, &matWorld);
	if (rotate2 != 0)
	{
		D3DXMatrixRotationY(&matRotate2, GetTickCount() / rotate2);//라디안기준
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
	//위치, 자전, 공전, 크기, 이전값
	SetPosition(0, 0, 0, 4, nullptr);
	g_pMesh->DrawSubset(0);//태양
	SetPosition(7, 0, rotate, 1, nullptr);
	g_pMesh->DrawSubset(0);//수
	SetPosition(14, 0, rotate, 1.5, nullptr);
	g_pMesh->DrawSubset(0);//금
	temp=SetPosition(21, 500, rotate, 2, nullptr);
	g_pMesh->DrawSubset(0);//지
	SetPosition(1.7, 0, 0, 0.3, &temp);
	g_pMesh->DrawSubset(0);//달, 지구 자전 영향받음
	SetPosition(28, 0, rotate, 2, nullptr);
	g_pMesh->DrawSubset(0);//화
	SetPosition(42, 0, rotate, 3, nullptr);
	g_pMesh->DrawSubset(0);//목
	SetPosition(56, 0, rotate, 3, nullptr);
	g_pMesh->DrawSubset(0);//토

	//g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MY_VERTEX));//소스
	//g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);//포맷
	//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);//그리기

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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));//메모리 초기화
	//ZeroMemory(&d3dpp, sizeof(d3dpp));//동일
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//화면 갱신방법(버림)
	d3dpp.BackBufferFormat = d3ddm.Format;
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_HWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);//d3ddevice ,하드웨어 엑세스 레이어, 핸들, vertex(3차원좌표 점),  d3d설정, d3ddevice)
	initVertexBuffer();//

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX));

    MSG msg;
	msg.message = 0;//
	BOOL fMessage = FALSE;//
    // 기본 메시지 루프입니다.
    while (msg.message != WM_QUIT)//종료메시지까지 루프
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_KEYUP:
	{
		switch (wParam)//입력키
		{
		case VK_ESCAPE://가상키 esc
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

// 정보 대화 상자의 메시지 처리기입니다.
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
