#include <Precompiled.h>
#include <Application.h>
#include <ChapterTwo.h>

// Easiest I've found to do C++ with the windows message pump. Just make sure that this pointer is 
// set within the constructor of the application class.
static Application *applicationHandle = nullptr;
static LRESULT CALLBACK windowMessageProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;
    default:
        if( applicationHandle != nullptr )
        {
            if( applicationHandle->IsInitialized() == true )
            {
                // Pass through to the handler in the class. This function is pretty much just
                // a wrapper.
                return applicationHandle->MessageHandler( handle, message, wParam, lParam );
            }
        }
    }
        
    return DefWindowProc( handle, message, wParam, lParam );
}

Application::Application( const std::wstring &set_name )
{
    if( set_name.size() <= 0 )
    {
        application_name = L"GL_application";
    }
    else
    {
        application_name = set_name;
    }

    class_name = set_name + L"_class";
    
    max_frame_time = 0.25;
    
    last_time = 0.0;
    accumulated_time = 0.0;
    time_step = 0.02;

    initialized = false;
    done = false;
    changed_resolution = false;

    window_activated = false;
    window_initialized = false;

    window_width = 480;
    window_height = 300;
    screen_width = 0;
    screen_height = 0;
    window_x_position = 0;
    window_y_position = 0;
   
    is_windowed = false;

    down_keys = std::vector<bool>( 256, false );
    pressed_keys = std::vector<bool>( 256, false );

    timing_frequency = 0.0;
    counter_start = 0;

    applicationHandle = this;
}

// Nothing to see here
Application::~Application( void )
{
}

bool Application::Initialize( void )
{   
    chapter = std::make_shared<ChapterTwo>(this);

    if( initializeWindow() == false )
    {
        MessageBox( NULL, L"Failed to initialize window.", L"SYSTEM ERROR", MB_OK | MB_ICONERROR );
        initialized = false;
        window_activated = false;

        return false;
    }
    
    // Here we initialize the timing system.
    LARGE_INTEGER timing_query;
    if( false == QueryPerformanceFrequency( &timing_query ) )
    {
        initialized = false;
        return false;
    }

    timing_frequency = static_cast<double>( timing_query.QuadPart ) / 1000.0;
    QueryPerformanceCounter( &timing_query );
    counter_start = timing_query.QuadPart;
    last_time = getTime();

    initialized = true;

    GLUtility::Instance( L"logFile.txt" );

    return true;
}

bool Application::Run( void )
{
    MSG message;
    ZeroMemory( &message, sizeof( message ) );

    while( done == false )
    {
        if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &message );
            DispatchMessage( &message );
        }

        if( ( WM_QUIT == message.message ) || ( false == initialized ) )
        {
            done = true;
            break;
        }

        // ESC is the default quit. Probably will change this later if 
        // I need a more robust state system.
        if( IsKeyDown( VK_ESCAPE ) )
        {
            done = true;
            break;
        }

        // Fixed time-step, full speed render. To accomplish this, we
        // use an accumulator.

        // First, we get the time since the last render.
        double new_time = getTime();
        double frame_time = new_time - last_time;
        if( max_frame_time < frame_time )
        {
            frame_time = max_frame_time;
        }
        last_time = new_time;

        // Then we add it to the accumulator.
        accumulated_time += frame_time;

        // We then subtract off fixed chunks of time and simulate according
        // to that time step. This gives us a fixed simulation time-step, while
        // allowing us to render with a variable time-step.
        while( accumulated_time > time_step )
        {
            accumulated_time -= time_step;
            step();
        }

        draw();
    }

    return true;
}


void Application::Shutdown( void )
{
    if( chapter != nullptr )
    {
        chapter->Shutdown();
    }

    shutdownWindow();
}

void Application::draw( void )
{
    // This will be expanded if we switch to a state system
    if( chapter != nullptr )
    {
        chapter->Draw();
    }

    SwapBuffers( device_context );
}

double Application::getTime( void )
{
    if( false == initialized )
    {
        return -1.0;
    }

    LARGE_INTEGER timing_query;
    if( false == QueryPerformanceFrequency( &timing_query ) )
    {
        return -1.0;
    }

    timing_frequency = static_cast<double>( timing_query.QuadPart ) / 1000.0;
    QueryPerformanceCounter( &timing_query );

    // Returns the difference between the current time and the start time. The delta is then
    // divided by the frequency (ticks / ticks_per_second = seconds)
    return static_cast<double>( timing_query.QuadPart - counter_start ) / timing_frequency;
}
    
