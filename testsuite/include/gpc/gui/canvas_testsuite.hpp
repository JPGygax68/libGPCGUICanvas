#pragma once

#include <functional>
#include <iostream>
#include <chrono>
#include <thread>

#include <boost/concept_check.hpp>
#include <cereal/archives/binary.hpp>

#include <gpc/fonts/RasterizedFont.hpp>
#include <gpc/fonts/cereal.hpp>

#include "fonts.hpp"

namespace gpc {

    namespace gui {
    
        template <
            class Canvas,                   // CanvasBackend implementation
            typename DisplayHandle,         // type returned by display creator function
            class DisplayDriver
        >
        class CanvasTestSuite {
        public:

            typedef Canvas                                      canvas_t;
            typedef DisplayHandle                               display_t;
            typedef DisplayDriver                               display_driver_t;
            typedef std::pair<display_t, canvas_t*>             context_t;
            typedef std::function<void(display_t, canvas_t*)>   init_fn_t;
            typedef std::function<void(display_t, canvas_t*)>   cleanup_fn_t;
            typedef std::function<void(display_t, canvas_t*)>   draw_fn_t;

            class TestCase {
            public:

                TestCase() :  suite(nullptr), display_driver(nullptr) {}

                void init(CanvasTestSuite *suite_, DisplayDriver *driver_) 
                {
                    suite = suite_;
                    display_driver = driver_;
                }

                bool run()
                {
                    using namespace std;

                    cout << endl << intro_message() << endl;
                    cout << "Press ENTER to start test" << endl;
                    wait_for_return();

                    typename init_fn_t init_fn = [&](display_t disp, canvas_t *canvas) { init_display(disp, canvas); };
                    typename draw_fn_t draw_fn = [&](display_t disp, canvas_t *canvas) { draw_content(disp, canvas); };

                    context_t ctx = display_driver->create_window(500, 400, init_fn, draw_fn);

                    display_driver->present_window(ctx.first, ctx.second, draw_fn);

                    //std::this_thread::sleep_for(std::chrono::milliseconds { 5000 });

                    display_driver->destroy_window(ctx.first);

                    return check_if_correct();
                }

            protected:

                // Standard implementation of the output checking function
                virtual bool check_if_correct()
                {
                    cout << std::endl << checking_question() << " " << std::endl;
                    std::string answer;
                    std::cin >> answer;
                    return answer == "y" || answer == "Y";
                }

                virtual auto intro_message() const -> std::string = 0;

                virtual auto checking_question() const -> std::string = 0;

                virtual void init_display(display_t display, canvas_t *canvas) {}

                virtual void cleanup_display(display_t display, canvas_t *canvas) {}

                virtual void draw_content(display_t display, canvas_t *canvas) = 0;

            private:

                void wait_for_return() { std::cin.ignore(200, '\n'); }

                CanvasTestSuite     *suite;
                DisplayDriver             *display_driver;
            };
        
            class Test_fill_rect : public TestCase {
            public:

                auto intro_message() const -> std::string override { 
                    return "This will display 4 squares in red, green, blue, and white; each 150x150 pixels in size, arranged in a square "
                        "with 10 pixels of separation between the two rows and columns."; 
                }

                auto checking_question() const -> std::string override { 
                    return "Did you see the 4 squares? (y/n)"; 
                }

                void draw_content(display_t display, canvas_t *canvas) override
                {
                    int x = 50, y = 50;

                    canvas->fill_rect(50, y, 150, 150, canvas->rgb_to_native({ 1, 0, 0 }));
                    canvas->fill_rect(50 + 150 + 10, y, 150, 150, canvas->rgb_to_native({ 0, 1, 0 }));
                    y += 150 + 10;
                    canvas->fill_rect(50, y, 150, 150, canvas->rgb_to_native({ 0, 0, 1 }));
                    canvas->fill_rect(50 + 150 + 10, y, 150, 150, canvas->rgb_to_native({ 1, 1, 1 }));
                }

            };
           
            class Test_draw_image : public TestCase {
            public:

                static const int WIDTH = 170, HEIGHT = 130;

                auto intro_message() const -> std::string override {

                    return "This will display a single image in multiple ways:\n"
                        "- display once, as-is\n"
                        "- repeated both horizontally and vertically"
                        "- clipped\n"
                        "- with an offset (i.e. leaving out image pixels from the left and the top";
                }

