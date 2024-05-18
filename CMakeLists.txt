cmake_minimum_required(VERSION 3.22)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment target")
#set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(OneEuro VERSION 1.0.0 LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

find_package(JUCE CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

# generate dSYM files for the Release build
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym")
set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES")

if(MSVC)
add_compile_options("/W4")
else()
add_compile_options(
    -Wall
    -Wbool-conversion
    -Wcast-align
    -Wconditional-uninitialized
    -Wconstant-conversion
    -Wconversion
    -Wdeprecated
    -Wextra-semi
    -Wno-float-equal
    -Winconsistent-missing-destructor-override
    -Wint-conversion
    -Wmissing-field-initializers
    -Wmissing-prototypes
    -Wno-ignored-qualifiers
    -Wnullable-to-nonnull-conversion
    -Woverloaded-virtual
    -Wpedantic
    -Wreorder
    -Wshadow-all
    -Wshift-sign-overflow
    -Wshorten-64-to-32
    -Wsign-compare
    -Wno-sign-conversion
    -Wstrict-aliasing
    -Wswitch-enum
    -Wuninitialized
    -Wunreachable-code
    -Wunused-parameter
    -Wunused-private-field
    -Wzero-as-null-pointer-constant
)
endif()

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME "audiodev.blog"
    PLUGIN_MANUFACTURER_CODE Dvbl
    PLUGIN_CODE Euro
    BUNDLE_ID "blog.audiodev.one-euro"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "OneEuro"
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    src/OneEuroFilter.h
    src/Oversampler.h
    src/Parameters.cpp
    src/Parameters.h
    src/PluginEditor.cpp
    src/PluginEditor.h
    src/PluginProcessor.cpp
    src/PluginProcessor.h
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_MODAL_LOOPS_PERMITTED=0
    DONT_SET_USING_JUCE_NAMESPACE=1
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)
