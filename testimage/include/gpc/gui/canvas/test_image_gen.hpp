#pragma once

//#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/RasterizedFont.hpp>
#include <gpc/fonts/cereal.hpp>

#include <gpc/gui/canvas.hpp> // TODO: replace with specific header file for color handling when available

#include "fonts.hpp"

namespace gpc {

    namespace gui {
    
        namespace canvas {

            template <
                typename Canvas 
            >
            class TestImageGenerator {
            public:

                static const int WIDTH = 1280, HEIGHT = 1024;

                auto generate(Canvas *canvas) -> std::vector<RGBA32>
                {
                    std::vector<RGBA32> img;

                    Canvas::native_color_t clr = canvas->rgba_to_native( {1, 1, 1, 0} );
                    canvas->clear(clr);

                    return img;
                }

            private:
            };

        } // ns canvas

    } // ns gui

} // ns gpc