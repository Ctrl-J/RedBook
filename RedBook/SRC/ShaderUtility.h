#pragma once
#include <Precompiled.h>

struct ShaderInfo
{
    GLenum type;
    std::string filename;
    GLuint shader_id;
};

//
// This class is designed to handle the reading and compiling of shaders.
// It doesn't handle linking, or shader managemeng. Simply reading shader code
// input, compiling the source, and deleting shaders.
//
class ShaderUtility
{
public:

    // Loads in a vector of shaders.
    static GLuint LoadShader( std::vector<ShaderInfo> &input );

private:
    // Reads in a shader file to a string. (Probably should be expanded to a separate 
    // utility class to generically read a file to a std::string)
    static std::string ReadShader( const std::string &filename );

    // Deletes a vector of shaders that were created using ShaderUtility::LoadShader
    static void DeleteShaders( std::vector<ShaderInfo> &input );

    // Outputs detailed shader error information
    static void OutputError( GLuint program_id );
};

