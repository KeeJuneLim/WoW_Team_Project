
float3 gWorldLightDir;
float4 gWorldCameraPos;
float4 lhtDir = { 0.0f, 0.0f, -1.0f, 1.0f }; //light Direction 
float4 lightDiffuse = { 0.6f, 0.6f, 0.6f, 1.0f }; // Light Diffuse
float4 MaterialAmbient : MATERIALAMBIENT = { 0.1f, 0.1f, 0.1f, 1.0f };
float4 MaterialDiffuse : MATERIALDIFFUSE = { 0.8f, 0.8f, 0.8f, 1.0f };

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3 mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4 mViewProj : VIEWPROJECTION;

texture DiffuseMap_Tex;

sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMap_Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};



///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 BlendWeights : BLENDWEIGHT;
    float4 BlendIndices : BLENDINDICES;
    float3 Normal : NORMAL;
    float3 Tex0 : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float Diffuse : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float3 Reflection : TEXCOORD3;
};

float3 Diffuse(float3 Normal)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Normal, lhtDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


PS_INPUT VS(VS_INPUT i, uniform int NumBones)
{
    PS_INPUT o;
    float3 Pos = 0.0f;
    float3 Normal = 0.0f;
    float LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4]) i.BlendWeights;
    int IndexArray[4] = (int[4]) IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones - 1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, (float3x3)mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight;

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones - 1]]) * LastWeight);
    Normal += (mul(i.Normal, (float3x3)mWorldMatrixArray[IndexArray[NumBones - 1]]) * LastWeight);
    
    // transform position from world space into view and then projection space
    o.Position = mul(float4(Pos.xyz, 1.0f), mViewProj);


    float3 viewDir = normalize(Pos.xyz - gWorldCameraPos.xyz);
    o.ViewDir = viewDir;

    // normalize normals
    Normal = normalize(Normal);

    o.Diffuse = dot(-gWorldLightDir, Normal);
    o.Reflection = reflect(gWorldLightDir, Normal);


    // Shade (Ambient + etc.)
    //o.Diffuse = MaterialAmbient.xyz + Diffuse(Normal) * MaterialDiffuse.xyz;
    
    // copy the input texture coordinate through
    o.TexCoord = i.Tex0.xy;

    return o;
}


float4 PS(PS_INPUT Input) : COLOR
{
    float3 diffuse = saturate(Input.Diffuse);
    float3 reflection = normalize(Input.Reflection);
    float3 viewDir = normalize(Input.ViewDir);
    float3 specular = 0;
   
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
        specular = specular;
    }

    float3 albedo = tex2D(DiffuseSampler, Input.TexCoord).rgb;
    diffuse = diffuse * albedo * MaterialDiffuse.rgb;
    
    float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo * MaterialAmbient.rgb;
   
    return float4(diffuse + ambient + specular, 1);
}



int CurNumBones = 2;
VertexShader vsArray[4] =
{
    compile vs_2_0 VS(1),
	compile vs_2_0 VS(2),
	compile vs_2_0 VS(3),
    compile vs_2_0 VS(4)
};


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
        PixelShader = compile ps_2_0 PS();
    }
}

