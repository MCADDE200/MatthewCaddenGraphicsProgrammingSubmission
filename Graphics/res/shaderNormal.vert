#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transform; 

void main()
{
    gl_Position = transform  * vec4(VertexPosition, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * VertexNormal, 0.0)));
}