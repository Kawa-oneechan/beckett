#ifndef PULLEDPORK
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
#endif

out vec2 TexCoords;
flat out int index;

//Match this to SpriteRenderer.h
const int BatchSize = 200;

uniform mat4 model[BatchSize];
uniform mat4 projection;

void main()
{
	index = gl_InstanceID;
#ifndef PULLEDPORK
	TexCoords = vertex.zw;
	gl_Position = projection * model[gl_InstanceID] * vec4(vertex.xy, 0.0, 1.0);
#else
	vec2 vx = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1.0;
	TexCoords = (vx + 1.0) * 0.5;
	gl_Position = projection * model[gl_InstanceID] * vec4(vx, 0.0, 1.0);
#endif
	gl_Position.z = gl_Position.w;
}
