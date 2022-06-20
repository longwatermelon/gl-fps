#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
}; 

in vec2 TexCoords;

struct Light {
    int type;

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 spotlight_dir;
    float spotlight_cutoff;
    float spotlight_outer_cutoff;

    vec3 directional_dir;
};

in vec3 FragPos;
in vec3 Normal;
 
uniform vec3 viewPos;
uniform Material material;

#define nlights 2
uniform Light lights[nlights];

vec3 calculate_point_light(Light light)
{
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords));

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculate_spotlight_light(Light light)
{
    float theta = dot(normalize(light.position - FragPos), normalize(-light.spotlight_dir)); 
    float epsilon = (light.spotlight_cutoff - light.spotlight_outer_cutoff);
    float intensity = clamp((theta - light.spotlight_outer_cutoff) / epsilon, 0.0, 1.0);

    return calculate_point_light(light) * intensity;
}

vec3 calculate_light(Light light)
{
    float distance = length(light.position - FragPos);
    float att = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // The only difference between point and directional light is lightDir
    if (light.type == 0) return att * calculate_point_light(light);
    if (light.type == 1) return att * calculate_spotlight_light(light);

    return vec3(0.0);
}

void main()
{
    vec3 res = vec3(0.0);

    for (int i = 0; i < nlights; i++)
        res += calculate_light(lights[i]);

    FragColor = vec4(res, 1.0);
}

