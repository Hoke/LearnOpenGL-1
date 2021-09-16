#version 330 core

// 输入的顶点，定点颜色，纹理坐标
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;



out vec3 ourColor;
out vec2 TexCoord;

void main ()
{
	gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;

    TexCoord = aTexCoord;
}