#pragma once
#include <Precompiled.h>

class GLUtility
{
public:
    static GLUtility &Instance(const std::wstring &setLogFilename = L"");
    ~GLUtility();

    enum LogLevel
    {
        LOG_CRITICAL,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO
    };

    bool IsStringEmpty( const std::wstring &input );
    std::wstring GetTimeString( void );
    void LogMessage( LogLevel outputLogLevel, const std::wstring &message);
    void LogGLMessage( LogLevel outputLogLevel, GLenum ErrorStatus);


private:
    GLUtility();
    GLUtility( const GLUtility &other ) = delete;
    GLUtility &operator= ( const GLUtility &other ) = delete;

    std::wstring getGLStringFromCode( GLenum errorCode );
    
    std::wstring logFilename;
};