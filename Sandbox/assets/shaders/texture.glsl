#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoor;
uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_ModelMatrix;
out vec2 vTexCoor;
void main()
{
	gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vTexCoor = aTexCoor;
};

#type fragment
#version 330 core
out vec4 FragColor;
in vec2 vTexCoor;
uniform sampler2D u_Texture;
uniform vec4 u_color;
void main()
{
	FragColor = texture(u_Texture,vTexCoor) * u_color;
};