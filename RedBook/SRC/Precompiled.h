#pragma once
// Obvious include for a Win32 application
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// GLEW is included to handle loading GL extensions, since handling all the function pointers
// seemed like an interesting exercise, but not the point of this project
#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLEW\wglew.h>

// GLM for my math functions! And the constants header (mainly for pi)
#include <GLM\glm.hpp>
#include <GLM\gtc\constants.hpp>

// Basic stream manipulation, memory (for c++11 smart pointers), and basic data structures.
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <map>