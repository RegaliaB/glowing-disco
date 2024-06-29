#pragma once



#include "imgui.h"
#include "imgui.cpp"

#ifndef OPENGL_IMPL
#define OPENGL_IMPL
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#endif // !OPENGL_IMPL


#include "imgui_impl_glfw.cpp"
#include "imgui_impl_opengl3.cpp"
#include "imgui_widgets.cpp"
#include "imgui_tables.cpp"
#include "imgui_internal.h"
#include "imgui_draw.cpp"
#include "imgui_demo.cpp"
