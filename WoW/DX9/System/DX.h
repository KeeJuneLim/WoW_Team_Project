#pragma once

struct D3DInfo
{
	CString appName;
	HINSTANCE hInstance;
	HWND hWnd;
	UINT screenWidth;
	UINT screenHeight;
	bool isWindowMode;
	bool useMultySample;
};


class DX
{
private:
	static DX*					instance;
	static LPDIRECT3DDEVICE9	pD3DDevice;
	static D3DInfo				d3dInfo;

	DX();
	~DX();

public:
	static DX* Get();
	static void Delete();
	static LPDIRECT3DDEVICE9 GetDevice() {return pD3DDevice; }
	static D3DInfo& GetInfo() { return d3dInfo; }
	static void SetInfo(D3DInfo info) { d3dInfo = info; }

	HRESULT				Init();
};
/*
http://telnet.or.kr/directx/graphics/dxgraphics.htm
P73 ~P99
Direct3D �� 3D ���� �ϵ��� �̿��� 3D ���� ǥ���� �����ִ� �׷��� API �̴�.
Application - Direct3D - HAL(Hardware Abstraction Layer) - Graphics Hardware
Device ���� �������� ������ D3D���� �����ϴ� ����� REF ��ġ�� ���� �̿��� �� �ִ�.
D3DDEVTYPE_HAL or D3DDEVTYPE_REF
REF ��ġ�� ���� ���� �������� �����ȴ�.DirectX SDK ���� ���Ե�.����� ����.

COM(Component Object Model)
IUnknown Interface �� ��ӹ���.�������� �κе��� ���������� �۵��ϸ� �ڽ��� �޸� ������ ������ �����Ѵ�.
Ư�� �Լ��� ���� ��ü�� �����͸� ��� Release Method �� ���� �����Ѵ�.(new, delete Ű���� ��� ����)
*/