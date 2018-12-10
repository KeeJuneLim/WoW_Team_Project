#pragma once

class AseModelParts;
struct ASE_SCENE;

class AseLoader
{
private:
	static const int		TOKEN_SIZE = 256;
	char					m_szToken[TOKEN_SIZE];
	ifstream				m_fin;
	LPCTSTR					m_filepath;
	
	vector<D3DMATERIAL9>	m_mtlList;
	vector<CString>			m_diffuseMapNameList;
	map<string, AseModelParts*>	m_AseModelPartsList;

public:
	AseLoader();
	~AseLoader();

	AseModelParts*	Load(LPCTSTR filepath, LPCTSTR filename, OUT ASE_SCENE* pScene);
	int			GetTokenAsInt();
	float		GetTokenAsFloat();
	char*		GetTokenNoQuotation();

	void		ProcessSCENE(OUT ASE_SCENE* pScene);
	void		ProcessMATERIAL_LIST();
	void		ProcessMATERIAL(int index);
	void		ProcessMAP_DIFFUSE(int index);
	AseModelParts*	ProcessGEOMOBJECT(OUT AseModelParts* &pObj);
	void		ProcessNODE_TM(OUT AseModelParts* pObj);
	void		ProcessMESH(OUT AseModelParts* pObj);
	void		ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecV);
	void		ProcessMESH_FACE_LIST(IN vector<D3DXVECTOR3>& vecV, OUT vector<VERTEX_PNT>&	vecVertex);
	void		ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>& vecVT);
	void		ProcessMESH_TFACELIST(IN vector<D3DXVECTOR2>& vecVT, OUT vector<VERTEX_PNT>& vecVertex);
	void		ProcessMESH_NORMALS(OUT vector<VERTEX_PNT>&	vecVertex);
	void		ProcessTM_ANIMATION(OUT AseModelParts* pObj);
	void		ProcessPOS_TRACK(OUT AseModelParts* pObj);
	void		ProcessROT_TRACK(OUT AseModelParts* pObj);
};

