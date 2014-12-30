#pragma once

#include <functional>
#include <iostream>

#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/RasterizedFont.hpp>
#include <gpc/fonts/cereal.hpp>

#include "fonts.hpp"

namespace gpc {

    namespace gui {
    
        template <
            class BackendImpl,              // CanvasBackend implementation
            typename DisplayHandle          // type returned by display creator function
        >
        class CanvasTestsuite {
        public:

            typedef BackendImpl canvas_t;
            typedef DisplayHandle display_t;

            typedef std::pair<display_t, canvas_t*> context_t;

            typedef std::function<void(display_t, canvas_t)> draw_fn_t;

            typedef std::function<context_t(int, int, draw_fn_t)> display_creator_fn_t;
            typedef std::function<void(display_t)> display_destructor_fn_t;

            CanvasTestsuite(display_creator_fn_t dc, display_destructor_fn_t dd) :
                display_creator(dc), display_destructor(dd)
            {
            }

            void update_canvas_size(int w, int h)
            {
                //canvas_t::
            }

            bool run_all_tests()
            {
                init();

                size_t total_count = 0, ok_count = 0;

                total_count++;
                if (test_fillrect()) ok_count ++;

                total_count++;
                if (test_image()) ok_count++;

                total_count++;
                if (test_text()) ok_count++;

                return ok_count == total_count;
            }

            bool test_fillrect()
            {
                // TODO: display explanatory message to user

                auto display = create_display(0, 0, [&](display_t disp, canvas_t &canvas) {

                    int x = 50, y = 50;

                    canvas.fill_rect(50, y, 150, 150, canvas.rgb_to_native({ 1, 0, 0 }));
                    canvas.fill_rect(50 + 150 + 10, y, 150, 150, canvas.rgb_to_native({ 0, 1, 0 }));
                    y += 150 + 10;
                    canvas.fill_rect(50, y, 150, 150, canvas.rgb_to_native({ 0, 0, 1 }));
                    canvas.fill_rect(50 + 150 + 10, y, 150, 150, canvas.rgb_to_native({ 1, 1, 1 }));
                });

                // TODO: wait 5 seconds

                // TODO: ask user about result

                return false;
            }

            bool test_image()
            {
                // TODO: display explanatory message to user

                auto display = create_display(0, 0, [&](display_t disp, canvas_t &canvas) {

                    int x = 50, y = 50, w, h;

                    // Single image
                    canvas.draw_image(x, y, 170, 130, test_image_handle);
                    x += 170 + SEPARATION;
                    // Repeated image
                    w = 2 * 170 + 8, h = 2 * 130 + 5;
                    canvas.draw_image(x, y, w, h, test_image_handle);
                    // Repeated, with clipping
                    x += w + SEPARATION;
                    canvas.set_clipping_rect(x + 20, y + 20, w - 40, h - 40);
                    canvas.draw_image(x, y, w, h, test_image_handle);
                    canvas.cancel_clipping();
                    // Image with offset
                    x += w + 20;
                    canvas.draw_image(x, y, w, h, test_image_handle, 20, 20);
                });

                // TODO: wait 5 seconds

                // TODO: ask user about result

                return false;
            }

            bool test_text()
            {
                // TODO: display explanatory message to user

                auto display = create_display(0, 0, [&](display_t disp, canvas_t &canvas) {

                    int x = 50, y = 50;

                    // Ascent (estimated) = 15; TODO: correct for top-down, but for bottom-up, descent should be used
                    canvas.draw_text(my_font, x, y + 15, "ABCDEFabcdef,;", 14);
                    canvas.fill_rect(x, y + 15 - 1, x + 150, 1, canvas.rgba_to_native({ 1, 0, 0, 0.5f }));
                    y += 20;
                    // With clipping
                    y += 10;
                    canvas.set_clipping_rect(x + 5, y + 3, 100, 20 - 3 - 3);
                    canvas.draw_text(my_font, x, y + 15, "Clipping clipping clipping", 26);
                    canvas.cancel_clipping();
                });

                // TODO: wait 5 seconds

                // TODO: ask user about result

                return false;
            }

        private:

            static auto 
            makeColorInterpolatedRectangle(size_t width, size_t height, const std::array<RGBAFloat, 4> &corner_colors)
                -> std::vector<RGBA32>
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

            void init()
            {
                static char liberations_sans_data[] = {
                    #include "LiberationSans-Regular-20.rft.h"
                };

                std::string data_string(liberations_sans_data, liberations_sans_data + sizeof(liberations_sans_data));
                std::stringstream sstr(data_string);
                cereal::BinaryInputArchive ar(sstr);
                ar >> rfont;
            }

            auto create_display(int w, int h, draw_fn_t draw_fn) 
                -> context_t
            {
                context_t ctx = display_creator(w, h, draw_fn);
                canvas.init();
                return ctx;
            }

            display_creator_fn_t    display_creator;
            display_destructor_fn_t display_destructor;
            gpc::fonts::RasterizedFont rfont;
        };

    } // ns gui

} // ns gpc