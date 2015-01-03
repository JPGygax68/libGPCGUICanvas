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
    
        namespace canvas {

            template <
                typename Canvas 
            >
            class TestImageGenerator {
            public:

                typedef Canvas                                      canvas_t;

            private:

            };

        } // ns canvas

    } // ns gui

} // ns gpc