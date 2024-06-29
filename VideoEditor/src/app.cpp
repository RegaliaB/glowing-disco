
// Message to beginner C/C++ programmers about the meaning of the 'static' keyword:
// In this demo code, we frequently use 'static' variables inside functions. A static variable persists across calls,
// so it is essentially like a global variable but declared inside the scope of the function. We do this as a way to
// gather code and data in the same place, to make the demo source code faster to read, faster to write, and smaller
// in size. It also happens to be a convenient way of storing simple UI related information as long as your function
// doesn't need to be reentrant or used in multiple threads. This might be a pattern you will want to use in your code,
// but most of the real data you would be editing is likely going to be stored outside your functions.

// The Demo code in this file is designed to be easy to copy-and-paste into your application!
// Because of this:
// - We never omit the ImGui:: prefix when calling functions, even though most code here is in the same namespace.
// - We try to declare static variables in the local scope, as close as possible to the code using them.
// - We never use any of the helpers/facilities used internally by Dear ImGui, unless available in the public API.
// - We never use maths operators on ImVec2/ImVec4. For our other sources files we use them, and they are provided
//   by imgui_internal.h using the IMGUI_DEFINE_MATH_OPERATORS define. For your own sources file they are optional
//   and require you either enable those, either provide your own via IM_VEC2_CLASS_EXTRA in imconfig.h.
//   Because we can't assume anything about your support of maths operators, we cannot use them in imgui_demo.cpp.

// Navigating this file:
// - In Visual Studio IDE: CTRL+comma ("Edit.GoToAll") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
// - With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.

