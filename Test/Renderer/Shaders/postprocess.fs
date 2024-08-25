#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;
uniform sampler2D depthMap;

void main()
{             
    vec3 v = texture(depthMap, TexCoords).rgb;
    FragColor = vec4(v, 1.0f);
}
