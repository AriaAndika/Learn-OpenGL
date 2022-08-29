#shader vertex
#version 330 core
layout(location = 0) in vec3 vPos;   // the position variable has attribute position 0
layout(location = 1) in vec3 vCol; // the color variable has attribute position 1
layout(location = 2) in vec2 vCoord; // the texcoord variable has attribute position 1

uniform vec2 uOffset;
uniform vec2 offset;

out vec3 outCol; // output a color to the fragment shader
out vec2 TexCoord;
out vec2 Offset;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPos, 1.0f);
    outCol = vCol; 
    TexCoord = vCoord;
    Offset = offset;
}



#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 outCol;
in vec2 TexCoord;
in vec2 Offset;

uniform sampler2D ourTexture;

void main()
{
    // FragColor = vec4(outCol,1.0f);
    FragColor = texture(ourTexture, TexCoord + Offset);//  - vec4(ourColor, 1.0f);
}