layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 WorldPos;

#include "common.fs"

uniform mat4 model;

void main()
{
	vec4 aPos4 = vec4(aPos, 1.0);

	FragPos = vec3(model * aPos4);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Tangent = mat3(transpose(inverse(model))) * aTangent;
	WorldPos = (model * aPos4).xyz;

	vec4 tm = View * model * aPos4;

	gl_Position = Projection * tm;
	TexCoord = aTexCoord;
}
