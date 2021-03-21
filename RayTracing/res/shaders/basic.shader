#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    frag_pos = vec3(model * vec4(aPosition, 1.0));
//    normal = mat3(transpose(inverse(model))) * aNormal;
    normal = vec3(model * vec4(aNormal, 1.0));
//    normal = aNormal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
uniform vec3 light_pos;

in vec3 normal;
in vec3 frag_pos;

void main()
{
    float diff = max( dot(normalize(normal), (normalize(light_pos - frag_pos)) ) , 0.0);
//    color = u_color * diff;
    color = u_color;
};