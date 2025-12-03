#version 330 core
#define MAX_LIGHTS 30

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
uniform vec3 objectColor;
uniform vec3 viewPos;
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
        L = normalize(light.position - FragPos);
        float d = length(light.position - FragPos);
        attenuation = 1.0 / max(light.atten.x + light.atten.y * d + light.atten.z * d * d, 0.001);

        // spotlight
        if (light.type == 2) {
            vec3 spotDir = normalize(-light.direction);
            float theta = dot(L, spotDir);
            float cutoffCos = cos(light.cutoff);

            if (theta < cutoffCos) return vec3(0.0);

            float intensity = smoothstep(cutoffCos, cutoffCos + 0.1, theta);
            attenuation *= intensity;
        }
    }

    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = 0.0;

    if (diff > 0.0) {
        spec = pow(max(dot(N, H), 0.0), 32.0);
    }

    vec3 ambient = 0.15 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;

    return (ambient + diffuse + specular) * attenuation;
}


void main() {
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

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
