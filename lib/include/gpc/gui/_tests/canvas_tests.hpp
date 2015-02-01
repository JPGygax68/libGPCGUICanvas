#pragma once

namespace gpc {

    namespace gui {
    
        namespace _tests {
        
            typedef (*CheckFunc)(bool value, const char *description = nullptr);
            
            template <typename Canvas>
            struct Canvas_tests {
            
                static void test_all(Canvas &canvas, CheckFunc *check_fn)
                {
                    // TODO
                }
                
            }
            
        } // ns tests
        
    } // ns gui
    
} // ns gpc