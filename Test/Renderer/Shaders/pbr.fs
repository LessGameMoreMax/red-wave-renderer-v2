#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

// material parameters
uniform sampler2D positionMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metalRoughAoMap;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// Shadow
uniform mat4 view;
uniform vec3 lightDir;
uniform sampler2DArray shadowMap;
uniform float farPlane;
uniform int cascadeCount;
layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];

// lights
struct PointLight {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};

struct DirLight {
    vec3 Position;
    vec3 Color;
};
// const int PointLightNum = 4;
// uniform PointLight pointLights[PointLightNum];

const int DirLightNum = 1;
uniform DirLight dirLights[DirLightNum];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------
vec3 boxCubeMapLookup(vec3 rayOrigin, vec3 rayDir, vec3 boxExtent){
    vec3 t1 = (-rayOrigin + boxExtent) / rayDir;
    vec3 t2 = (-rayOrigin - boxExtent) / rayDir;
    vec3 tmax = max(t1, t2);

    float t = min(min(tmax.x, tmax.y), tmax.z);
    return normalize(rayOrigin + t * rayDir);
}

float ShadowCalculation(vec3 fragPosWorldSpace, vec3 normal)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    fragPosLightSpace.z = -fragPosLightSpace.z;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    // calculate bias (based on depth map resolution and slope)
    vec3 N = normalize(normal);
    float bias = max(0.05f * (1.0 - dot(N, lightDir)), 0.005f);
    const float biasModifier = 2.0f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCSS
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    float blockDepth = 0.0f;
    int blockCount = 0;
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcssDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            if((currentDepth - bias) > pcssDepth){
                blockCount += 1;
                blockDepth += pcssDepth;
            }
        }
    }
    if(blockCount == 0) return 0.0;
    blockDepth /= float(blockCount);

    vec3 lightWorldPosition = lightDir * 20.0f;
    vec3 depthProjCoords = vec3(projCoords.x, projCoords.y, blockDepth);
    depthProjCoords = depthProjCoords * 2.0f - 1.0f;
    depthProjCoords.z = -depthProjCoords.z;
    vec4 depthWorldPosition = inverse(lightSpaceMatrices[layer]) * vec4(depthProjCoords, 1.0f);
    depthWorldPosition /= depthWorldPosition.w;
    const float minCount = 2.0f;
    const float maxCount = 500.0f;
    float alpha = distance(depthWorldPosition.xyz, fragPosWorldSpace)/distance(lightWorldPosition, fragPosWorldSpace);
    int count = int(mix(minCount, maxCount, alpha));

    float shadow = 0.0;
    int ic = -1;
    if(count == 2){
        for(int x = -count; x <= count; ++x)
        {
            for(int y = -count; y <= count; ++y)
            {
                float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
                shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        ic = 0;
    }else{
        for(int interval = 0; (interval + 1)*2 <= count; interval = int(pow(2.0f, ic))){
            ic += 1;
            for(int x = -2 * (interval + 1); x <= 2 * (interval + 1); x += interval + 1)
            {
                for(int y = -2 * (interval + 1); y <= 2 * (interval + 1); y += interval + 1)
                {
                    float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
                    shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
                }    
            }
        }
    }

    shadow /= float((ic + 1) * 25);
    return shadow;
}

void main()
{		
    vec3 WorldPos = texture(positionMap, TexCoords).rgb;
    // material properties
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));

    float ao = texture(metalRoughAoMap, TexCoords).r;
    float roughness = texture(metalRoughAoMap, TexCoords).g;
    float metallic = texture(metalRoughAoMap, TexCoords).b;
    
    // input lighting data
    vec3 N = texture(normalMap, TexCoords).rgb;
    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // for(int i = 0; i < PointLightNum; ++i) 
    // {
    //     // calculate per-light radiance
    //     vec3 L = normalize(pointLights[i].Position - WorldPos);
    //     vec3 H = normalize(V + L);
    //     float distance = length(pointLights[i].Position - WorldPos);
    //     if(distance < pointLights[i].Radius){
    //         float attenuation = 1.0 / (1.0 + pointLights[i].Linear * distance + pointLights[i].Quadratic * distance * distance);
    //         vec3 radiance = pointLights[i].Color * attenuation;
    //
    //         // Cook-Torrance BRDF
    //         float NDF = DistributionGGX(N, H, roughness);   
    //         float G   = GeometrySmith(N, V, L, roughness);    
    //         vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
    //
    //         vec3 numerator    = NDF * G * F;
    //         float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    //         vec3 specular = numerator / denominator;
    //
    //          // kS is equal to Fresnel
    //         vec3 kS = F;
    //         // for energy conservation, the diffuse and specular light can't
    //         // be above 1.0 (unless the surface emits light); to preserve this
    //         // relationship the diffuse component (kD) should equal 1.0 - kS.
    //         vec3 kD = vec3(1.0) - kS;
    //         // multiply kD by the inverse metalness such that only non-metals 
    //         // have diffuse lighting, or a linear blend if partly metal (pure metals
    //         // have no diffuse light).
    //         kD *= 1.0 - metallic;	                
    //
    //         // scale light by NdotL
    //         float NdotL = max(dot(N, L), 0.0);        
    //
    //         // add to outgoing radiance Lo
    //         Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    //     }
    // }   

    for(int i = 0; i < DirLightNum; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(dirLights[i].Position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(dirLights[i].Position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = dirLights[i].Color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    float extent = 1000000.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, boxCubeMapLookup(WorldPos, R, vec3(extent)),  roughness * MAX_REFLECTION_LOD).rgb;    
    // vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    float shadow = 1.0f - ShadowCalculation(WorldPos, N);
    vec3 color = Lo * shadow + ambient * min(shadow + 0.005f, 1.0f);

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color , 1.0);
}
