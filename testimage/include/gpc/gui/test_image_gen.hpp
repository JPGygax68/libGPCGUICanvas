#pragma once

#include <codecvt>

//#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/cereal.hpp>

#include <gpc/gui/renderer.hpp> // TODO: replace with specific header file for color handling when available

namespace gpc {

    namespace gui {
    
        template <
            typename Renderer 
        >
        class TestImageGenerator {
        public:

            static const int WIDTH = 1200, HEIGHT = 675;

            static auto utf8toucs4(const std::string &from) -> std::u32string
            {
                static std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> converter;

                std::basic_string<int32_t> temp = converter.from_bytes(from);

                auto start = reinterpret_cast<const char32_t*>(&temp[0]);
                auto end = start + temp.size();

                return std::u32string(start, end);
            }


            TestImageGenerator(): renderer(nullptr) {}

            void init(Renderer *canvas_)
            {
                renderer = canvas_;

                register_colors();
                register_fonts();
                register_test_image();
            }

            auto generate() -> std::vector<rgba>
            {
                std::vector<rgba> img;

                renderer->clear(renderer->rgba_to_native({ 0.8f, 0.8f, 0.8f, 0 }));

                draw_grid();
                draw_plain_rects();
                draw_images(200, 50);
                render_text(50, 200);

                return img;
            }

        private:

            static auto
            makeColorInterpolatedRectangle(size_t width, size_t height, const std::array<rgba, 4> &corner_colors) -> std::vector<rgba>
            {
                std::vector<rgba> image(width * height);

                auto it = image.begin();
                for (auto y = 0U; y < height; y++) {
                    for (auto x = 0U; x < width; x++) {
                        rgba top    = interpolate(corner_colors[0], corner_colors[1], float(x) / float(width));
                        rgba bottom = interpolate(corner_colors[2], corner_colors[3], float(x) / float(width));
                        rgba color  = interpolate(top, bottom, float(y) / float(height));
                        //*it = rgba32{ static_cast<uint8_t>(255 * color.r), static_cast<uint8_t>(255 * color.g), static_cast<uint8_t>(255 * color.b), 255 };
                        *it = color;
                        //*it = { {255, 0, 0, 255} };
                        it++;
                    }
                }

                return image;
            }

            void register_colors()
            {
               red = renderer->rgba_to_native({ 1, 0, 0, 1 });
               green = renderer->rgba_to_native({ 0, 1, 0, 1 });
               blue = renderer->rgba_to_native({ 0, 0, 1, 1 });
               white = renderer->rgba_to_native({ 1, 1, 1, 1 });
               grey = renderer->rgba_to_native({ 0.5f, 0.5f, 0.5f, 1});
            }

            void register_fonts()
            {
                static const uint8_t sans_reg_16[] = {
                    #include "LiberationSans-Regular-16.rft.h"
                };

                std::string data_string(sans_reg_16, sans_reg_16 + sizeof(sans_reg_16));
                std::stringstream sstr(data_string);
                cereal::BinaryInputArchive ar(sstr);
                gpc::fonts::rasterized_font rfont;
                ar >> rfont;

                font = renderer->register_font(rfont);

                // TODO: dispose of fonts
            }

            void register_test_image()
            {
                auto pixels = makeColorInterpolatedRectangle(50, 50, { { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1, 1, 1 } } });
                test_image = renderer->register_rgba_image(50, 50, &pixels[0]);
            }

            void draw_grid()
            {
                static const int LINE_WIDTH = 1;

                Renderer::native_color before = renderer->rgba_to_native({0, 0, 0, 1});
                Renderer::native_color after = renderer->rgba_to_native({1, 1, 1, 1});

                // Vertical axis
                for (int y = 0; y <= HEIGHT; y += 50) {
                    renderer->fill_rect(0, y - LINE_WIDTH, WIDTH, LINE_WIDTH, before);
                    renderer->fill_rect(0, y, WIDTH, LINE_WIDTH, after);
                    auto label = utf8toucs4(std::to_string(y));
                    renderer->render_text(font, 4, y - 4, label.c_str(), label.size());
                }
                // Horizontal axis
                for (int x = 0; x <= WIDTH; x += 50) {
                    renderer->fill_rect(x - LINE_WIDTH, 0, LINE_WIDTH, HEIGHT, before);
                    renderer->fill_rect(x, 0, LINE_WIDTH, HEIGHT, after);
                    auto label = utf8toucs4(std::to_string(x));
                    renderer->render_text(font, x+4, 18, label.c_str(), label.size());
                }
            }

            void draw_plain_rects()
            {
                renderer->fill_rect(50, 50, 50, 50, red);
                renderer->fill_rect(100, 50, 50, 50, green);
                renderer->fill_rect(50, 100, 50, 50, blue);
                renderer->fill_rect(100, 100, 50, 50, white);
            }

            void draw_images(int x, int y)
            {
                static const int SEP = 25;

                renderer->draw_image(x, y, 50, 50, test_image); x += 50;
                x += SEP;
                renderer->draw_image(x, y, 75, 75, test_image); x += 75;
                x += SEP;
                renderer->fill_rect(x, y, 50, 50, grey);
                renderer->set_clipping_rect(x+5, y+5, 40, 40);
                renderer->draw_image(x, y, 50, 50, test_image); x += 50;
                renderer->cancel_clipping();
                x += SEP;
                renderer->draw_image(x, y, 50, 50, test_image, 10, 10); x += 50;
                x += SEP;
                renderer->draw_image(x, y, 50, 50, test_image, 20, 20); x += 50;
            }

            void render_text(int x, int y)
            {
                static const int SEP = 25;

                renderer->set_text_color(renderer->rgba_to_native({0, 0, 0, 1}));
                renderer->render_text(font, x, y, U"Some black text.", 16); x += 200;
                renderer->set_text_color(renderer->rgba_to_native({ 0.5f, 0, 0, 1 }));
                renderer->render_text(font, x, y, U"Now some RED text.", 18); x += 200;
                renderer->set_text_color(renderer->rgba_to_native({ 0., 0, 0, 0.5f }));
                renderer->render_text(font, x, y, U"Half-transparent text.", 21); x += 200;
            }

            Renderer *renderer;
            typename Renderer::font_handle font;
            typename Renderer::image_handle test_image;
            typename Renderer::native_color red, green, blue, white, grey;
        };

    } // ns gui

} // ns gpc