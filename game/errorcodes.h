/*
==========================================
Copyright (c) 2020 Ostrich Labs

Big ol' list of error codes

Using good ol' defines because enums might by type safe but also I don't care
==========================================
*/

#ifndef OSTRICH_ERRORCODES_H_
#define OSTRICH_ERRORCODES_H_

// generic
#define OST_ERROR_OK            0x0000'0000 // no error
#define OST_ERROR_ISACTIVE      0x0000'0001 // subsystem is already active
#define OST_ERROR_EXCEPTCPP     0x0000'0002 // std::exception caught in Main
#define OST_ERROR_EXCEPTGENERIC 0x0000'0003 // ostrich::Exception caught in Main
#define OST_ERROR_EXCEPTINIT    0x0000'0004 // InitException caught in Main
#define OST_ERROR_EXCEPTPROXY   0x0000'0005 // ProxyException caught in Main

// Event Queue
#define OST_ERROR_EVENTQUEUE    0x0000'0100 // start of event queue errors
#define OST_ERROR_JOURNAL       (OST_ERROR_EVENTQUEUE+0x01) // unable to open Message Journal

// Signal handler
#define OST_ERROR_SIGNAL            0x0000'0200 // start of signal handler errors
#define OST_ERROR_HANDLERSIGINT     (OST_ERROR_SIGNAL+0x01) // unable to register SIGINT handler
#define OST_ERROR_HANDLERSIGTERM    (OST_ERROR_SIGNAL+0x02) // unable to register SIGTERM handler
#define OST_ERROR_HANDLERSIGHUP     (OST_ERROR_SIGNAL+0x03) // unable to register SIGHUP handler

// Input handler
#define OST_ERROR_INPUT         0x0000'0300             // start of input handler errors
#define OST_ERROR_UDEVINIT      (OST_ERROR_INPUT+0x01)  // udev - call to udev_new() failed
#define OST_ERROR_UDEVMONITOR   (OST_ERROR_INPUT+0x02)  // udev - call to udev_monitor_new_from_netlink() failed
#define OST_ERROR_UDEVNODEVICES (OST_ERROR_INPUT+0x03)  // udev - unable to add any devices
#define OST_ERROR_UDEVENUM      (OST_ERROR_INPUT+0x04)  // udev - call to udev_enumerate_new() failed
#define OST_ERROR_GLXGETDISPLAY (OST_ERROR_INPUT+0x05)  // GLX - call to glXGetCurrentDisplay() failed

// Display handler
#define OST_ERROR_DISPLAY               0x0000'0400 // start of display handler errors
#define OST_ERROR_WINREGISTERCLASS      (OST_ERROR_DISPLAY+0x01) // Windows - call to RegisterClass() failed
#define OST_ERROR_WINCREATEWINDOW       (OST_ERROR_DISPLAY+0x02) // Windows - call to CreateWindowExW() failed
#define OST_ERROR_WINGETDC              (OST_ERROR_DISPLAY+0x03) // Windows - call to GetDC() failed
#define OST_ERROR_WGLGETPROCADDR        (OST_ERROR_DISPLAY+0x04) // WGL - Critical WGL extension could not be loaded
// 0x05-0x07 available
#define OST_ERROR_WINGLCHOOSEFORMAT     (OST_ERROR_DISPLAY+0x08) // Windows - call to wglChoosePixelFormatARB() failed
#define OST_ERROR_WINGLGETFORMAT        (OST_ERROR_DISPLAY+0x09) // Windows - call to wglGetPixelFormatAttribivARB() failed
#define OST_ERROR_WINSETFORMAT          (OST_ERROR_DISPLAY+0x0A) // Windows - call to SetPixelFormat() failed
#define OST_ERROR_WINGLCREATECONTEXT    (OST_ERROR_DISPLAY+0x0B) // Windows - call to wglCreateContextAttribsARB() failed
#define OST_ERROR_WINGLMAKECURRENT      (OST_ERROR_DISPLAY+0x0C) // Windows - call to wglMakeCurrent() failed