void Application::initializeExtensions( void )
{
    GLenum result;
    PIXELFORMATDESCRIPTOR pixel_format_descriptor;

    // Create a temporary context while we set up GLEW
    device_context = GetDC( h_wnd );
    if( NULL == device_context )
    {
        MessageBox( NULL, L"Could not get device context", L"Renderer Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    result = SetPixelFormat( device_context, 1, &pixel_format_descriptor );
    if( GL_TRUE != result )
    {
        MessageBox( NULL, L"Could not set temporary pixel format.", L"Renderer Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    temp_render_context = wglCreateContext( device_context );
    if( NULL == render_context )
    {
        MessageBox( NULL, L"Could not get OpenGL context", L"Renderer Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    result = wglMakeCurrent( device_context, temp_render_context );
    if( GL_TRUE != result )
    {
        MessageBox( NULL, L"Could not make render context current.", L"Renderer Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    if( GLUtility::Instance()->InitializeGLExtensions() == false )
    {
        MessageBox( NULL, L"Could not load extensions", L"Renderer Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    // Release the temporary context.
    wglMakeCurrent( NULL, NULL );
    ReleaseDC( h_wnd, device_context );
    device_context = NULL;
}

void Application::initializeRenderer( void )
{
    GLenum result;
    PIXELFORMATDESCRIPTOR pixel_format_descriptor;

    unsigned int format_count;
    std::string vendor;
    std::string renderer;

    // Double buffered OpenGL accelerated context, with 32-bit color, 24 bit depth, and 8-bit stencil.
    int attribute_list_int[] =
    {
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
        WGL_STENCIL_BITS_ARB, 8,
        0
    };

    device_context = GetDC( h_wnd );
    if( NULL == device_context )
    {
        MessageBox( NULL, L"Could not get the device context", L"Render Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    int pixel_format;
    result = wglChoosePixelFormatARB( device_context, attribute_list_int, NULL, 1, &pixel_format, &format_count );
    if( GL_TRUE != result )
    {
        MessageBox( NULL, L"Could not choose the pixel format.", L"Render Initialization Error", MB_OK | MB_ICONERROR );
        return;
    }

    result = SetPixelFormat( device_context, pixel_format, &pixel_format_descriptor );
    if( GL_TRUE != result )
    {
        MessageBox( NULL, L"Error setting pixel format", L"Render Initialization", MB_OK | MB_ICONERROR );
        return;
    }

    // OpenGL 3.3 core context
    int attribute_list[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    render_context = wglCreateContextAttribsARB( device_context, 0, attribute_list );
    if( NULL == render_context )
    {
        MessageBox( NULL, L"Could not create the rendering context.", L"Render Initialization Error", MB_OK | MB_ICONERROR );
        return;
    }

    result = wglMakeCurrent( device_context, render_context );
    if( GL_TRUE != result )
    {
        MessageBox( NULL, L"Could not make the context current.", L"Render Initialization Error", MB_OK | MB_ICONERROR );
        return;
    }
    
    // Reload the extensions
    if( GLUtility::Instance()->InitializeGLExtensions() == false )
    {
        GLUtility::Instance( L"Logfile.txt" )->LogMessage( GLUtility::LogLevel::LOG_CRITICAL, L"Could not load OpenGL extensions for application render context." );
        return;
    }

    // For now we don't want any culling or depth testing
    // Just get the triangles on the screen
    glClearDepth( 1.0 );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    // Clockwise triangles face forward
    glFrontFace( GL_CW );

    auto get_vendor = const_cast< GLubyte * >( glGetString( GL_VENDOR ) );
    vendor = std::string( reinterpret_cast< char * >( get_vendor ) );
    auto get_renderer = const_cast< GLubyte * >( glGetString( GL_RENDERER ) );
    renderer = std::string( reinterpret_cast< char * >( get_renderer ) );

    wglSwapIntervalEXT( 1 );
}

bool Application::initializeWindow( void )
{
    h_instance = GetModuleHandle( NULL );

    // Standard windows boilerplate
    WNDCLASSEX wcx;
    ZeroMemory( &wcx, sizeof( wcx ) );

    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );
    wcx.hCursor = LoadCursor( NULL, IDC_CROSS );
    wcx.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wcx.hIconSm = wcx.hIcon;
    wcx.hInstance = h_instance;
    wcx.lpfnWndProc = windowMessageProc;
    wcx.lpszClassName = class_name.c_str();
    wcx.lpszMenuName = NULL;
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.cbSize = sizeof( wcx );

    RegisterClassEx( &wcx );

    // The following code assumes we only have one monitor and are displaying on the primary monitor

    // The following code sets up a windowed display that is centered on the screen
    screen_width = GetSystemMetrics( SM_CXSCREEN );
    screen_height = GetSystemMetrics( SM_CYSCREEN );

    if( window_width > screen_width )
    {
        window_width = screen_width - 20;
    }
    if( window_height > screen_height )
    {
        window_height = screen_height - 20;
    }

    window_x_position = ( screen_width - window_width ) / 2;
    window_y_position = ( screen_height - window_height ) / 2;
    
    // Here we create a temporary window so that we can initialize GLEW
    h_wnd = CreateWindowEx( NULL, class_name.c_str(), application_name.c_str(), WS_POPUP, 100, 100, 100, 100,
                            NULL, NULL, h_instance, NULL );
    if( NULL == h_wnd )
    {
        return false;
    }
    ShowWindow( h_wnd, SW_HIDE );
    initializeExtensions();
    DestroyWindow( h_wnd );
    h_wnd = NULL;

    // And now we can finally create the context
    h_wnd = CreateWindowEx( NULL, class_name.c_str(), application_name.c_str(), WS_POPUP, window_x_position, window_y_position,
                            window_width, window_height, NULL, NULL, h_instance, NULL );
    if( NULL == h_wnd )
    {
        return false;
    }
    window_activated = false;
    initializeRenderer();

    ShowWindow( h_wnd, SW_SHOW );
    SetFocus( h_wnd );
    SetForegroundWindow( h_wnd );

    window_activated = true;
    window_initialized = true;

    // Call any initialization of the chapter demo (now that the context is up and running.)
    if( chapter != nullptr )
    {
        chapter->Initialize();
    }

    return true;
}

bool Application::resizeWindow( unsigned int width, unsigned int height )
{
    // Needs to be implemented
    return true;
}

void Application::shutdownWindow( void )
{
    // Release the context
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( render_context );
    wglDeleteContext( temp_render_context );
    temp_render_context = NULL;        
    render_context = NULL;
    ReleaseDC( h_wnd, device_context );
    device_context = NULL;

    // If we maximized the window, reset the display back to default
    if( changed_resolution == true )
    {
        ChangeDisplaySettings( NULL, 0 );
    }

    // Cleans up the window properly
    DestroyWindow( h_wnd );
    UnregisterClass( class_name.c_str(), h_instance );
    h_instance = NULL;

    applicationHandle = nullptr;
}

void Application::step( void )
{
    if( chapter != nullptr )
    {
        chapter->Step( time_step );
    }
}

bool Application::IsInitialized( void ) const
{
    return initialized;
}
    
bool Application::IsKeyDown( unsigned int keycode )
{
    return down_keys[keycode];
}

bool Application::IsKeyPressed( unsigned int keycode )
{
    bool isPressed = pressed_keys[keycode];
    pressed_keys[keycode] = false;
    return isPressed;
}


LRESULT CALLBACK Application::MessageHandler( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
    unsigned int keycode = static_cast< unsigned int >( wParam );
    switch( message )
    {
    case WM_KEYDOWN:
        if( ( keycode >= 0 ) && ( keycode < 256 ) )
        {
            // On the first keydown
            if( ( lParam & ( 1 << 30 ) ) == 0 )
            {
                pressed_keys[keycode] = true;
            }

            if( down_keys[keycode] == true )
            {
                pressed_keys[keycode] = false;
            }

            down_keys[keycode] = true;
        }
        break;
    case WM_KEYUP:
        if( ( keycode >= 0 ) && ( keycode < 256 ) )
        {
            pressed_keys[keycode] = false;
            down_keys[keycode] = false;
        }
        break;
    case WM_ACTIVATE:
        if( true == window_initialized )
        {
            // These lines handle the always-on-top behavior. In other words, if the window is active,
            // then it's at the top of the window Z-order, otherwise it's at the bottom.
            if( true == window_activated )
            {
                window_activated = false;
                SetWindowPos( h_wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            }
            else
            {
                window_activated = true;
                SetWindowPos( h_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            }
        }
        break;
    default:
        break;
    }
    return DefWindowProc( handle, message, wParam, lParam );
}