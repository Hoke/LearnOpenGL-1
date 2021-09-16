#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform float mixValue;
uniform float dir;
// 纹理采样器
uniform sampler2D ourTexture;
uniform sampler2D texture2;

void main()
{
	// 从两个纹理上采样,然后线性插值
	 FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, vec2(TexCoord.x * dir, TexCoord.y)), mixValue);
}
