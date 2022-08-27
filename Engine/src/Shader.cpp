#include <Shader.h>

Shader::Shader(const std::string filePath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream stream(filePath);

    bool isVertex = true;
    // ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];

    // go line by line
    while (getline(stream, line))
    {
        // if find #shader
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                isVertex = false;
            }
            else if (line.find("fragment") != std::string::npos) {
                isVertex = true;
            }
        }
        // other line
        else {
            ss[isVertex] << line << '\n';
        }
    }
    vertexCode = ss[0].str();
    fragmentCode = ss[1].str();
    const char* vShaderSource = vertexCode.c_str();
    const char* fShaderSource = fragmentCode.c_str();

    // std::cout << vShaderSource << std::endl;
    // std::cout << fShaderSource << std::endl;

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "Vertex Shader Compilation failed\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compilation failed\n" << infoLog << std::endl;
    };



    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Shader program linking failed\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// set float
void Shader::setFloat(const std::string& name, float value, float value2) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value, value2);
}
void Shader::setFloat(const std::string& name, float value, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value, value2, value3);
}
