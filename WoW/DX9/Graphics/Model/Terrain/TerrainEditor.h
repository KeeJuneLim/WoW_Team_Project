#pragma once

class Terrain;

class TerrainEditor
{
	
public:
	enum EditorMenu { Data, Edit};
	enum BrushType { BT_None, BT_Sum, BT_Mean, BT_Converge, BT_Blending };
	enum BrushShape { BS_None, BS_Rect, BS_Circle };
	
	const string typeStr[5] = {
		string("BT_None"), string("BT_Sum"),string("BT_Mean"), string("BT_Converge"), string("BT_Blending")
	};
	const string shapeStr[3] = {
		string("BS_None"), string("BS_Rect"),string("BS_Circle")
	};
	
	Terrain* pTerrain;

	EditorMenu menuIndex = Data;
	int terrainDim = 65;
	BrushType type = BT_None;
	BrushShape shape = BS_None;
	D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 color = D3DXVECTOR3(0.5f, 0.5f, 0);
	float range = 2.0f;
	float intensity = 10.0f;
	float converge = 0.0f;
	bool fit = false;

	int currentDiffuseMapIndex = 0;
	int blendingTexDim = 65;
	int currentBlendingChannel = 0;
	LPDIRECT3DTEXTURE9 blendingTex = NULL;
	
	TerrainEditor();
	TerrainEditor(Terrain* pTerrain);
	
	function<void(wstring)> OnSaveDataFunc;
	void OnSaveData(wstring filename);
	function<void(wstring)> OnOpenDataFunc;
	void OnOpenData(wstring filename);
	function<void(wstring)> OnOpenDiffuseMapFunc;
	void OnOpenDiffuseMap(wstring filename);
	function<void(wstring)> OnCreateBlendingMapFunc;
	void OnCreateBlendingMap(wstring filename);
	function<void(wstring)> OnOpenBlendingMapFunc;
	void OnOpenBlendingMap(wstring filename);
	function<void(wstring)> OnSaveBlendingMapFunc;
	void OnSaveBlendingMap(wstring filename);

	void Init();
	void UpdateUI();
	void ComputeEditedIndices(BYTE* vertices, UINT stride, vector<UINT> &editedIndices, UINT dimension);
	void EditHeight(BYTE* vertices, UINT stride, vector<UINT> &editedIndices, UINT dimension);
	void EditBlendingTex(UINT dimension);
};