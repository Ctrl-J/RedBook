#pragma once
#include <Precompiled.h>
#include <ChapterBase.h>

class ChapterOne : public ChapterBase
{
public:

    ChapterOne( Application *setAppPointer );
    ~ChapterOne( void );
    ChapterOne( const ChapterOne &other ) = delete;
    ChapterOne &operator= ( const ChapterOne &other ) = delete;

    void Initialize( void );
    void Draw( void );
    void Step( double time_step );

private:
    enum class vertex_array_object_identifiers
    {
        triangles,
        num_arrays
    };

    enum class buffer_object_identifiers
    {
        array_buffer,
        num_buffers
    };

    enum class attribute_object_identifiers
    {
        vertex_position = 0,
        num_attributes
    };

    std::vector<GLuint> vertex_array_objects;
    std::vector<GLuint> buffer_objects;
    
    const GLuint num_vertices;
};