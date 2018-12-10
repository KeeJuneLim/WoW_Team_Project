#pragma once

class AseModel;
struct ASE_POS_SAMPLE;
struct ASE_ROT_SAMPLE;

class AseModelParts : public IDisplayObject
{
	typedef VERTEX_PNT VertexType;
protected:
	VARIABLE_P(vector<VertexType>, vertices, Vertices);
private:

	vector<ASE_POS_SAMPLE>	m_vecPosTrack;
	vector<ASE_ROT_SAMPLE>	m_vecRotTrack;

	AseModel* model;
public:
	AseModelParts();
	virtual ~AseModelParts();
	
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	void SetModel(AseModel* pModel);
	void InitEachData();
	void InitLocalMatrix(AseModelParts* pParent = NULL);

	void UpdateLocalTranslationMatrix(int tickCount, OUT D3DXMATRIXA16& mat);
	void UpdateLocalRotationMatrix(int tickCount, OUT D3DXMATRIXA16& mat);
	
	vector<ASE_POS_SAMPLE>* GetPosTrack() { return &m_vecPosTrack; }
	vector<ASE_ROT_SAMPLE>* GetRotTrack() { return &m_vecRotTrack; }

};


struct ASE_POS_SAMPLE
{
	int				tick;
	D3DXVECTOR3		position;
};

struct ASE_ROT_SAMPLE
{
	int				tick;
	D3DXQUATERNION	quaternion;
};

struct ASE_SCENE
{
	int numFirstFrame;
	int numLastFrame;
	int FrameSpeed;
	int TicksPerFrame;
};
