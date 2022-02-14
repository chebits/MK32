idf_component_register(SRCS "mk32_main.cpp" "keymap.c" "matrix.c" "keypress_handles.c" "key_definitions.c"
                    INCLUDE_DIRS "."
                    REQUIRES "battery" "ble" "oled" "nvsfuncs" "plugins" )
