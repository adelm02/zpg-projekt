#version 330 core
const int MAX_LIGHTS = 30;

struct Light {
    int   type;
    vec3  position;
    vec3  color;
    vec3  atten;
    vec3  direction;
    float cutoff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform Light lights[MAX_LIGHTS];
uniform int nol;

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D diffuseTexture;
uniform bool useTexture;

vec3 applyPoint(Light l, vec3 N, vec3 V) {
    vec3  L = normalize(l.position - FragPos);
    float d = length(l.position - FragPos);
    float denom = l.atten.x + l.atten.y * d + l.atten.z * d * d;
    float f = 1.0 / max(denom, 0.001);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(L + viewDir);

    float diff = max(dot(N, L), 0.0);
    float spec = 0.0;

    const float shininess = 32.0;
    const float specularStrength = 0.5;

    if (diff > 0.0) {
        spec = pow(max(dot(N, halfwayDir), 0.0), shininess);
    }

    vec3 ambient = 0.15 * l.color;
    vec3 diffuse = diff * l.color;
    vec3 specular = specularStrength * spec * l.color;

    return (ambient + diffuse + specular) * f;
}

vec3 applyDirectional(Light l, vec3 N, vec3 V) {
    vec3 L = normalize(-l.position);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(L + viewDir);

    float diff = max(dot(N, L), 0.0);
    float spec = 0.0;

    const float shininess = 32.0;
    const float specularStrength = 0.5;

    if (diff > 0.0) {
        spec = pow(max(dot(N, halfwayDir), 0.0), shininess);
    }

    vec3 ambient  = 0.15 * l.color;
    vec3 diffuse  = diff * l.color;
    vec3 specular = specularStrength * spec * l.color;

    return ambient + diffuse + specular;
}

vec3 applySpotlight(Light l, vec3 N, vec3 V) {
    vec3 L = normalize(l.position - FragPos);
    float d = length(l.position - FragPos);

    vec3 spotDir = normalize(-l.direction);
    float theta = dot(L, spotDir);
    float cutoffCos = cos(l.cutoff);

    if (theta < cutoffCos) {
        return vec3(0.0);
    }

    float intensity = smoothstep(cutoffCos, cutoffCos + 0.1, theta);
    float denom = l.atten.x + l.atten.y * d + l.atten.z * d * d;
    float f = 1.0 / max(denom, 0.001);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(L + viewDir);

    float diff = max(dot(N, L), 0.0);
    float spec = 0.0;

    const float shininess = 32.0;
    const float specularStrength = 0.5;

    if (diff > 0.0) {
        spec = pow(max(dot(N, halfwayDir), 0.0), shininess);
    }

    vec3 ambient = 0.05 * l.color;
    vec3 diffuse = diff * l.color;
    vec3 specular = specularStrength * spec * l.color;

    return (ambient + diffuse + specular) * f * intensity;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    for (int i = 0; i < nol; ++i) {
        if (lights[i].type == 1) {
            result += applyPoint(lights[i], N, V);
        } else if (lights[i].type == 2) {
            result += applySpotlight(lights[i], N, V);
        } else {
            result += applyDirectional(lights[i], N, V);
        }
    }

    vec3 baseColor;
    if (useTexture) {
        baseColor = texture(diffuseTexture, TexCoord).rgb;
    } else {
        baseColor = objectColor;
    }

    FragColor = vec4(result * baseColor, 1.0);
}