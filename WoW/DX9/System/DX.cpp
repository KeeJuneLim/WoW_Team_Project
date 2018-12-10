#include "StdAfx.h"
#include "DX.h"

DX* DX::instance = NULL;
LPDIRECT3DDEVICE9 DX::pD3DDevice = NULL;
D3DInfo DX::d3dInfo;

DX * DX::Get()
{
	if (instance == NULL)
		instance = new DX();

	return instance;
}

void DX::Delete()
{
	SAFE_DELETE(instance);
}

DX::DX()
{
}

DX::~DX()
{
	if (pD3DDevice)
	{
		ULONG ul = pD3DDevice->Release();
		//assert(ul == 0 && "Release All Instance");
	}
}

HRESULT DX::Init()
{
	//���� ������ ���� IDirect3D9 Interface �� ������ ȹ��
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert(pD3D != NULL && "��� ����");

	D3DCAPS9	caps;	// ��ġ�� ���� ��	���� ����
	int			vp;		// ���ؽ��� ó�� �ϴ� ���
	
	//�� �׷���ī���� ������ D3DCAPS9 �� �޾ƿ´�.
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;
	
	//�ϵ��� ����ó���� �����ϴ��� Ȯ��
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING; 
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	//������ IDirect3DDevice9 interface �� Ư���� �������ش�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	//Present �� Swap �� BackBuffer �� �����͸� ������ ���� ��.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//���� ���� �ڵ� ����
	d3dpp.EnableAutoDepthStencil = TRUE;
	//���̹��� ����. ���� ��Ʈ ���� ���� ���� ��Ȯ�� ���� üũ ����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	
	if (d3dInfo.useMultySample)
	{
		DWORD level;
		if (SUCCEEDED(pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, D3DFMT_D24S8, TRUE, D3DMULTISAMPLE_4_SAMPLES, &level)))
		{
			d3dpp.MultiSampleQuality = level - 1;
			d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		}
	}
	
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = d3dInfo.hWnd;
	d3dpp.Windowed = d3dInfo.isWindowMode;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = d3dInfo.screenWidth;
	d3dpp.BackBufferHeight = d3dInfo.screenHeight;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		d3dInfo.hWnd, vp, &d3dpp, &pD3DDevice);
	assert(pD3DDevice != NULL && "��ġ���� ����");
	
	SAFE_RELEASE(pD3D);

	return S_OK;
}

