in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 FragPos;
in vec3 WorldPos;

out vec4 fragColor;

layout(binding=0) uniform sampler2DArray albedoTexture;
layout(binding=1) uniform sampler2DArray normalTexture;
layout(binding=2) uniform sampler2DArray mixTexture;
layout(binding=3) uniform sampler2DArray opacityTexture;

uniform vec3 viewPos;
uniform int layer;
uniform mat4 model;

#include "common.fs"
#include "lighting.fs"

void main()
{
	fragColor = texture(albedoTexture, vec3(TexCoord + vec2(-TotalTime * 2.0, 0.0), 0));
}
