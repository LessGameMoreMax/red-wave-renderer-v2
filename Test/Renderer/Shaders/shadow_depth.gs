#version 430 core

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		vec4 position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        position.z = -position.z;
        gl_Position = position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}  
