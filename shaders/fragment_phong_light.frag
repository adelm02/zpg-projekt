#version 330 core
#define MAX_LIGHTS 64

struct Light {
    int type;
    vec3 position;
    vec3 color;
    vec3 atten;
    vec3 direction;
    float cutoff;
};

uniform Light lights[MAX_LIGHTS];
uniform int nol;

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
    float h;
};

uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;

uniform sampler2D diffuseTexture;
uniform bool useTexture;

vec3 applyPoint(Light l, vec3 N, vec3 V) {
    vec3 L = normalize(l.position - FragPos);
    float d = length(l.position - FragPos);
    float f = 1.0 / max((l.atten.x + l.atten.y * d + l.atten.z * d * d), 0.001);

    float diff = max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), material.h);

    vec3 ambient  = material.ra * l.color;
    vec3 diffuse  = material.rd * diff * l.color;
    vec3 specular = material.rs * spec * l.color;

    return (ambient + diffuse + specular) * f;
}

vec3 applyDirectional(Light l, vec3 N, vec3 V) {
    vec3 L = normalize(-l.position);
    float diff = max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), material.h);

    vec3 ambient  = material.ra * l.color;
    vec3 diffuse  = material.rd * diff * l.color;
    vec3 specular = material.rs * spec * l.color;

    return ambient + diffuse + specular;
}

vec3 applySpotlight(Light l, vec3 N, vec3 V) {
    vec3 L = normalize(l.position - FragPos);
    float d = length(l.position - FragPos);
    vec3 spotDir = normalize(-l.direction);

    float theta = dot(L, spotDir);
    float cutoffCos = cos(l.cutoff);
    if (theta < cutoffCos) return vec3(0.0);

    float intensity = smoothstep(cutoffCos, cutoffCos + 0.1, theta);
    float f = 1.0 / max((l.atten.x + l.atten.y * d + l.atten.z * d * d), 0.001);

    float diff = max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), material.h);

    vec3 ambient  = material.ra * l.color;
    vec3 diffuse  = material.rd * diff * l.color;
    vec3 specular = material.rs * spec * l.color;

    return (ambient + diffuse + specular) * f * intensity;
}

void main() {
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 globalAmbient = vec3(0.07, 0.07, 0.07);
    vec3 result = globalAmbient;


    for (int i = 0; i < nol; ++i) {
        if (lights[i].type == 1)
        result += applyPoint(lights[i], N, V);
        else if (lights[i].type == 2)
        result += applySpotlight(lights[i], N, V);
        else
        result += applyDirectional(lights[i], N, V);
    }


    vec3 baseColor;
    if (useTexture) {
        baseColor = texture(diffuseTexture, TexCoord).rgb;
    } else {
        baseColor = objectColor;
    }

    FragColor = vec4(result * baseColor, 1.0);
}