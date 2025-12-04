#version 330 core
#define MAX_LIGHTS 64

struct Light {
    int type;      // 0=directional, 1=point, 2=spotlight
    vec3 position;
    vec3 color;
    vec3 atten;
    vec3 direction;
    float cutoff;
};

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
    float h;  // shininess
};

uniform Light lights[MAX_LIGHTS];
uniform int nol;
uniform Material material;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;


vec3 calculateLight(Light light, vec3 N, vec3 V) {
    vec3 L;
    float attenuation = 1.0;

    if (light.type == 0) {
// Directional
        L = normalize(-light.position);
    } else {
//point
        L = normalize(light.position - FragPos);
        float d = length(light.position - FragPos);
        attenuation = 1.0 / max(light.atten.x + light.atten.y * d + light.atten.z * d * d, 0.001);
//spotlight (baterka)
        if (light.type == 2) {
            vec3 spotDir = normalize(-light.direction);
            float theta = dot(L, spotDir);
            float cutoffCos = cos(light.cutoff);

            if (theta < cutoffCos) return vec3(0.0);

            float intensity = smoothstep(cutoffCos, cutoffCos + 0.1, theta);
            attenuation *= intensity;
        }
    }

    float diff = max(dot(N, L), 0.0);

    float spec = 0.0;
    if (dot(N, L) >= 0.0) {
        vec3 R = reflect(-L, N);
        spec = pow(max(dot(R, V), 0.0), material.h);
    }

    vec3 ambient  = material.ra * light.color;
    vec3 diffuse  = material.rd * diff * light.color;
    vec3 specular = material.rs * spec * light.color ;
    return (ambient + diffuse + specular) * attenuation;
}


void main() {
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // global ambient
    vec3 result = vec3(0.1);

    for (int i = 0; i < nol; i++) {
        result += calculateLight(lights[i], N, V);
    }

    vec3 baseColor;
    if (useTexture) {
        baseColor = texture(diffuseTexture, TexCoord).rgb;
    } else {
        baseColor = objectColor;
    }

    FragColor = vec4(result * baseColor, 1.0);
}
