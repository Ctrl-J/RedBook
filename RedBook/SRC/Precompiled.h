#pragma once
// Obvious include for a Win32 application
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

// OpenGl
#include <gl\GL.h>
#include <GLEXT\glext.h>
#include <GLEXT\wglext.h>

// GLM for math functions! And the constants header (mainly for pi)
#include <GLM\glm.hpp>
#include <GLM\gtc\constants.hpp>
#include <GLM\gtc\type_ptr.hpp>

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

#include <GLUtility.h>