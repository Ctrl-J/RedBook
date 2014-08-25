#include <Precompiled.h>
#include <ChapterTwo.h>
#include <Application.h>

ChapterTwo::ChapterTwo( Application *appPointer ) : ChapterBase(appPointer), num_vertices( 6 )
{
    vertex_array_objects.resize( static_cast< int >( vertex_array_object_identifiers::num_arrays ) );
    buffer_objects.resize( static_cast< int >( buffer_object_identifiers::num_buffers ) );
}

ChapterTwo::~ChapterTwo( void )
{
}

void ChapterTwo::Initialize( void )
{
    commandDelay = 10.0;
    commandTimer = 0.0;
    clearColor = glm::vec3( 0.25, 0.45, 0.5 );
    clearColorSpeed = glm::vec3( 0.2, 0.05, 0.1 );
    scaleSpeed = 0.1;
    GLenum error = GL_NO_ERROR;
    glGenVertexArrays( static_cast< int >( vertex_array_object_identifiers::num_arrays ), &vertex_array_objects[0] );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"GenVertexArrays"); 

    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"BindVertexArray"); 

    std::vector<GLfloat> vertices;
    vertices.resize( 2 * num_vertices );

    vertices[0]  = -0.90f;  vertices[1]  = -0.90f;
    vertices[2]  =  0.85f;  vertices[3]  = -0.90f;
    vertices[4]  = -0.90f;  vertices[5]  =  0.85f;
    vertices[6]  =  0.90f;  vertices[7]  = -0.85f;
    vertices[8]  =  0.90f;  vertices[9]  =  0.90f;
    vertices[10] = -0.85f;  vertices[11] =  0.90f;

    glGenBuffers( static_cast< int >( buffer_object_identifiers::num_buffers ), &buffer_objects[0] );
    error = glGetError();
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"GenBuffers"); 

    glBindBuffer( GL_ARRAY_BUFFER, buffer_objects[static_cast< int >( buffer_object_identifiers::array_buffer )] );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"BindBuffer"); 

    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices[0] ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"BufferData"); 

    std::vector<ShaderInfo> shaders;
    ShaderInfo current_shader;

    current_shader.type = GL_VERTEX_SHADER;
    current_shader.filename = "ch2_uniform_block.glvs";
    current_shader.shader_id = GL_NONE;
    shaders.push_back( current_shader );

    current_shader.type = GL_FRAGMENT_SHADER;
    current_shader.filename = "ch2_uniform_block.glfs";
    current_shader.shader_id = GL_NONE;
    shaders.push_back( current_shader );

    current_shader.type = GL_NONE;
    current_shader.filename = "";
    current_shader.shader_id = GL_NONE;
    shaders.push_back( current_shader );

    shaderProgram = ShaderUtility::LoadShader( shaders );
    glUseProgram( shaderProgram );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"UseProgram"); 
    
    glVertexAttribPointer( static_cast< int >( attribute_object_identifiers::vertex_position ), 2, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET( 0 ) );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"VertexAttribPointer"); 

    glEnableVertexAttribArray( static_cast< int >( attribute_object_identifiers::vertex_position ) );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"EnableVertexAttribArray"); 

    setupUniformBlock();

    glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.0f );

    blockAttributes = std::make_shared<UniformBlockStructure>();
    blockAttributes->scale = 1.0;
    blockAttributes->rotation = glm::vec4( 0.0, 0.0, 0.0, 1.0 );
    blockAttributes->translation = glm::vec3( 0.0, 0.0, 0.0 );
    blockAttributes->enabled = true;
}

void ChapterTwo::Draw( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

    updateUniformBlock( blockAttributes->translation, blockAttributes->scale, blockAttributes->rotation, blockAttributes->enabled );
    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );
    glDrawArrays( GL_TRIANGLES, 0, num_vertices );

    glFlush();
}

