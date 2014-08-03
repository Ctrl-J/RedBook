#include <Precompiled.h>
#include <ChapterTwo.h>

ChapterTwo::ChapterTwo( void ) : num_vertices( 6 )
{
    vertex_array_objects.resize( static_cast< int >( vertex_array_object_identifiers::num_arrays ) );
    buffer_objects.resize( static_cast< int >( buffer_object_identifiers::num_buffers ) );

}

ChapterTwo::~ChapterTwo( void )
{

}

void ChapterTwo::Initialize( void )
{
    glGenVertexArrays( static_cast< int >( vertex_array_object_identifiers::num_arrays ), &vertex_array_objects[0] );
    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );

    std::vector<GLfloat> vertices;
    vertices.resize( 2 * num_vertices );

    vertices[0]  = -0.90f;  vertices[1]  = -0.90f;
    vertices[2]  =  0.85f;  vertices[3]  = -0.90f;
    vertices[4]  = -0.90f;  vertices[5]  =  0.85f;
    vertices[6]  =  0.90f;  vertices[7]  = -0.85f;
    vertices[8]  =  0.90f;  vertices[9]  =  0.90f;
    vertices[10] = -0.85f;  vertices[11] =  0.90f;

    glGenBuffers( static_cast< int >( buffer_object_identifiers::num_buffers ), &buffer_objects[0] );
    glBindBuffer( GL_ARRAY_BUFFER, buffer_objects[static_cast< int >( buffer_object_identifiers::array_buffer )] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices[0] ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );

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

    GLuint program = ShaderUtility::LoadShader( shaders );
    glUseProgram( program );
    glVertexAttribPointer( static_cast< int >( attribute_object_identifiers::vertex_position ), 2, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET( 0 ) );
    glEnableVertexAttribArray( static_cast< int >( attribute_object_identifiers::vertex_position ) );

    GLuint  uniform_block_index     = -1;
    GLint   uniform_block_size      = -1;
    GLuint  uniform_block           = -1;
    GLvoid *uniform_block_buffer    = nullptr;

    uniform_block_index = glGetUniformBlockIndex( program, "Uniforms" );
    
    glGetActiveUniformBlockiv( program, uniform_block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &uniform_block_size );

    //uniform_block_buffer = 
    if( uniform_block_buffer == nullptr )
    {
        MessageBox( NULL, L"Error allocating uniform block buffer", L"Error", MB_OK );
        exit( EXIT_FAILURE );
    }

    GLfloat scale = 0.5f;
    GLfloat translation[] = { 0.1f, 0.1f, 0.0f };
    GLfloat rotation[] = { 90.0f, 0.0f, 0.0f, 1.0f };
    GLboolean enabled = GL_TRUE;
   
    const char *names[static_cast< int >( uniform_block_identifiers::num_uniforms )] =
    {
        "translation",
        "scale",
        "rotation"
        "enabled"
    };

    GLuint  indices[static_cast< int >( uniform_block_identifiers::num_uniforms )];
    GLint   size[static_cast< int >( uniform_block_identifiers::num_uniforms )];
    GLint 	offset[static_cast< int >( uniform_block_identifiers::num_uniforms )];
    GLint 	type[static_cast< int >( uniform_block_identifiers::num_uniforms )];

    glGetUniformIndices( program, static_cast< int >( uniform_block_identifiers::num_uniforms ), names, indices );
    glGetActiveUniformsiv( program, static_cast< int >( uniform_block_identifiers::num_uniforms ), indices, GL_UNIFORM_OFFSET, offset );
    glGetActiveUniformsiv( program, static_cast< int >( uniform_block_identifiers::num_uniforms ), indices, GL_UNIFORM_SIZE, size );
    glGetActiveUniformsiv( program, static_cast< int >( uniform_block_identifiers::num_uniforms ), indices, GL_UNIFORM_TYPE, type );

    glGenBuffers( 1, &uniform_block );
    glBindBuffer( GL_UNIFORM_BUFFER, uniform_block );
    glBufferData( GL_UNIFORM_BUFFER, uniform_block_size, uniform_block_buffer, GL_STATIC_DRAW );
    glBindBufferBase( GL_UNIFORM_BUFFER, uniform_block_index, uniform_block);


    glClearColor( 0.2f, 0.4f, 0.3f, 1.0f );
}

void ChapterTwo::Draw( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );
    glDrawArrays( GL_TRIANGLES, 0, num_vertices );

    glFlush();
}

void ChapterTwo::Step( double time_step )
{

}