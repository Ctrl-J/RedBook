#include <Precompiled.h>
#include <ChapterOne.h>

ChapterOne::ChapterOne( Application *appPointer ) : ChapterBase(appPointer), num_vertices( 6 )
{
    vertex_array_objects.resize( static_cast< int >( vertex_array_object_identifiers::num_arrays ) );
    buffer_objects.resize( static_cast< int >( buffer_object_identifiers::num_buffers ) );

}

ChapterOne::~ChapterOne( void )
{

}

void ChapterOne::Initialize( void )
{
    glGenVertexArrays( static_cast< int >( vertex_array_object_identifiers::num_arrays ), &vertex_array_objects[0] );
    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );

    std::vector<GLfloat> vertices;
    vertices.resize( 2 * num_vertices );

    vertices[0] = -0.90f;  vertices[1]  = -0.90f;
    vertices[2] =  0.85f;  vertices[3]  = -0.90f;
    vertices[4] = -0.90f;  vertices[5]  =  0.85f;
    vertices[6] =  0.90f;  vertices[7]  = -0.85f;
    vertices[8] =  0.90f;  vertices[9]  =  0.90f;
    vertices[10] = -0.85f; vertices[11] =  0.90f;

    glGenBuffers( static_cast< int >( buffer_object_identifiers::num_buffers ), &buffer_objects[0] );
    glBindBuffer( GL_ARRAY_BUFFER, buffer_objects[static_cast<int>( buffer_object_identifiers::array_buffer )] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices[0] ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );

    std::vector<ShaderInfo> shaders;
    ShaderInfo current_shader;

    current_shader.type = GL_VERTEX_SHADER;
    current_shader.filename = "ch1_triangle.glvs";
    current_shader.shader_id = GL_NONE;
    shaders.push_back( current_shader );
    
    current_shader.type = GL_FRAGMENT_SHADER;
    current_shader.filename = "ch1_triangle.glfs";
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

    glClearColor( 0.2f, 0.4f, 0.3f, 1.0f );
}

void ChapterOne::Draw( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindVertexArray( vertex_array_objects[static_cast< int >( vertex_array_object_identifiers::triangles )] );
    glDrawArrays( GL_TRIANGLES, 0, num_vertices );

    glFlush();
}

void ChapterOne::Step( double time_step )
{

}