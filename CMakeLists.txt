cmake_minimum_required(VERSION 3.10)
if (WIN32)
#why this has to be here is SUPER annoying
cmake_policy(SET CMP0091 NEW) #required for MSVC runtime
endif()

project (clip)

if (NOT PATH_TO_IDL) 
set(PATH_TO_IDL "../hl-idl")
endif()

# a bunch of pre-configured options that may or may not be useful
# it attempts to cover most of what is used for basic libraries
# you can reference the paths defined here to help make your cmake code more portable
include(${PATH_TO_IDL}/h-idl-defines.cmake)
#### Project definitions


set( PROJECT_ADDITIONAL_SOURCES 
"ext/Clipper2/CPP/Clipper2Lib/src/clipper.engine.cpp"
"ext/Clipper2/CPP/Clipper2Lib/src/clipper.offset.cpp"
"ext/Clipper2/CPP/Clipper2Lib/src/clipper.rectclip.cpp"
"ext/polypartition/src/polypartition.cpp"
)

set (PROJECT_ADDITIONAL_INCLUDES
"ext/Clipper2/CPP/Clipper2Lib/include"
"ext/polypartition/src"
)

set (PROJECT_ADDITIONAL_LIB_DIRS
)

set(PROJECT_ADDITIONAL_LIBS
)



include(${PATH_TO_IDL}/h-idl-configure.cmake)