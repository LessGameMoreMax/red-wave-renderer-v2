#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;
uniform sampler2D depthMap;

// SSR
uniform vec3 camPos;
uniform int traceCount;
uniform float traceDistance[16];
uniform float maxDistance;
uniform mat4 view;
uniform mat4 projection;
const float hitAlpha = 0.0f;
const float minDistance = 0.5f;

float GetDepthFromMap(int level, vec2 coord){
    vec2 texelSize = 1.0 / vec2(textureSize(depthMap, 0));
    if(level == 0){
        return texture(depthMap, coord).r;
    }else{
        --level;
        int interval = int(pow(2.0f, level));
        float depth = 10.0f;
        for(int x = -2 * (interval + 1); x <= 2 * (interval + 1); x += interval + 1)
        {
            for(int y = -2 * (interval + 1); y <= 2 * (interval + 1); y += interval + 1)
            {
                float t = texture(depthMap, coord + vec2(x, y) * texelSize).r;
                depth = min(depth, t);
            }    
        }
        return depth;
    }
}

bool IsOut(vec2 coord){
    if(coord.x < 0.0f || coord.y < 0.0f ||
        coord.x > 1.0f || coord.y > 1.0f)
        return true;
    return false;
}

void main()
{             
    vec3 color = texture(colorMap, TexCoords).rgb;
    float currentDepth = texture(depthMap, TexCoords).r;
    if(currentDepth > 0.999f){
        FragColor = vec4(color, 1.0f);
        return;
    }

    vec3 worldPos = texture(positionMap, TexCoords).rgb;
    vec3 V = normalize(camPos - worldPos);
    vec3 N = normalize(texture(normalMap, TexCoords).xyz);
    vec3 R = normalize(reflect(-V, N));
    int level = 0;
    bool hit = false;
    float distance = 0.0f;
    while(distance < maxDistance){
        if(level == -1){
            hit = true;
            break;
        }
        float newDis = distance + traceDistance[level];
        vec3 newPos = worldPos + R * newDis;
        vec4 ndcPos = projection * view * vec4(newPos, 1.0f);
        vec3 projCoords = ndcPos.xyz / ndcPos.w;
        projCoords = projCoords * 0.5 + 0.5;
        if(IsOut(projCoords.xy)) break;
        if(projCoords.z > GetDepthFromMap(level, projCoords.xy)){
            --level;
        }else{
            distance += traceDistance[level];
            level = min(level + 1, traceCount-1);
        }
    }
    if(!hit || distance < minDistance){
        FragColor = vec4(color, 1.0f);
        return;
    }
    
    vec3 hitPos = worldPos + R * distance;
    vec4 hitNdcPos = projection * view * vec4(hitPos, 1.0f);
    vec3 hitProjCoords = hitNdcPos.xyz / hitNdcPos.w;
    hitProjCoords = hitProjCoords * 0.5 + 0.5;
    vec3 hitColor = texture(colorMap, hitProjCoords.xy).rgb;

    float alpha = clamp(1.0f - distance / maxDistance + hitAlpha, 0.0f, 1.0f);

    FragColor = vec4(mix(hitColor, color, alpha), 1.0f);
}
