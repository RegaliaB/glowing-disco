#ifndef OPENGL_IMPL
#define OPENGL_IMPL
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#endif // !OPENGL_IMPL

extern "C" {
	#include "libavutil/imgutils.h"
	#include "libavcodec/avcodec.h"
	#include"libavcodec/codec.h"
	#include "libavformat/avformat.h"
	#include <libavutil/pixdesc.h>
	#include <libavutil/pixfmt.h>
	#include<libswscale/swscale.h>
	#include<libavfilter/avfilter.h>


}

//I am going to have to reconfigure how this works becaue I need the opengl to be impl here 
// 
// I wonder if getting rid or changing the def
// 
// I'm not going to use this at all, I will eventually delete this
// leaving it in to reference code from here
//namespace tex_util {
//
//
//
//    bool LoadTextureFromFrame(AVFrame* frame, GLuint* out_texture, int* out_width, int* out_height)
//    {
//
//
//        int image_width = 0;
//        int image_height = 0;
//
//
//
//        //unsigned char* image_data = "a";
//
//        GLuint image_texture;
//        glGenTextures(1, &image_texture);
//        glBindTexture(GL_TEXTURE_2D, image_texture);
//
//        // Setup filtering parameters for display
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
//        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
//
//        // Upload pixels into texture
//#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
//        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//#endif
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width,
//            frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//            frame->data[0]);
//
//
//        *out_texture = image_texture;
//        *out_width = image_width;
//        *out_height = image_height;
//
//
//        return true;
//    }
//}