#version 330 core
const int MAX_LIGHTS = 30;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform int  lightCount;
uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec3 lightAtten[MAX_LIGHTS];

uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    vec3 specular = vec3(0.0);
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    const float maxRange = 12.0;

    const float shininess = 6.0;
    const float specularStrength = 0.05;

    for (int i = 0; i < lightCount; i++) {
        vec3 L = normalize(lightPos[i] - FragPos);


        float d = length(lightPos[i] - FragPos);
        vec3  a = lightAtten[i];
        float denom = a.x + a.y * d + a.z * d * d;
        float f  = 1.0 / (a.x + a.y*d + a.z*d*d);
        float fade = clamp(1.0 - d / maxRange, 0.0, 1.0);

        float diff = max(dot(norm, L), 0.0);

        float spec = 0.0;
        if (diff > 0.0) {
            vec3 R = reflect(-L, norm);
            spec = pow(max(dot(viewDir, R), 0.0), shininess);
        }


        //ambient += 0.05 * lightColor[i];
        ambient += (0.09 / float(lightCount)) * lightColor[i];
        diffuse += diff * lightColor[i] * f * fade;
        specular += specularStrength * spec * lightColor[i] * f * fade;
    }

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}

