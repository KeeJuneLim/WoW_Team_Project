#pragma once
 
class Boundary;
class IShader;

class IDisplayObject
{
protected:
	VARIABLE(bool, activated, Active);

	VARIABLE_P(D3DXVECTOR3, rotation, Rotation);
	VARIABLE_P(D3DXVECTOR3, scale, Scale);

	VARIABLE_P(D3DXMATRIX, localMatrix, LocalMatrix);
	VARIABLE_P(D3DXMATRIX, worldMatrix, WorldMatrix);

	VARIABLE(IDisplayObject*, root, Root);
	VARIABLE(IDisplayObject*, parent, Parent);
	VARIABLE_P(vector<IDisplayObject*>, children, Children);

	VARIABLE_P(D3DXVECTOR3, forward, Forward);
	VARIABLE_P(D3DXVECTOR3, deltaTransform, deltaTransform);
	VARIABLE_P(D3DXVECTOR3, targetPosition, TargetPosition);
	VARIABLE_P(D3DXVECTOR3, movePosition, MovePosition);
	VARIABLE_P(D3DXVECTOR3, moveDirection, MoveDirection);


	VARIABLE(float, moveSpeed, MoveSpeed);
	VARIABLE(float, rotationSpeed, RotationSpeed);
	VARIABLE(float, baseRotY, BaseRotY);

	VARIABLE_P(D3DXVECTOR3, velocity, Velocity);
	VARIABLE(float, jumpPower, JumpPower);

	VARIABLE(wstring, name, Name);
	VARIABLE(WORD, tag, Tag);
	VARIABLE(ShaderType, shaderType, ShaderType);

	VARIABLE(Material*, material, Material);
	VARIABLE(Boundary*, boundary, Boundary);
	VARIABLE(Hiding*, hiding, Hiding);
	VARIABLE(bool, isCastShadow, IsCastShadow);
	VARIABLE(bool, isReceiveShadow, IsReceiveShadow);

protected:
	IDirect3DVertexDeclaration9* vertexDecl;
	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib;
	UINT vertexStride;
	UINT vertexCount;
	UINT indexCount;

	IDisplayObject();
public:
	VARIABLE_P(D3DXVECTOR3, position, Position);

	virtual ~IDisplayObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Render(IShader* pShader) {};
	virtual void Draw() = 0;
	virtual void DrawPrimitive();
	virtual void DrawIndexedPrimitive();

	virtual void AddChild(IDisplayObject* pChild);
	IDisplayObject* FindChildByName(wstring name);
	void SetRootWithChildren(IDisplayObject* pObj);
	void SetShaderTypeWithChildren(ShaderType type);
	
	virtual void UpdateUI();
	void UpdateLocalMatrix();
	void UpdateWorldMatrix();
	void UpdateInput();
	void ApplyInput();
	void ApplyVelocity();
};

