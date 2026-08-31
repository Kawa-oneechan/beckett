	vec3 norm = Toon ? normalize(Normal) : calcNormal(normalVal);
	vec3 viewPos = ((InvView - model) * vec4(0.0, 0.0, 0.0, 0.1)).xyz;
	vec3 viewDir = normalize(viewPos - FragPos);

	if (DebugMode == 3)
	{
		albedoVal.rgb = vec3(0.5);
	}

	vec3 result;
	result = directLight(norm, albedoVal.rgb, specularVal, viewDir);
	for (int i = 1; i < NUMLIGHTS; i++)
		result += getLight(Lights[i], albedoVal.rgb, norm, viewDir, specularVal);

	if (DebugMode == 3)
	{
		fragColor = vec4(result, 1.0);
		return;
	}
	else if (DebugMode == 4)
	{
		fragColor = vec4(vec3(getFresnel(model, norm)), 1.0);
		return;
	}

	if (fresnelVal < 1.0)
		result += vec3(fresnelVal * getFresnel(model, norm));

	if (opacityVal < OPACITY_CUTOFF) discard;

	fragColor = vec4(result, opacityVal);
}
