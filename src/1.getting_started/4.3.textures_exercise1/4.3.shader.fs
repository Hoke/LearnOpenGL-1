#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// 纹理采样器
uniform sampler2D ourTexture;
uniform sampler2D texture2;

void main()
{
	// 从两个纹理上采样,然后线性插值
	// 修改片段着色器，仅让笑脸图案朝另一个方向看,
	// 反着采样就ok了
	 FragColor = mix(texture(ourTexture, TexCoord), texture(texture2,vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);
}
