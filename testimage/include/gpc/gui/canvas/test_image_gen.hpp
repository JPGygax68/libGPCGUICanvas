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

                static const int WIDTH = 1200, HEIGHT = 675;

                auto generate(Canvas *canvas) -> std::vector<RGBA32>
                {
                    std::vector<RGBA32> img;

                    canvas->clear(canvas->rgba_to_native({ 0.8f, 0.8f, 0.8f, 0 }));

                    draw_grid(canvas);

                    return img;
                }

            private:

                void draw_grid(Canvas *canvas)
                {
                    Canvas::native_color_t red  = canvas->rgba_to_native({1, 0, 0, 1});
                    Canvas::native_color_t blue = canvas->rgba_to_native({0, 0, 1, 1});

                    // Horizontal
                    for (int y = 0; y <= HEIGHT; y += 50) {
                        canvas->fill_rect(0, y-2, WIDTH, 2, red);
                        canvas->fill_rect(0, y, WIDTH, 2, blue);
                    }
                    // Vertical
                    for (int x = 0; x <= WIDTH; x += 50) {
                        canvas->fill_rect(x-2, 0, 2, HEIGHT, red);
                        canvas->fill_rect(x, 0, 2, HEIGHT, blue);
                    }
                }
            };

        } // ns canvas

    } // ns gui

} // ns gpc