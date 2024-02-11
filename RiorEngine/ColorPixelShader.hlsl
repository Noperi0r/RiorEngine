
// Typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


// Pixel Shader: Shader entry point function where shader execution begins.
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}