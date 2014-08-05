#pragma once
#include <Precompiled.h>

class GLUtility
{
public:
    enum LogLevel
    {
        LOG_CRITICAL,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO
    };
    
    void SetLogFilename( std::wstring filename );

    bool IsStringEmpty( std::wstring input );
    std::wstring GetTimeString( void );
    void LogMessage( LogLevel outputLogLevel, std::wstring message);
    void LogGLMessage( LogLevel outputLogLevel, GLuint ErrorStatus);


private:
    std::wstring logFilename;

};