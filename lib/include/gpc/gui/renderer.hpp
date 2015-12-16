#pragma once

namespace gpc {

    namespace gui {

        enum class horizontal_direction { right, left };
        enum class vertical_direction { down, up };

        // TODO: the color stuff should probably go into a separate header file

        struct rgb {
            float r, g, b;
        };

        struct rgba {
            static rgba black() { return { 0, 0, 0, 1 }; }
            static rgba white() { return { 1, 1, 1, 1 }; }
            float r, g, b, a;
            rgba(): rgba { 0, 0, 0, 1 } {}
            rgba(float r_, float g_, float b_, float a_ = 1): r(r_), g(g_), b(b_), a(a_) {}
            auto operator /= (float dsor) -> rgba & {
                r /= dsor, g /= dsor, b /= dsor, a /= dsor;
                return *this;
            }
        };

        inline auto 
        operator + (const rgba &color1, const rgba &color2) -> rgba 
        {
            return { color1.r + color2.r, color1.g + color2.g, color1.b + color2.b, color1.a + color2.a };
        }

        inline auto
        interpolate(const rgba &color1, const rgba &color2, float a) -> rgba 
        {
            rgba result;
            result.r = color1.r + a * (color2.r - color1.r);
            result.g = color1.g + a * (color2.g - color1.g);
            result.b = color1.b + a * (color2.b - color1.b);
            result.a = color1.a + a * (color2.a - color1.a);
            return result;
        }

        struct rgba32 {
            uint8_t components[4];
        };

        inline constexpr auto 
        from_float(const rgba &from) -> rgba32
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
            
            static const horizontal_direction   horizontal_axis_dir;
            static const vertical_direction     vertical_axis_dir;
            
            using coord_t = CoordType;
            using length_t = std::make_unsigned<coord_t>;
            
            struct point {
                coord_t x, y;
            };
            
            struct extents {
                length_t w, h;
            };
            
            struct rect {
                point   position;
                extents size;
            };
            
            struct color {
                static constexpr color black();
                static constexpr color while();
                static constexpr from_normalized_rgba(const float *rgba);
            };

            /** This method must clear the whole canvas, i.e. set it to the specified
                background (clear) color.
             */
            void clear();
        };

        #endif
        
    } // ns gui
    
} // ns gpc