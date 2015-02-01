#pragma once

#include "canvas.hpp"

namespace gpc {

    namespace gui {

        /** This is a Concept. Do not use directly.
        
            A Pixel Canvas is a Canvas that is based on pixels. While practically
            any type of display today is arguably based on pixels, a distinction
            can be made between displays where individual pixels can still be made 
            out (such as most current computer monitors), and displays where pixels
            no longer need to be considered individually. The latter could be the case 
            for actual so-called "retina" displays, but also for display pipelines
            that use anti-aliasing to hide individual pixels.

            The basic idea is to use the specialized concept of a Pixel Canvas for
            those displays that need to do any kind of "grid fitting", making it 
            possible to leave out that part in Canvas implementations that do not
            need it.
         */
        template <
            VerticalDirection VertAxisDir,
            typename CoordType // TODO: must be a signed integral type
        >
        class PixelCanvas: public Canvas<VertAxisDir, CoordType> {
        public:
            
            struct _RGB24 {
                uint8_t rgb[3];
            };
            
            typedef std::vector<RGB24> _RGB24Image;
            
            /** This function is intended for debugging purposes only. It must return
                the full content of the display as a two-dimensional bitmap. RGB triplets
                are arranged in top-to-bottom, left-to-right order regardless of the defined
                vertical axis direction.
                Each triplet is made of 3 unsigned bytes representing red, green, and blue 
                respectively (and in that order: the bytes are NOT to be treated as a single 
                24-bit integer).
                RGB24Image (see above) is a simple vector of RGB24 triplets. Each row must
                contain exactly as many triplets as the content rectangle is wide, and there 
                must be exactly as many rows as the content rectangle is high.
             */
            auto _getRGB24Screenshot() -> _RGB24Image;
            
        };

    } // ns gui
    
} // ns gpc