
float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjMatrix;

float4 gColor = float4(0, 0, 1, 1);

struct VS_INPUT 
{
    float4 mPosition : POSITION;
    float4 mColor : COLOR0;
};

struct PS_INPUT
{
    float4 mPosition : POSITION;
    float4 mColor : COLOR0;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPosition = Input.mPosition;
    Output.mPosition = mul(Output.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
    Output.mColor = Input.mColor;

   return Output;
}

float4 PS(PS_INPUT Input) : COLOR
{   
    //return gColor;
    return (Input.mColor);
}

//--------------------------------------------------------------//
// Technique Section for ColorShader
//--------------------------------------------------------------//
technique ColorShader
{
   pass Pass_0
   {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
   }

}

