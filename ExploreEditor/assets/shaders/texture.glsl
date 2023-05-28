#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoor;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;
uniform mat4 u_ProjectionViewMatrix;
out vec4 vColor;
out vec2 vTexCoor;
out float vTexIndex;
out float vTilingFactor;
void main()
{
	gl_Position = u_ProjectionViewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vColor = aColor;
	vTexCoor = aTexCoor;
	vTexIndex = aTexIndex;
	vTilingFactor = aTilingFactor;
};

#type fragment
#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 Color2;
in vec4 vColor;
in vec2 vTexCoor;
in float vTexIndex;
in float vTilingFactor;
//uniform sampler2D u_Texture;
//uniform vec4 u_color;
uniform sampler2D u_Textures[32];
void main()
{
	FragColor = texture(u_Textures[int(vTexIndex)],vTexCoor * vTilingFactor) * vColor;
	Color2 = vec4(1.0f,0.0f,0.0f,1.0f);
};