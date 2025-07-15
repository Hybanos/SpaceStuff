#include "render/shader.hpp"

Shader::Shader(const char *_vs, const char *_fs) : vs{_vs}, fs{_fs} {
    compile();
    if (program_id != 0) {
        get_layout();
    }
}

void Shader::compile() {
    program_id = glCreateProgram();

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vs, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        std::cout << "Unable to load shader " << vs << std::endl;
        getchar();
        program_id = 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fs, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    std::cout << "Compiling shader " << vs << std::endl;
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    std::cout << "Compiling shader " << fs << std::endl;
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    // Link the program
    printf("Linking program\n");
    glAttachShader(program_id, VertexShaderID);
    glAttachShader(program_id, FragmentShaderID);
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(program_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    glDetachShader(program_id, VertexShaderID);
    glDetachShader(program_id, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

void Shader::get_layout() {
    std::ifstream vs_stream(vs, std::ios::in);

   std::regex layout_line_re(R"(layout\s*\(\s*location\s*=\s*(\d+)\s*\)\s*in\s+(\S+)\s+.+)"); 

    std::string line;
    std::smatch matches;
    int cpt = 0;
    while (std::getline(vs_stream, line, ';')) {
        if (std::regex_search(line, matches, layout_line_re)) {
            std::cout << matches[1] << " " << matches[2] << std::endl;
            int loc = std::stoi(matches[1]);
            layout_types.push_back(matches[2]);
            buffer_to_layout.push_back(loc);
            layout_to_buffer.resize(loc + 1);
            layout_to_buffer[loc] = cpt; 

            cpt++;
        }
    }

    vs_stream.close();
}

void Shader::use() {
    glUseProgram(program_id);
}

int Shader::buff_count() {
    return buffer_to_layout.size();
}

int Shader::buff_to_loc(int buff) {
    return buffer_to_layout[buff];
}

int Shader::loc_to_buff(int loc) {
    return layout_to_buffer[loc];
}

int Shader::loc_to_type(int loc) {
    return glsl_types[layout_types[loc_to_buff(loc)]];
}

int Shader::loc_to_type_size(int loc) {
    return glsl_types_size[layout_types[loc_to_buff(loc)]];
}