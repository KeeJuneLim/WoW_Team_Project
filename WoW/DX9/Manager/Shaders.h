#pragma once
#include "Graphics/Shader/IShader.h"

//셰이더 클래스들을 관리

class Shaders
{
public:
	static Shaders*	Get() {
		if (instance == NULL) instance = new Shaders();
		return instance;
	}
	static void		Delete() { SAFE_DELETE(instance); }

private:
	static Shaders*	instance;
	Shaders();
	~Shaders();

	map<wstring, LPD3DXEFFECT> m_pEffects;
	vector<IShader*> shaderList;
	
	void AddShader(IShader* pShader, ShaderType type);
public:
	void Init();

	IShader* GetShader(ShaderType type) { return shaderList[type]; }
	
	LPD3DXEFFECT GetEffect(wstring filePath);
	LPD3DXEFFECT LoadEffect(wstring fullPath);
};