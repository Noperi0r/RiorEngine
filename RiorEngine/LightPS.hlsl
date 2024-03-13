Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET // SV_TARGET: The output value that will be stored in a render target
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;
    
    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Calculate the amount of light on this pixel.
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir)); // Clamp value within the range of 0~1
    
    if(lightIntensity > 0.0f) // If NdotL value is negative, it will subtract away some of the ambient color in the addition which is not correct.
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += diffuseColor * lightIntensity;
        color = saturate(color);
        
        // Specular lighting 
        reflection = normalize(2.0f * lightIntensity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }
    
    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;
    
    // Add the specular component last to the output color.
    color = saturate(color + specular);
    
    return color;
}