                auto checking_question() const -> std::string override {

                    return "Did you see the images as described? (y/n)";
                }

                void init_display(display_t display, canvas_t *canvas) override
                {
                    auto image_pixels = makeColorInterpolatedRectangle(WIDTH, HEIGHT, { { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1, 1, 1 } } });

                    image_handle = canvas->register_rgba_image(WIDTH, HEIGHT, &image_pixels[0]);
                }

                void cleanup_display(display_t display, canvas_t *canvas) override
                {
                    // TODO: dispose of ("unregister") image
                }

                virtual void draw_content(display_t display, canvas_t *canvas) override
                {
                    static const int SEPARATION = 10;

                    int x = 50, y = 50, w, h;

                    // Single image
                    canvas->draw_image(x, y, 170, 130, image_handle);
                    x += 170 + SEPARATION;
                    // Repeated image
                    w = 2 * 170 + 8, h = 2 * 130 + 5;
                    canvas->draw_image(x, y, w, h, image_handle);
                    // Repeated, with clipping
                    x += w + SEPARATION;
                    canvas->set_clipping_rect(x + 20, y + 20, w - 40, h - 40);
                    canvas->draw_image(x, y, w, h, image_handle);
                    canvas->cancel_clipping();
                    // Image with offset
                    x += w + 20;
                    canvas->draw_image(x, y, w, h, image_handle, 20, 20);
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

                typename canvas_t::image_handle_t image_handle;
            };

            class Test_draw_text : public TestCase {
            public:

                auto intro_message() const -> std::string override {

                    return "This will display two text strings:\n"
                        "- the first will be display just as-is\n"
                        "- the second should be clipped on all 4 sides";
                }

                auto checking_question() const -> std::string override {
                    return "Did you see the text, both as-is and clipped? (y/n)";
                }

                void init_display(display_t display, canvas_t *canvas) override
                {
                    static char liberations_sans_data[] = {
                        #include "LiberationSans-Regular-20.rft.h"
                    };

                    std::string data_string(liberations_sans_data, liberations_sans_data + sizeof(liberations_sans_data));
                    std::stringstream sstr(data_string);
                    cereal::BinaryInputArchive ar(sstr);

                    ar >> rfont;

                    font = canvas->register_font(rfont);
                }

                void cleanup_display(display_t display, canvas_t *canvas) override
                {
                    // TODO: dispose of ("unregister") font
                }

                virtual void draw_content(display_t display, canvas_t *canvas) override
                {
                    int x = 50, y = 50;

                    // Ascent (estimated) = 15; TODO: correct for top-down, but for bottom-up, descent should be used
                    canvas->draw_text(font, x, y + 15, "ABCDEFabcdef,;", 14);
                    canvas->fill_rect(x, y + 15 - 1, x + 150, 1, canvas->rgba_to_native({ 1, 0, 0, 0.5f }));
                    y += 20;
                    // With clipping
                    y += 10;
                    canvas->set_clipping_rect(x + 5, y + 3, 100, 20 - 3 - 3);
                    canvas->draw_text(font, x, y + 15, "Clipping clipping clipping", 26);
                    canvas->cancel_clipping();
                }

            private:
                gpc::fonts::RasterizedFont rfont;
                typename canvas_t::font_handle_t font;
            };

        public:

            CanvasTestSuite()
            {
                display_driver = new DisplayDriver();

                tests.push_back(new Test_fill_rect ());
                tests.push_back(new Test_draw_image());
                tests.push_back(new Test_draw_text ());
            }

            ~CanvasTestSuite()
            {
                delete display_driver;
            }

            void update_canvas_size(int w, int h)
            {
                //canvas_t::
            }

            bool run_all_tests()
            {
                display_driver->init();

                for(auto test: tests) { test->init(this, display_driver); }

                size_t total_count = 0, ok_count = 0;
                for (auto test: tests) {
                    total_count ++;
                    if (test->run()) ok_count ++;
                }

                return ok_count == total_count;
            }

        private:

            DisplayDriver *display_driver;
            std::vector<TestCase*> tests;
        };

    } // ns gui

} // ns gpc