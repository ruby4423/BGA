// directx.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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

LPDIRECT3D9 g_pD3D = NULL;//LP(포인터)
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //하드웨어 제어, 디바이스 메모리
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPD3DXMESH g_pMesh = NULL;//모델의 포인터
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
//글꼴, 높이, 넓이, 크기, 이텔릭, 언더라인, 볼드 등등
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
	//	&g_pVertexBuffer, NULL);//그래픽카드에 써줘야됨
	//g_pVertexBuffer->Lock(0, sizeof(ver), &pVertices, 0); //그래픽카드 메모리 가져와서 락 pVertices에 저장
	//memcpy(pVertices, ver, sizeof(ver));
	//g_pVertexBuffer->Unlock();
	//return S_OK;
	//글씨
	//HFONT hFont = CreateFont(FW_NORMAL, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");//필요업슴
	D3DXCreateFont(g_pD3DDevice, 60, 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, L"Arial", &g_pFont);
	//시간
	dwTime = GetTickCount();
	time(&before_time);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);//zbuffer
	//g_pD3DDevice->set
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//////////LIGHT///////
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//빛
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(32, 32, 32));
	D3DLIGHT9 light;
	memset(&light, 0, sizeof(light));//구조체 메모리 초기화
	//DIRECTIONAL방향만 존재//POINT위치감쇠//SPOT감쇠방향위치각도
	light.Type = D3DLIGHT_POINT;//FORCE 안씀
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

	g_material.Diffuse = dif;//반사하는 색상
	g_material.Ambient = amb;
	g_material.Specular = spe;
	g_material.Power = 0;//스펙큘러 세기
	D3DXCreateTorus(g_pD3DDevice, 0.1f, 1.5f, 3, 50, &g_pMesh, NULL);

	//return D3DXCreateTeapot(g_pD3DDevice, &g_pMesh, NULL);//주전자
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
	D3DXMATRIX matView;//카메라 회전
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(-150.f*zValue*sinf(xValue), 50.0f*zValue, -150.0f*zValue*cos(xValue)),
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

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER ,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 2);//초기화 색상
	
	g_pD3DDevice->BeginScene();//

	makeCamera();
	D3DXMATRIX temp, tempp;
	float rotate = 0.0f;
	//위치, 자전, 공전, 크기, 이전값
	SetPosition(0.0f, 2000.0f, 0.0f, 10.0f, nullptr);
	g_pD3DDevice->SetMaterial(&g_material);
	//g_pMesh->DrawSubset(0);//태양
	//g_pCube->Render();
	g_pSphere[0]->Render();
	SetPosition(0.0f, 0.0f, 0.0f, 12.0f, NULL);
	g_pCircle->Render();
	SetPosition(12.0f, 0.0f, rotate, 0.7f, nullptr);
	//g_pMesh->DrawSubset(0);//수
	//g_pCube->Render();
	g_pSphere[1]->Render();
	SetPosition(17.0f, 0.0f, rotate*1.1f, 1.0f, nullptr);
	//g_pMesh->DrawSubset(0);//금
	//g_pCube->Render();
	g_pSphere[2]->Render();
	temp=SetPosition(24.0f, 500.0f, rotate*1.2f, 2.0f, nullptr);
	//g_pMesh->DrawSubset(0);//지
	//g_pCube->Render();
	g_pSphere[3]->Render();
	SetPosition(1.7f, 0.0f, -300.0f, 0.3f, &temp);
	//g_pMesh->DrawSubset(0);//달, 지구 자전 영향받음
	//g_pCube->Render();
	g_pSphere[9]->Render();
	SetPosition(32.0f, 0.0f, rotate*1.4f, 2.0f, nullptr);
	//g_pMesh->DrawSubset(0);//화
	//g_pCube->Render();
	g_pSphere[4]->Render();
	SetPosition(44.0f, 0.0f, rotate*1.7f, 3.0f, nullptr);
	//g_pMesh->DrawSubset(0);//목
	//g_pCube->Render();
	g_pSphere[5]->Render();
	temp = SetPosition(56.0f, 0.0f, rotate*1.8f, 3.0f, nullptr);
	//g_pMesh->DrawSubset(0);//토
	//g_pCube->Render();
	g_pSphere[6]->Render();
	D3DXMatrixRotationX(&tempp, D3DX_PI / 2);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &(tempp*temp));
	g_pMesh->DrawSubset(0);//고리
	SetPosition(62.0f, 0.0f, rotate*2.0f, 1.5f, nullptr);
	g_pSphere[7]->Render();//천
	SetPosition(67.0f, 0.0f, rotate*2.2f, 1.5f, nullptr);
	g_pSphere[8]->Render();//해

	
	//cube//
	//g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MY_VERTEX));//소스
	//g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);//포맷
	//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);//그리기
	//cube//

	//g_pTerrain->Render();
	
	RECT rect;
	rect.left = 100; rect.top = 100; rect.right = 1000; rect.bottom = 1000;
	//g_pFont->DrawTextA(NULL, "태양계", -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

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
	d3dpp.BackBufferWidth = 1024;
	d3dpp.BackBufferHeight = 768;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_HWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);//d3ddevice ,하드웨어 엑세스 레이어, 핸들, vertex(3차원좌표 점),  d3d설정, d3ddevice)
	initVertexBuffer();/////////////버텍스 초기화

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
			if ((short)HIWORD(wParam) < 0)//휠다운
			{
				zValue += 0.1f;
			}
			else//휠업
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
