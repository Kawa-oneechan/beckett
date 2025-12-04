	vec3 norm = Toon ? normalize(Normal) : calcNormal(normalVal);
	vec3 viewPos = ((InvView - model) * vec4(0.0, 0.0, 0.0, 0.1)).xyz;
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	result = directLight(norm, albedoVal.rgb, specularVal, viewDir);
	for (int i = 1; i < NUMLIGHTS; i++)
		result += getLight(Lights[i], albedoVal.rgb, norm, viewDir, specularVal);

	if (fresnelVal < 1.0)
		result += vec3(fresnelVal * getFresnel(model, norm));

	if (opacityVal < OPACITY_CUTOFF) discard;

	fragColor = vec4(result, opacityVal);
}
