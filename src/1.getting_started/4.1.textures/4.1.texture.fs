#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// 纹理采样器
uniform sampler2D ourTexture;

void main()
{
	// 从纹理上采颜色
	// FragColor = texture(ourTexture, TexCoord);
	// FragColor = vec4(ourColor, 1.0);
	//  再乘上顶点颜色
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
