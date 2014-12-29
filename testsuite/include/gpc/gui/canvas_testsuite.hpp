#pragma once

#include <functional>
#include <iostream>

#include <boost/concept_check.hpp>

#include "fonts.hpp"

namespace gpc {

    namespace gui {
    
        template <
            class BackendImpl,              // CanvasBackend implementation
            typename DisplayHandle          // type returned by display creator function
        >
        class CanvasTestsuite {
        public:
            
            typedef DisplayHandle display_handle_t;

            typedef std::function<display_handle_t(int, int)> display_creator_fn_t;

            CanvasTestsuite(display_creator_fn_t dc) :
                display_creator(dc) 
            {
            }

            bool run_all_tests()
            {
                size_t total_count = 0, ok_count = 0;

                total_count++;
                if (test_fillrect()) ok_count ++;

                return ok_count == total_count;
            }

            bool test_fillrect()
            {

                return false;
            }

        private:
            display_creator_fn_t    display_creator;
        };

    } // ns gui

} // ns gpc