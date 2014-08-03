#pragma once
#include <Precompiled.h>
#include <ChapterBase.h>

class ChapterTwo : public ChapterBase
{
public:

    ChapterTwo( void );
    ~ChapterTwo( void );
    ChapterTwo( const ChapterTwo &other ) = delete;
    ChapterTwo &operator= ( const ChapterTwo &other ) = delete;

    void Initialize( void );
    void Draw( void );
    void Step( double time_step );

private:
    enum class vertex_array_object_identifiers
    {
        triangles = 0,
        num_arrays
    };

    enum class buffer_object_identifiers
    {
        array_buffer = 0,
        num_buffers
    };

    enum class attribute_object_identifiers
    {
        vertex_position = 0,
        num_attributes
    };

    std::vector<GLuint> vertex_array_objects;
    std::vector<GLuint> buffer_objects;

    enum class uniform_block_identifiers
    {
        translation = 0,
        scale,
        rotation,
        enabled,
        num_uniforms
    };

    const GLuint num_vertices;
};