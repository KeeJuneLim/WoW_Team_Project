

float3 gWorldCamPos;
float4x4 gVPMatrix;
float gAngle;

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

struct PS_Input
{
    float4 position : POSITION;
    float4 color : COLOR0;
    float pointSize : PSIZE;
    float2 tex : TEXCOORD0;
    
};



PS_Input VS_OUTLINE(VS_INPUT Input)
{
    PS_Input Out;
    //float4 p = lerp(float4(0.5, 0.5, 0.5, 1.0), Input.position, 0.99);
    float4 p = Input.position;
    
    Out.position = mul(p, gVPMatrix);
    
    float dist = length(Input.position.xyz - gWorldCamPos.xyz);
    Out.pointSize = 1000 / dist;

    Out.color = Input.color;
    
    Out.tex = float2(0, 0);

    return Out;
}


texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMap_Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
	ADDRESSU = CLAMP;
    ADDRESSV = CLAMP;
};
/*
struct PS_Input
{
    float2 tex : TEXCOORD0;
    float4 color : COLOR0;
};
*/
float4 PS_OUTLINE(PS_Input Input) : COLOR0
{
    float4 color;
    
    float fcos = cos(gAngle);
    float fsin = sin(gAngle);
    float2 uv = float2(Input.tex.x * 2.0f - 1.0f, Input.tex.y * 2.0f - 1.0f);
    //float2 uv = Input.tex;
    
    uv = float2(uv.x * fcos + uv.y * -fsin, uv.x * fsin + uv.y * fcos);
    uv = uv * 0.5f + 0.5f;
    color = Input.color * tex2D(DiffuseSampler, uv);
    //Out.color = tex2D(DiffuseSampler, Input.tex);

    return color;
}


//--------------------------------------------------------------//
// Technique Section for TextureMapping
//--------------------------------------------------------------//
technique TextureMapping
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 VS_OUTLINE();
        PixelShader = compile ps_2_0 PS_OUTLINE();
    }
    
}

