#version 330 core
out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;

};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;


uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    float distance = length(lightDir);
    float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));

    vec3 ambient = light.ambient*texture(material.diffuse, TexCoords).rgb;


    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

    if (theta>light.outerCutOff){

        // diffuse
        vec3 norm = normalize(Normal);
        //    vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.diffuse *texture(material.diffuse, TexCoords).rgb;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 H_vector = normalize(lightDir + viewDir);
        float spec = pow(max(dot(H_vector, norm), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

        diffuse*=intensity;
        specular*=intensity;

        vec3 result = (ambient + diffuse + specular)*attenuation;

        FragColor = vec4(result, 1.0);

    } else {
        FragColor = vec4(ambient*attenuation, 1.0);
    }
} 