#define OST_ERROR_RASPIGETDISPSIZE      (OST_ERROR_DISPLAY+0x0E) // Raspi - call to graphics_get_display_size() failed
#define OST_ERROR_RASPIDISPLAYOPEN      (OST_ERROR_DISPLAY+0x0F) // Raspi - call to vc_dispmanx_display_open() failed
#define OST_ERROR_RASPIUPDATESTART      (OST_ERROR_DISPLAY+0x10) // Raspi - call to vc_dispmanx_update_start() failed
#define OST_ERROR_RASPIELEMENTADD       (OST_ERROR_DISPLAY+0x11) // Raspi - call to vc_dispmanx_element_add() failed
#define OST_ERROR_ES2GETDISPLAY         (OST_ERROR_DISPLAY+0x12) // ES2 - call to eglGetDisplay() failed
#define OST_ERROR_ES2INITIALIZE         (OST_ERROR_DISPLAY+0x13) // ES2 - call to eglInitialize() failed
#define OST_ERROR_ES2CHOOSECONFIG       (OST_ERROR_DISPLAY+0x14) // ES2 - call to eglChooseConfig() failed
#define OST_ERROR_ES2CREATEWINSURFACE   (OST_ERROR_DISPLAY+0x15) // ES2 - call to eglCreateWindowSurface() failed
#define OST_ERROR_ES2CREATECONTEXT      (OST_ERROR_DISPLAY+0x16) // ES2 - call to eglCreateContext() failed
#define OST_ERROR_ES2MAKECURRENT        (OST_ERROR_DISPLAY+0x17) // ES2 - call to eglMakeCurrent() failed

#define OST_ERROR_X11OPENDISPLAY        (OST_ERROR_DISPLAY+0x18) // X11 - call to XOpenDisplay() failed
#define OST_ERROR_GLXQUERYVERSION       (OST_ERROR_DISPLAY+0x19) // GLX - call to glXQueryVersion() failed
#define OST_ERROR_GLXVERSION            (OST_ERROR_DISPLAY+0x1A) // GLX - version is not 1.3 or newer
#define OST_ERROR_GLXCHOOSEFBCONFIG     (OST_ERROR_DISPLAY+0x1B) // GLX - call to glXChooseFBConfig() failed
#define OST_ERROR_GLXGETVISUALFROMFB    (OST_ERROR_DISPLAY+0x1C) // GLX - call to glXGetVisualFromFBConfig() failed
#define OST_ERROR_XCREATEWINDOW         (OST_ERROR_DISPLAY+0x1D) // X11 - call to XCreateWindow() failed
#define OST_ERROR_GLXCREATECONTEXT      (OST_ERROR_DISPLAY+0x1E) // GLX - unable to generate context
#define OST_ERROR_GLXINDIRECT           (OST_ERROR_DISPLAY+0x1F) // GLX - context is indirect somehow
#define OST_ERROR_GLXMAKECURRENT        (OST_ERROR_DISPLAY+0x20) // GLX - call to glXMakeCurrent() failed

// Renderer - OpenGL 4
#define OST_ERROR_GL4                   0x0000'0500 // start of OpenGL 4 renderer errors
#define OST_ERROR_GL4GETSTRING          (OST_ERROR_GL4+0x01) // GL - call to glGetString() failed
#define OST_ERROR_GL4VERSION            (OST_ERROR_GL4+0x02) // GL - retrieved OpenGL version unsupported
#define OST_ERROR_GLSHADERVERSION       (OST_ERROR_GL4+0x03) // GL - OpenGL Shading Language version unsupported
#define OST_ERROR_GL4COREGETPROCADDR    (OST_ERROR_GL4+0x04) // GL - Failed to load a core OpenGL 4 function pointer

// Renderer - OpenGL ES2
#define OST_ERROR_ES2                   0x0000'0700 // start of OpenGL ES2 renderer errors
#define OST_ERROR_ES2GETSTRING          (OST_ERROR_ES2+0x01) // ES2 - call to glGetString() failed
#define OST_ERROR_ES2VERSION            (OST_ERROR_ES2+0x02) // ES2 - retrieved OpenGL ES version unsupported
#define OST_ERROR_ES2SHADERVERSION      (OST_ERROR_ES2+0x03) // ES2 - OpenGL ES Shading Language version unsupported

// State machine
#define OST_ERROR_STATEMACHINE 0x0000'00900 // start of state machine errors

#endif /* OSTRICH_ERRORCODES_H_ */
