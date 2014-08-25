#pragma once
#include <Precompiled.h>
#include <ShaderUtility.h>

#define BUFFER_OFFSET(x)  (reinterpret_cast<const void *>(x))
class Application;

class ChapterBase
{
public:
    ChapterBase( Application *setAppPointer );
    ~ChapterBase( void );
    ChapterBase( const ChapterBase &other ) = delete;
    ChapterBase &operator= ( const ChapterBase &other ) = delete;

    virtual void Initialize( void ) = 0;
    virtual void Draw( void ) = 0;
    virtual void Step( double time_step ) = 0;
    virtual void Shutdown( void ) = 0;

protected:
    Application *appPointer;
};