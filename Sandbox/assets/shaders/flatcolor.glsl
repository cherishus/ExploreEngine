#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_ModelMatrix;
void main()
{
	gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
};

#type fragment
#version 330 core
out vec4 FragColor;
uniform vec4 u_color;
void main()
{
	FragColor = u_color;
};