# Raytracer
A simple raytracer and renderer implemented in C++ using OpenGL, GLEW, and GLM
 
The project uses a basic MFC framework for GUI implementation. The main feature is the Render->Raytrace menu option. When activating this the application may freeze intermittently, this is normal, as the ray computation is very CPU/GPU heavy.

## Architecture

Supports Microsoft Windows-32 bit (x86) Builds **ONLY**

## Dependencies

1. OpenGL - A graphics library
2. [GLEW](https://github.com/nigels-com/glew) - An extension library for OpenGL
3. GLM - A mathematical extensions library
4. [Microsoft 2005 C++ Redistributables](http://www.microsoft.com/download/en/details.aspx?id=26347%20%20) - Runtime redistributables

## Common Issues & Fixes

1. "Application Unable to Start Correctly"

    Your machine does not have the Dependency #4 (C++ runtime redistributables) installed.

    https://community.rti.com/kb/how-resolve-error-application-was-unable-start-correctly-0xc0150002-click-ok-close-application
