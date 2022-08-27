#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout(location = 1) in vec3 aColor; // the color variable has attribute position 1
layout(location = 2) in vec2 aTexCoord; // the texcoord variable has attribute position 1

uniform vec2 Input;
uniform vec2 offset;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;
out vec2 Offset;

void main()
{
    gl_Position = vec4(aPos.x + Input.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor; 
    TexCoord = aTexCoord;
    Offset = offset;
}



#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec2 Offset;

uniform sampler2D ourTexture;

void main()
{
    
    FragColor = texture(ourTexture, TexCoord + Offset);//  - vec4(ourColor, 1.0f);
}