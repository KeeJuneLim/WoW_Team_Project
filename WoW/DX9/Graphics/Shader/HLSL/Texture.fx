
float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjMatrix;

float gThickness = 0.1f;
float4 gOutlineColor = float4(1, 0, 0, 1);
float gConst;

texture DiffuseMapTex;
sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMapTex);
};

struct VS_INPUT 
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
   float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
};



PS_INPUT VS_OUTLINE(VS_INPUT Input)
{
    PS_INPUT Output;
    Input.mNormal = normalize(Input.mNormal);
    Output.mPosition = float4(Input.mPosition.xyz + (Input.mNormal * gThickness), 1);
    Output.mPosition = mul(Output.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
   
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS_OUTLINE(PS_INPUT Input) : COLOR
{
    return gOutlineColor;
}


PS_INPUT VS1(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
    
    Input.mTexCoord.y = Input.mTexCoord.y + gConst;
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS1(PS_INPUT Input) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
    return albedo.rgba;
}
//--------------------------------------------------------------//
// Technique Section for TextureMapping
//--------------------------------------------------------------//
technique TextureMapping
{
	pass Pass_0
	{
		CULLMODE = CW;
		VertexShader = compile vs_2_0 VS_OUTLINE();
		PixelShader = compile ps_2_0 PS_OUTLINE();
	}
	pass Pass_1
	{
        CULLMODE = CCW;
        VertexShader = compile vs_2_0 VS1();
        PixelShader = compile ps_2_0 PS1();
    }
}