/*

Index of this file:

// [SECTION] Forward Declarations
// [SECTION] Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// - ShowDemoWindow()
// - sub section: ShowDemoWindowWidgets()
// - sub section: ShowDemoWindowLayout()
// - sub section: ShowDemoWindowPopups()
// - sub section: ShowDemoWindowTables()
// - sub section: ShowDemoWindowInputs()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] User Guide / ShowUserGuide()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Fullscreen window / ShowExampleAppFullscreen()
// [SECTION] Example App: Manipulating window titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Docking, DockSpace / ShowExampleAppDockSpace()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include "app.h"
//#include "imgui.cpp"

//#ifndef OPENGL_IMPL
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//#include "imgui_impl_opengl3_loader.h"
//
//#endif // !OPENGL_IMPL
//



#ifndef IMGUI_DISABLE

// System includes
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif
using namespace Decode;


// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to an 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'                     // not all warnings are known by all Clang versions and they tend to be rename-happy.. so ignoring warnings triggers new warnings on some configuration. Great!
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast                           // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"        // warning: 'xx' is deprecated: The POSIX name for this..   // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"       // warning: cast to 'void *' from smaller integer type
#pragma clang diagnostic ignored "-Wformat-security"                // warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"          // warning: declaration requires an exit-time destructor    // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"                  // warning: macro is not used                               // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant                   // some standard header variations use #define NULL 0
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#pragma clang diagnostic ignored "-Wreserved-id-macro"              // warning: macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"          // warning: format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"   // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif

// Play it nice with Windows users (Update: May 2018, Notepad now supports Unix-style carriage returns!)
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif

// Helpers
#if defined(_MSC_VER) && !defined(snprintf)
#define snprintf    _snprintf
#endif
#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf   _vsnprintf
#endif

// Format specifiers, printing 64-bit hasn't been decently standardized...
// In a real application you should be using PRId64 and PRIu64 from <inttypes.h> (non-windows) and on Windows define them yourself.
#ifdef _MSC_VER
#define IM_PRId64   "I64d"
#define IM_PRIu64   "I64u"
#else
#define IM_PRId64   "lld"
#define IM_PRIu64   "llu"
#endif

// Helpers macros
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifndef IMGUI_CDECL
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
//static void ShowExampleAppDockSpace(bool* p_open);
//static void ShowExampleAppDocuments(bool* p_open);
//static void ShowExampleAppMainMenuBar();
//static void ShowExampleAppConsole(bool* p_open);
//static void ShowExampleAppLog(bool* p_open);
//static void ShowExampleAppLayout(bool* p_open);
//static void ShowExampleAppPropertyEditor(bool* p_open);
//static void ShowExampleAppLongText(bool* p_open);
//static void ShowExampleAppAutoResize(bool* p_open);
//static void ShowExampleAppConstrainedResize(bool* p_open);
//static void ShowExampleAppSimpleOverlay(bool* p_open);
//static void ShowExampleAppFullscreen(bool* p_open);
//static void ShowExampleAppWindowTitles(bool* p_open);
//static void ShowExampleAppCustomRendering(bool* p_open);
//static void ShowExampleMenuFile();
//
//// We split the contents of the big ShowDemoWindow() function into smaller functions
//// (because the link time of very large functions grow non-linearly)
//static void ShowDemoWindowWidgets();
//static void ShowDemoWindowLayout();
//static void ShowDemoWindowPopups();
//static void ShowDemoWindowTables();
//static void ShowDemoWindowColumns();
//static void ShowDemoWindowInputs();
//
////-----------------------------------------------------------------------------
//// [SECTION] Helpers
////-----------------------------------------------------------------------------
//
//// Helper to display a little (?) mark which shows a tooltip when hovered.
//// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
//static void HelpMarker(const char* desc)
//{
//    ImGui::TextDisabled("(?)");
//    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
//    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
//    {
//        ImGui::BeginTooltip();
//        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//        ImGui::TextUnformatted(desc);
//        ImGui::PopTextWrapPos();
//        ImGui::EndTooltip();
//    }
//}
//
//static void ShowDockingDisabledMessage()
//{
//    ImGuiIO& io = ImGui::GetIO();
//    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
//    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
//    ImGui::SameLine(0.0f, 0.0f);
//    if (ImGui::SmallButton("click here"))
//        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//}
//
//// Helper to wire demo markers located in code to an interactive browser
//typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
//extern ImGuiDemoMarkerCallback      GImGuiDemoMarkerCallback;
//extern void* GImGuiDemoMarkerCallbackUserData;
//ImGuiDemoMarkerCallback             GImGuiDemoMarkerCallback = NULL;
//void* GImGuiDemoMarkerCallbackUserData = NULL;
//#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)
void ImGui::MainMenuWindow(bool* p_open)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {

            }
            if (ImGui::MenuItem("Import"))
            {

            }
            if (ImGui::MenuItem("Export"))
            {

            }
            if (ImGui::MenuItem("Exit")) {
                *p_open = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) 
        {

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void InitiateOpen(const char* filename) {
    reader_state* new_reader;
    new_reader = new reader_state();
    Decode::decoder_open(new_reader, filename);
}

void ImGui::PlayBackWindow(bool* p_open) 
{
    
    //const ImVec2* main_window = 
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    //ImVec2 windowSize = main_viewport->Size;
    ImGuiWindowFlags pb_flags = 0;
    pb_flags |= ImGuiWindowFlags_NoScrollbar;
    pb_flags |= ImGuiWindowFlags_MenuBar;
    pb_flags |= ImGuiWindowFlags_NoCollapse;
    pb_flags |= ImGuiWindowFlags_NoResize;

    if (*p_open) {
        if (ImGui::Begin("Player", p_open, pb_flags))
        {
            //ImGui::Image();


            ImGui::End();
        }

    }
    else {
        if (ImGui::Begin("Player", p_open, pb_flags))
        {
            //ImGui::Image();


            ImGui::End();
        }
    }
    //use ImGuiCond_FirstUseEver if you want it to save to the ini and be the same every time

    //ImGui::SetNextWindowPos(ImVec2(0, main_viewport->WorkSize.y / 2));
    //ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y / 2));

   
    
   
}

void ImGui::EditingWindow(bool* p_open)
{
    //const ImVec2* main_window = 
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    
    ImVec2 windowSize = main_viewport->WorkSize;
    ImVec2 windowPos = main_viewport->WorkPos;
    ImGuiWindowFlags pb_flags = 0;
    ImGuiCond pb_cond = 0;
    pb_flags |= ImGuiWindowFlags_NoScrollbar;
    pb_flags |= ImGuiWindowFlags_NoCollapse;
    pb_flags |= ImGuiWindowFlags_NoResize;
    pb_flags |= ImGuiWindowFlags_NoTitleBar;
    pb_cond |= ImGuiCond_Once;

    //use ImGuiCond_FirstUseEver if you want it to save to the ini and be the same every time
    //use ImGui::DockSpace(dockspace_id)
    //use ImGui::DockBuilder
   

    if (ImGui::Begin("Editor", p_open, pb_flags))
    {



        ImGui::End();
    }

   
}
#endif
#endif