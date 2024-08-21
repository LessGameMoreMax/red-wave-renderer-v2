#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gAlbedo;
layout (location = 2) out vec3 gNormal;
layout (location = 3) out vec3 gMetalRoughAo;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform float normalMapAlpha;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform float aoMapAlpha;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = normalize(cross(T, N));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{    
    gPosition = FragPos;
    gAlbedo = texture(albedoMap, TexCoords).rgb;
    gNormal = normalize(mix(Normal, getNormalFromMap(), normalMapAlpha).rgb);
    gMetalRoughAo.r = mix(0.0, texture(aoMap, TexCoords).r, aoMapAlpha);
    gMetalRoughAo.g = texture(roughnessMap, TexCoords).g;
    gMetalRoughAo.b = texture(metallicMap, TexCoords).b;
}
