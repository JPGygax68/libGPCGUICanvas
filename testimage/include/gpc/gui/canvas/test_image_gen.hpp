#pragma once

//#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/RasterizedFont.hpp>
#include <gpc/fonts/cereal.hpp>

#include <gpc/gui/canvas.hpp> // TODO: replace with specific header file for color handling when available

namespace gpc {

    namespace gui {
    
        namespace canvas {

            template <
                typename Canvas 
            >
            class TestImageGenerator {
            public:

                static const int WIDTH = 1200, HEIGHT = 675;

                TestImageGenerator(): canvas(nullptr) {}

                void init(Canvas *canvas_)
                {
                    canvas = canvas_;

                    register_fonts();
                }

                void register_fonts()
                {
                    static char sans_reg_20[] = {
                        #include "LiberationSans-Regular-20.rft.h"
                    };

                    std::string data_string(sans_reg_20, sans_reg_20 + sizeof(sans_reg_20));
                    std::stringstream sstr(data_string);
                    cereal::BinaryInputArchive ar(sstr);
                    gpc::fonts::RasterizedFont rfont;
                    ar >> rfont;

                    font = canvas->register_font(rfont);

                    // TODO: dispose of fonts
                }

                auto generate() -> std::vector<RGBA32>
                {
                    std::vector<RGBA32> img;

                    canvas->clear(canvas->rgba_to_native({ 0.8f, 0.8f, 0.8f, 0 }));

                    draw_grid();
                    draw_plain_rects();

                    return img;
                }

            private:

                void draw_grid()
                {
                    static const int LINE_WIDTH = 1;

                    Canvas::native_color_t before = canvas->rgba_to_native({0, 0, 0, 1});
                    Canvas::native_color_t after = canvas->rgba_to_native({1, 1, 1, 1});

                    // Vertical axis
                    for (int y = 0; y <= HEIGHT; y += 50) {
                        canvas->fill_rect(0, y - LINE_WIDTH, WIDTH, LINE_WIDTH, before);
                        canvas->fill_rect(0, y, WIDTH, LINE_WIDTH, after);
                        auto label = std::to_string(y);
                        canvas->draw_text(font, 4, y - 4, label.c_str(), label.size());
                    }
                    // Horizontal axis
                    for (int x = 0; x <= WIDTH; x += 50) {
                        canvas->fill_rect(x - LINE_WIDTH, 0, LINE_WIDTH, HEIGHT, before);
                        canvas->fill_rect(x, 0, LINE_WIDTH, HEIGHT, after);
                        auto label = std::to_string(x);
                        canvas->draw_text(font, x+4, 18, label.c_str(), label.size());
                    }
                }

                void draw_plain_rects()
                {
                    Canvas::native_color_t red = canvas->rgba_to_native({ 1, 0, 0, 1 });
                    Canvas::native_color_t green = canvas->rgba_to_native({ 0, 1, 0, 1 });
                    Canvas::native_color_t blue = canvas->rgba_to_native({ 0, 0, 1, 1 });
                    Canvas::native_color_t white = canvas->rgba_to_native({ 1, 1, 1, 1 });

                    canvas->fill_rect(50, 50, 50, 50, red);
                    canvas->fill_rect(100, 50, 50, 50, green);
                    canvas->fill_rect(50, 100, 50, 50, blue);
                    canvas->fill_rect(100, 100, 50, 50, white);
                }

                Canvas *canvas;
                typename Canvas::font_handle_t font;
            };

        } // ns canvas

    } // ns gui

} // ns gpc