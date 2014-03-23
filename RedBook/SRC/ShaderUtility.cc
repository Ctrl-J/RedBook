#include <Precompiled.h>
#include <ShaderUtility.h>

void ShaderUtility::DeleteShaders( std::vector<ShaderInfo> &input )
{
    for( auto current_shader = input.begin(); current_shader != input.end(); current_shader++ )
    {
        if( current_shader->shader_id != GL_NONE )
        {
            glDeleteShader( current_shader->shader_id );
            current_shader->shader_id = GL_NONE;
        }
    }
}

std::string ShaderUtility::ReadShader( const std::string &filename )
{
    std::ifstream file( filename, std::ios::in | std::ios::binary);
    if( file.is_open() == false )
    {
        MessageBox( NULL, L"Error reading shader file.", L"Initialization error", MB_OK | MB_ICONERROR );
        return nullptr;
    }

    std::string contents;

    file.seekg( 0, std::ios::end );
    contents.resize( file.tellg() );

    file.seekg( 0, std::ios::beg );
    file.read( &contents[0], contents.size() );

    return contents;
}

void ShaderUtility::OutputError( GLuint identifier, ErrorType error_code )
{
    std::ofstream file( "shader_error_log.txt" );
    GLsizei length;

    if( error_code == ErrorType::program_link )
    {
        glGetProgramiv( identifier, GL_INFO_LOG_LENGTH, &length );
    }
    else if( error_code == ErrorType::shader_compile )
    {
        glGetShaderiv( identifier, GL_INFO_LOG_LENGTH, &length );
    }

    std::vector<char> info_log;

    if( length <= 0 )
    {
        MessageBox( NULL, L"Info log reports zero size.", L"Info log error", MB_OK | MB_ICONERROR );
        return;
    }
    else
    {
        info_log.resize( length + 1, ' ' );
    }
    
    if( error_code == ErrorType::program_link )
    {
        glGetProgramInfoLog( identifier, length, &length, &info_log[0] );
    }
    else if( error_code == ErrorType::shader_compile )
    {
        glGetShaderInfoLog( identifier, GL_INFO_LOG_LENGTH, &length, &info_log[0] );
    }

    file << "Shader program reports error:\n\n";
   
    bool end_of_line = false;

    for( unsigned int position = 0; position < info_log.size(); position++ )
    {
        if( ( position % 80 ) == 0 )
        {
            end_of_line = true;
        }

        // break on non-word characters
        if( end_of_line == true )
        {
            if( false == ( ( ( info_log[position] >= 'a' ) && ( info_log[position] <= 'z' ) ) ||
                           ( ( info_log[position] >= 'A' ) && ( info_log[position] <= 'Z' ) ) ) )
            {
                file << std::endl;
                end_of_line = false;
            }
        }

        file << info_log[position];
    }

    file << std::endl;

    MessageBox( NULL, L"Check shader_error_log.txt", L"Shader program creation failed", MB_OK | MB_ICONERROR );
}

GLuint ShaderUtility::LoadShader( std::vector<ShaderInfo> &input )
{
    if( input.size() <= 0 )
    {
        return 0;
    }

    GLuint program = glCreateProgram();

    std::vector<ShaderInfo>::iterator current_shader = input.begin();
    while( current_shader->type != GL_NONE )
    {
        GLuint generated_shader_id = glCreateShader( current_shader->type );
        current_shader->shader_id = generated_shader_id;

        std::string source_contents = ShaderUtility::ReadShader( current_shader->filename );
        const GLchar *shader_source = source_contents.c_str();
        if( shader_source == nullptr )
        {
            // If we couldn't load one of the shaders, clean up and exit. 
            ShaderUtility::DeleteShaders( input );
            return 0;
        }

        glShaderSource( current_shader->shader_id, 1, &shader_source, NULL );
        glCompileShader( current_shader->shader_id );

        GLint compiled;
        glGetShaderiv( current_shader->shader_id, GL_COMPILE_STATUS, &compiled );
        if( compiled == GL_FALSE )
        {
            // We throw detailed shader messages into the debug build
            // Release version just errors and exits.
#ifdef _DEBUG
            ShaderUtility::OutputError( current_shader->shader_id, ErrorType::shader_compile );
#else
            MessageBox( NULL, L"Error compiling shader.", L"Initialization ERROR", MB_OK | MB_ICONERROR);
#endif
            ShaderUtility::DeleteShaders( input );
            return 0;
        }

        glAttachShader( program, current_shader->shader_id );
        current_shader++;
    }

    glLinkProgram( program );

    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if( linked == GL_FALSE )
    {
#ifdef _DEBUG
        ShaderUtility::OutputError( program, ErrorType::program_link );
#else
        MessageBox( NULL, L"Error linking shader", L"Initialization ERROR", MB_OK | MB_ICONERROR);
#endif
        ShaderUtility::DeleteShaders( input );
        return 0; 
    }
    
    return program;
}