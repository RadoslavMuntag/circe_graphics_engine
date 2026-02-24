#version 330 core

out vec4 FragColor;

in vec2 vUV;

uniform samplerCube skybox;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPos;

uniform vec3 blackHolePos;
uniform vec3 objectPos;

// ===== SDF =====
float sdfSphere(vec3 p, float r) {
    return length(p) - r;
}

float map(vec3 p) {
    float sphere = sdfSphere(p - vec3(0.0, 0.0, 0.0), 1.0);
    float ground = p.y + 1.0;
    return min(sphere, ground);
}

// ===== Normal from SDF =====
vec3 calcNormal(vec3 p) {
    float e = 0.001;
    vec2 h = vec2(e, 0.0);

    float dx = map(p + vec3(h.x, h.y, h.y)) - map(p - vec3(h.x, h.y, h.y));
    float dy = map(p + vec3(h.y, h.x, h.y)) - map(p - vec3(h.y, h.x, h.y));
    float dz = map(p + vec3(h.y, h.y, h.x)) - map(p - vec3(h.y, h.y, h.x));

    return normalize(vec3(dx, dy, dz));
}

// ===== Build Ray from Camera Matrices =====
vec3 getRayDirection(vec2 uv) {
    // Convert UV to NDC space (-1 to 1)
    vec2 ndc = uv * 2.0 - 1.0;

    // Clip space
    vec4 clip = vec4(ndc, -1.0, 1.0);

    // View space
    vec4 viewSpace = inverse(projection) * clip;
    viewSpace = vec4(viewSpace.xy, -1.0, 0.0);

    // World space
    vec3 worldDir = normalize((inverse(view) * viewSpace).xyz);
    return worldDir;
}

void main() {
    vec3 position = cameraPos;
    vec3 direction = getRayDirection(vUV);

    vec3 bhCenter = blackHolePos;
    float eventHorizon = 1.0;
    float bhMass = 0.6; // tweak this

    const int MAX_STEPS = 256;
    const float STEP_SIZE = 0.1;

    bool absorbed = false;
    
    vec3 color = vec3(0.0);

    for (int i = 0; i < MAX_STEPS; i++) {
        // Move ray forward
        position += direction * STEP_SIZE;

        // Vector to black hole center
        vec3 toBH = bhCenter - position;
        float r = length(toBH);

        // Event horizon check (ray gets eaten)
        if (sdfSphere(position - objectPos, 1.0) < 0.0) {
            // Add some glow near the black hole
            absorbed = true;
            color = vec3(1.0, 0.5, 0.0) * (1.0 - r / 5.0); // orange glow
            break;
        }
        if (r < eventHorizon) {
            absorbed = true;
            color = vec3(0.0); // pure black hole
            break;
        }

        // Gravitational bending (stable version)
        vec3 gravityDir = normalize(toBH);
        float force = bhMass / pow(r, 3.0); // softening term prevents explosion
        
        direction = normalize(direction + gravityDir * force * STEP_SIZE);
    }


    if (!absorbed) {
        // If not absorbed, use skybox color
        color = texture(skybox, direction).rgb;
    } 

    FragColor = vec4(color, 1.0);
}
