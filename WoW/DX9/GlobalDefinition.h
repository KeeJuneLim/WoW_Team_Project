#pragma once

const wstring ASSET_PATH = _T("../../_Assets/");
const wstring SHADER_PATH = _T("Graphics/Shader/HLSL/");


#define SAFE_UPDATE(p) if(p) { (p)->Update(); }
#define SAFE_RENDER(p) if(p) { (p)->Render(); }
#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

#define VARIABLE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define VARIABLE_P(varType, varName, funName)\
public: varType varName;\
public: inline varType* Get##funName(void) { return &varName; }\
public: inline void Set##funName(varType* var){ varName = *var; }

#define VARIABLE_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define STATIC_VARIABLE(varType, varName, funName)\
protected: static varType varName;\
public: inline static varType Get##funName(void) { return varName; }\
public: inline static void Set##funName(varType var){ varName = var; }

#define STATIC_VARIABLE_P(varType, varName, funName)\
protected: static varType varName;\
public: inline static varType* Get##funName(void) { return &varName; }\
public: inline static void Set##funName(varType* var){ varName = *var; }




