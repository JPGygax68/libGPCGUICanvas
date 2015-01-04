#pragma once

//#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/RasterizedFont.hpp>
#include <gpc/fonts/cereal.hpp>

#include <gpc/gui/renderer.hpp> // TODO: replace with specific header file for color handling when available

namespace gpc {

    namespace gui {
    
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

                register_test_image();
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

            void register_image()
            {
            }

            auto generate() -> std::vector<RGBA32>
            {
                std::vector<RGBA32> img;

                canvas->clear(canvas->rgba_to_native({ 0.8f, 0.8f, 0.8f, 0 }));

                draw_grid();
                draw_plain_rects();
                draw_images(200, 50);

                return img;
            }

        private:

            static auto
            makeColorInterpolatedRectangle(size_t width, size_t height, const std::array<RGBAFloat, 4> &corner_colors) -> std::vector<RGBA32>
            {
                std::vector<RGBA32> image(width * height);

                auto it = image.begin();
                for (auto y = 0U; y < height; y++) {
                    for (auto x = 0U; x < width; x++) {
                        RGBAFloat top = interpolate(corner_colors[0], corner_colors[1], float(x) / float(width));
                        RGBAFloat bottom = interpolate(corner_colors[2], corner_colors[3], float(x) / float(width));
                        RGBAFloat color = interpolate(top, bottom, float(y) / float(height));
                        *it = fromFloat(color);
                        //*it = { {255, 0, 0, 255} };
                        it++;
                    }
                }

                return image;
            }

            void register_test_image()
            {
                auto pixels = makeColorInterpolatedRectangle(50, 50, { { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1, 1, 1 } } });
                test_image = canvas->register_rgba_image(50, 50, &pixels[0]);
            }

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
                Canvas::native_color_t red   = canvas->rgba_to_native({ 1, 0, 0, 1 });
                Canvas::native_color_t green = canvas->rgba_to_native({ 0, 1, 0, 1 });
                Canvas::native_color_t blue  = canvas->rgba_to_native({ 0, 0, 1, 1 });
                Canvas::native_color_t white = canvas->rgba_to_native({ 1, 1, 1, 1 });

                canvas->fill_rect(50, 50, 50, 50, red);
                canvas->fill_rect(100, 50, 50, 50, green);
                canvas->fill_rect(50, 100, 50, 50, blue);
                canvas->fill_rect(100, 100, 50, 50, white);
            }

            Canvas *canvas;
            typename Canvas::font_handle_t font;
            typename Canvas::image_handle_t test_image;

            void draw_images(int x, int y)
            {
                static const int SEP = 25;

                canvas->draw_image(x, y, 50, 50, test_image); x += 50;
                x += SEP;
                canvas->draw_image(x, y, 75, 75, test_image); x += 75;
                x += SEP;
                canvas->set_clipping_rect(x+5, y+5, 40, 40);
                canvas->draw_image(x, y, 50, 50, test_image); x += 50;
                canvas->cancel_clipping();
                x += SEP;
                canvas->draw_image(x, y, 50, 50, test_image, 10, 10); x += 50;
                x += SEP;
                canvas->draw_image(x, y, 50, 50, test_image, 20, 20); x += 50;
            }
        };

    } // ns gui

} // ns gpc