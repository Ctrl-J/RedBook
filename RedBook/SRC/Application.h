#pragma once
#include <Precompiled.h>
#include <ChapterBase.h>
#include <ChapterOne.h>
#include <ChapterTwo.h>

class Application
{
public:

    // Constructor just sets some defaults.
    Application( const std::wstring &set_name );
    ~Application( void );

    // Rule of three, and there's no reason to copy the application.
    Application( const Application &other ) = delete;
    Application &operator= ( const Application &other ) = delete;

    // Initializes the all of the application systems.
    bool Initialize( void );

    // Handles the main loop and windows message pump
    bool Run( void );

    // Cleans up anything that is not automatically cleaned up
    void Shutdown( void );

    // Handles any application relevant Win32 messages
    LRESULT CALLBACK MessageHandler( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

    // Accessor for internal initialization status
    bool IsInitialized( void ) const;
    
    // Keyboard input status. Down fires as long as the key is held,
    // pressed only fires on the first time the key is hit
    bool IsKeyDown( unsigned int keycode );
    bool IsKeyPressed( unsigned int keycode );
   
private:

    // Draws the scene.
    void draw( void );
   
    // gets the current time using QueryPerformanceCounter/Frequency
    double getTime( void );

    // Initializes GLEW with a temporary context. GLEW needs a context in
    // order to be initialized, but we need GLEW to get a modern context.
    // So, we need a temporary context before we get the 4.3 Core context.
    void initializeExtensions( void );

    // Creates an OpenGL 4.3 render context
    void initializeRenderer( void );

    // Creates the Win32 application window and makes the appropriate calls
    // to make sure that the context and GLEW are setup properly
    bool initializeWindow( void );

    // Not yet implemented. This will resize the current window to the size 
    // specified, handling any viewport changes. Possibly going to throw
    // window fullscreen toggling into here?
    bool resizeWindow( unsigned int width, unsigned int height );

    // Cleans up the window and destroys the OpenGL context
    void shutdownWindow( void );

    // Logic step wrapper. So that we can insert state updates into the main loop.
    void step( void );

    // Currently just the first chapter state. Eventually I will expand
    // this to be a polymorphic class target and possibly enable all of
    // the chapter demos to be played through some sort of interface
    std::shared_ptr<ChapterBase> chapter;

    std::wstring application_name;
    std::wstring class_name;

    HINSTANCE h_instance;
    HWND h_wnd;

    HDC device_context;
    HGLRC render_context;
    HGLRC temp_render_context;

    double max_frame_time;
    double last_time;
    double accumulated_time;
    double timing_frequency;
    double time_step;
    __int64 counter_start;

    bool initialized;
    bool done;
    bool changed_resolution;

    bool window_activated;
    bool window_initialized;

    int window_width;
    int window_height;
    int screen_width;
    int screen_height;
    int window_x_position;
    int window_y_position;

    bool is_windowed;

    // storage for key state
    std::vector<bool> down_keys;
    std::vector<bool> pressed_keys;
};