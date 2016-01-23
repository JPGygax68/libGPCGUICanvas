#pragma once

#include <array>

namespace gpc {

    namespace gui {

        enum class horizontal_direction { right, left };
        enum class vertical_direction { down, up };

        // TODO: the color stuff should probably go into a separate header file

        /** Normalized RGBA (red, green, blue, alpha) color.
         */
        struct rgba_norm {

            // Predefined colors
            static constexpr const rgba_norm black() { return { 0, 0, 0, 1 }; }
            static constexpr const rgba_norm white() { return { 1, 1, 1, 1 }; }

            // Constructors
            constexpr rgba_norm(): rgba_norm { 0, 0, 0, 1 } {}
            constexpr rgba_norm(float r, float g, float b, float a = 1): components { r, g, b, a } {}

            // Accessors
            constexpr float r() const { return components[0]; }
            constexpr float g() const { return components[1]; }
            constexpr float b() const { return components[2]; }
            constexpr float a() const { return components[3]; }
            float& r() { return components[0]; }
            float& g() { return components[1];  }
            float& b() { return components[2]; }
            float& a() { return components[3]; }
            // Not really an accessor, but allows usage in calls to OpenGL
            constexpr operator const float * () const { return &components[0]; }

            // Operations
            auto operator /= (float dsor) -> rgba_norm & {
                components[0] /= dsor, components[1] /= dsor, components[2] /= dsor, components[3] /= dsor;
                return *this;
            }

            // Data
            std::array<float, 4> components;
        };

        inline auto 
        operator + (const rgba_norm &color1, const rgba_norm &color2) -> rgba_norm 
        {
            return { color1.r() + color2.r(), color1.g() + color2.g(), color1.b() + color2.b(), color1.a() + color2.a() };
        }

        inline auto
        interpolate(const rgba_norm &color1, const rgba_norm &color2, float a) -> rgba_norm 
        {
            rgba_norm result;
            result.r() = color1.r() + a * (color2.r() - color1.r());
            result.g() = color1.g() + a * (color2.g() - color1.g());
            result.b() = color1.b() + a * (color2.b() - color1.b());
            result.a() = color1.a() + a * (color2.a() - color1.a());
            return result;
        }

        struct rgba32 {
            uint8_t components[4];
        };

        inline constexpr auto 
        from_float(const rgba_norm &from) -> rgba32
        {
            return { { uint8_t(from.r() * 255), uint8_t(from.g() * 255), uint8_t(from.b() * 255), uint8_t(from.a() * 255) } };
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
                static constexpr from_normalized_rgba(const float *rgba_norm);
            };

            /** This method must clear the whole canvas, i.e. set it to the specified
                background (clear) color.
             */
            void clear();
        };

        #endif
        
    } // ns gui
    
} // ns gpc