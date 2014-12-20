#pragma once

namespace gpc {

    namespace gui {

        enum class HorizontalDirection { RIGHT, LEFT };
        enum class VerticalDirection { DOWN, UP };

        /** This is a Concept and not supposed to compile. Do not use this template directly.
        
            This concept describes what a GPC GUI Canvas is supposed to be able to do.
         */
        template <
            VerticalDirection VertAxisDir,
            typename CoordType
        >
        class Canvas {
        public:
            
            static const HorizontalDirection    horizontal_axis_dir;
            static const VerticalDirection      vertical_axis_dir;
            
            typename CoordType coord_t;
            typename std::make_unsigned<CoordType>  length_t;
            
            struct Point {
                position_t x, y;
            };
            
            struct Extents {
                length_t w, h;
            };
            
            struct Rect {
                Point position;
                Size size;
            };
            
            struct Color {
                static Color BLACK();
                static Color WHITE();
                static Color fromNormalizedRGBA(const float *rgba);
            };

            /** This method must clear the whole canvas, i.e. set it to the specified
                background (clear) color.
             */
            void clear();
        };

    } // ns gui
    
} // ns gpc