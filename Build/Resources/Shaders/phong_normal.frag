#version 430 core
in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} fs_in;

out vec4 outColor;

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (binding = 0) uniform sampler2D colorSampler;
layout (binding = 1) uniform sampler2D normalSampler;

void main()
{
    mat4 model_view = view * model;

    // ambient
    vec3 ambient = light.ambient;

    //diffuse
    vec3 vnormal = mat3(model_view) * fs_in.normal;
    vec4 vposition = model_view * vec4(fs_in.position, 1);
    vec3 light_dir = normalize(vec3(light.position - vposition));

    float intensity = max(dot(light_dir, vnormal), 0);
    vec3 diffuse = material.diffuse * light.diffuse * intensity;

    // specular
    vec3 specular = vec3(0);
    if(intensity > 0)
    {
        vec3 view_dir = normalize(-vec3(vposition));
        vec3 reflection = reflect(-light_dir, vnormal);
        intensity = max(dot(view_dir, reflection), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * light.specular * intensity;
    }
    
    outColor = vec4(ambient + diffuse, 1) * texture(colorSampler, fs_in.texcoord) + vec4(specular, 1);
}