
float4x4 gWVPMatrix;

int gIsPicked = 0;

struct VS_INPUT 
{
    float4 Position : POSITION;
};

struct PS_INPUT
{
    float4 Position : POSITION;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT Output;
    Output.Position = mul(input.Position, gWVPMatrix);
    
	return Output;
}

float4 PS_White(PS_INPUT input) : COLOR
{   
    return float4(1.0f, 1.0f, 1.0f, 0.4f);
}

float4 PS_Green(PS_INPUT input) : COLOR
{
    return float4(0.0f, 1.0f, 0.0f, 0.4f);
}

//--------------------------------------------------------------//
// Technique Section for ColorShader
//--------------------------------------------------------------//

PixelShader psArray[2] =
{
    compile ps_2_0 PS_White(),
	compile ps_2_0 PS_Green()
};

technique ColorShader
{
   pass Pass_0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = (psArray[gIsPicked]);
	}

}

