#pragma once

namespace gpc {

    namespace gui {

        enum class HorizontalDirection { RIGHT, LEFT };
        enum class VerticalDirection { DOWN, UP };

        // TODO: the color stuff should probably go into a separate header file

        struct RGBFloat {
            GLfloat r, g, b;
        };

        struct RGBAFloat {
            static const RGBAFloat BLACK() { return { 0, 0, 0, 1 }; }
            static const RGBAFloat WHITE() { return { 1, 1, 1, 1 }; }
            GLfloat r, g, b, a;
            RGBAFloat(): RGBAFloat { 0, 0, 0, 1 } {}
            RGBAFloat(GLfloat r_, GLfloat g_, GLfloat b_, GLfloat a_ = 1): r(r_), g(g_), b(b_), a(a_) {}
            auto operator / (float dsor) -> RGBAFloat & {
                r /= dsor, g /= dsor, b /= dsor, a /= dsor;
                return *this;
            }
        };

        inline auto 
        operator + (const RGBAFloat &color1, const RGBAFloat &color2) -> RGBAFloat 
        {
            return { color1.r + color2.r, color1.g + color2.g, color1.b + color2.b, color1.a + color2.a };
        }

        inline auto
        interpolate(const RGBAFloat &color1, const RGBAFloat &color2, float a) -> RGBAFloat 
        {
            RGBAFloat result;
            result.r = color1.r + a * (color2.r - color1.r);
            result.g = color1.g + a * (color2.g - color1.g);
            result.b = color1.b + a * (color2.b - color1.b);
            result.a = color1.a + a * (color2.a - color1.a);
            return result;
        }

        struct RGBA32 {
            uint8_t components[4];
        };

        inline auto 
        fromFloat(const RGBAFloat &from) -> RGBA32 
        {
            return { { uint8_t(from.r * 255), uint8_t(from.g * 255), uint8_t(from.b * 255), uint8_t(from.a * 255) } };
        }

        // TODO: use Boost concept checking to define something usable here

        #ifdef NOT_DEFINED
        
        /** This is a Concept and not supposed to compile. Do not use this template directly.
        
            This concept describes what a GPC GUI Canvas is supposed to be able to do.
         */
        template <
            VerticalDirection VertAxisDir,
            typename CoordType
        >
        class Renderer {
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

        #endif
        
    } // ns gui
    
} // ns gpc