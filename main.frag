#version 420

in vec2 TexCoord;
in vec3 posWorld;
in vec3 normalWorld;

out vec4 colour;

layout(std140) uniform Fragment
{
    vec3 viewPosition; // 12    4
    bool useTexture; // 4       0
    // 16
};

layout(std140) uniform Material
{
    float ambient; // 4     0
    float shininess; // 4   4
    float diffuse; // 4     8
    float specular; // 4    12
    // 16
} material;

uniform sampler2D theTexture;
layout(binding=0) uniform samplerCube skybox;
layout(binding=1) uniform samplerCube diffuseCubemap; 

void main()
{
	vec3 toEye = normalize(viewPosition - posWorld);
    
    vec4 diffuse = texture(diffuseCubemap, normalWorld);
    vec4 specular = texture(skybox, reflect(-toEye, normalWorld));
    
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, material.shininess);
    
    diffuse.xyz *= material.diffuse;
    specular.xyz *= material.specular;
    
    if(useTexture)
    {
        diffuse *= texture(theTexture, TexCoord);
    }
    
    colour = diffuse + specular;
}