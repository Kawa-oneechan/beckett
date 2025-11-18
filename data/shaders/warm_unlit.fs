in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 FragPos;
in vec3 WorldPos;

out vec4 fragColor;

uniform mat4 model;

#include "common.fs"

void main()
{
	fragColor = vec4(1.0, 0.89, 0.69, 1.0);
}
