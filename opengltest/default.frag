#version 330 core

out vec4 fragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D tex0;
uniform sampler2D spec0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
	vec3 lightVec = lightPos - fragPos;

	float distance = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float intensity = 1.0f / (a * distance * distance + b * distance + 1.0f);

	float ambient = 0.20f;

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(finalNormal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, finalNormal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse + ambient) + texture(spec0, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
	float outerCone = 0.90f;
	float innerCone = 0.95f;
	
	float ambient = 0.20f;

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - fragPos);
	float diffuse = max(dot(finalNormal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, finalNormal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(spec0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directLight() {
	float ambient = 0.20f;

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.f));
	float diffuse = max(dot(finalNormal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, finalNormal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse + ambient) + texture(spec0, texCoord).r * specular) * lightColor;
}

void main(){
	fragColor = directLight();
}