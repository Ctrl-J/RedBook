#pragma once
#include <Precompiled.h>
#include <ShaderUtility.h>

#define BUFFER_OFFSET(x)  (reinterpret_cast<const void *>(x))

class ChapterBase
{
public:
    ChapterBase( void );
    ~ChapterBase( void );
    ChapterBase( const ChapterBase &other ) = delete;
    ChapterBase &operator= ( const ChapterBase &other ) = delete;

    virtual void Initialize( void ) = 0;
    virtual void Draw( void ) = 0;
    virtual void Step( double time_step ) = 0;
};