void ChapterTwo::Step( double time_step )
{
    commandTimer += time_step;

    if( clearColor.r > 1.0f )
    {
        clearColor.r = 1.0f;
        clearColorSpeed.r = -clearColorSpeed.r;
    }
    else if(clearColor.r <= 0.0f )
    {
        clearColor.r = 0.0f;
        clearColorSpeed.r = -clearColorSpeed.r;
    }
    if( clearColor.g > 1.0f )
    {
        clearColor.g = 1.0f;
        clearColorSpeed.g = -clearColorSpeed.g;
    }
    else if(clearColor.g <= 0.0f )
    {
        clearColor.g = 0.0f;
        clearColorSpeed.g = -clearColorSpeed.g;
    }
    if( clearColor.b > 1.0f )
    {
        clearColor.b = 1.0f;
        clearColorSpeed.b = -clearColorSpeed.r;
    }
    else if(clearColor.b <= 0.0f )
    {
        clearColor.b = 0.0f;
        clearColorSpeed.b = -clearColorSpeed.r;
    }

    clearColor.r += clearColorSpeed.r * static_cast< float >( time_step );
    clearColor.g += clearColorSpeed.g * static_cast< float >( time_step );
    clearColor.b += clearColorSpeed.b * static_cast< float >( time_step );


    if( blockAttributes != nullptr )
    {
        if( blockAttributes->scale >= 1.0 )
        {
            scaleSpeed = -scaleSpeed;
            blockAttributes->scale = 1.0;
        }
        else if( blockAttributes->scale <= 0.0 )
        {
            scaleSpeed = -scaleSpeed;
            blockAttributes->scale = 0.0;
        }
        blockAttributes->scale += static_cast< GLfloat >( scaleSpeed * time_step );

        if( appPointer->IsKeyPressed( 'P' ) )
        {
            blockAttributes->enabled = ( !blockAttributes->enabled );
            commandTimer = 0.0;
        }
        
        if( appPointer->IsKeyDown( VK_LEFT ) )
        {
            blockAttributes->translation.x -= 0.01f;
        }
        
        if( appPointer->IsKeyDown( VK_RIGHT ) )
        {
            blockAttributes->translation.x += 0.01f;
        }
        
        if( appPointer->IsKeyDown( VK_UP ) )
        {
            blockAttributes->translation.y += 0.01f;
        }
        
        if( appPointer->IsKeyDown( VK_DOWN ) )
        {
            blockAttributes->translation.y -= 0.01f;
        }

        if( blockAttributes->translation.x > 1.0f )
        {
            blockAttributes->translation.x = 1.0f;
        }
        else if( blockAttributes->translation.x < -1.0f )
        {
            blockAttributes->translation.x = -1.0f;
        }
        
        if( blockAttributes->translation.y > 1.0f )
        {
            blockAttributes->translation.y = 1.0f;
        }
        else if( blockAttributes->translation.y < -1.0f )
        {
            blockAttributes->translation.y = -1.0f;
        }
    }

}

void ChapterTwo::Shutdown( void )
{
}

void ChapterTwo::setupUniformBlock( void )
{
    uniformBlockIndex = glGetUniformBlockIndex( shaderProgram, "Uniforms" );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetUniformBlockIndex -> uniformBlockIndex"); 

    glGenBuffers( 1, &uniformBlockObject );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGenBuffers - uniformBlock"); 
    
    glBindBuffer( GL_UNIFORM_BUFFER, uniformBlockObject );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glBindBuffer - uniformBlock"); 

    glGetProgramIV( shaderProgram, GL_ACTIVE_UNIFORMS, &numberOfActiveUniforms );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetProgramIV (GL_ACTIVE_UNIFORMS) Failed" );

    const char *uniformNames[static_cast< int >( uniform_identifiers::num_uniforms )] = { "rotation", "translation", "scale", "enabled" };

    glGetUniformIndices( shaderProgram, static_cast<int>(uniform_identifiers::num_uniforms), uniformNames, uniformIndices );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetUniformIndices Failed" );

    glGetActiveUniformBlockIV( shaderProgram, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetActiveUniformBlockIV (GL_UNIFORM_BLOCK_DATA_SIZE) Failed"); 
    
    glGetActiveUniformsIV( shaderProgram, static_cast<int>(uniform_identifiers::num_uniforms), uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetActiveUniformsIV (GL_UNIFORM_OFFSET) Failed"); 

    glGetActiveUniformsIV( shaderProgram, static_cast<int>(uniform_identifiers::num_uniforms), uniformIndices, GL_UNIFORM_SIZE, uniformSizes );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetActiveUniformsIV (GL_UNIFORM_SIZE) Failed"); 
    glGetActiveUniformsIV( shaderProgram, static_cast<int>(uniform_identifiers::num_uniforms), uniformIndices, GL_UNIFORM_TYPE, uniformTypes );
    GLUtility::Instance()->LogGLMessage( GLUtility::LogLevel::LOG_ERROR, L"glGetActiveUniformsIV (GL_UNIFORM_TYPE) Failed"); 
    
    bufferData.resize( uniformBlockSize / sizeof(GLfloat), 0 );
    
    glBufferData( GL_UNIFORM_BUFFER, uniformBlockSize, &bufferData[0], GL_DYNAMIC_DRAW );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}
    
void ChapterTwo::updateUniformBlock( const glm::vec3 &translation, const GLfloat &scale, 
                                     const glm::vec4 &rotation, const bool &enabled )
{
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::rotation )] / sizeof(GLfloat) + 0] = rotation.r;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::rotation )] / sizeof(GLfloat) + 1] = rotation.g;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::rotation )] / sizeof(GLfloat) + 2] = rotation.b;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::rotation )] / sizeof(GLfloat) + 3] = rotation.a;
    
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::translation )] / sizeof(GLfloat) + 0] = translation.r;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::translation )] / sizeof(GLfloat) + 1] = translation.g;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::translation )] / sizeof(GLfloat) + 2] = translation.b;
    
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::scale )] / sizeof(GLfloat)] = scale;
    bufferData[uniformOffsets[static_cast< int >( uniform_identifiers::enabled )] / sizeof(GLfloat)] = enabled;

    glBindBuffer( GL_UNIFORM_BUFFER, uniformBlockObject );
    glBufferData( GL_UNIFORM_BUFFER, uniformBlockSize, &bufferData[0], GL_STATIC_DRAW );
    glBindBufferBase( GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBlockObject);
}