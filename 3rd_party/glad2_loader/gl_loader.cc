#include "gl_loader.h"

#include <SDL_video.h>
#include <glad/gl.h>

namespace gl_loader {

auto LoadGl() -> bool {
    auto function_loader = reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress);
    return gladLoadGL(function_loader) == 0;
}

}  // namespace gl_loader
