#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

######################################################################
# SIMPLE BUILD SYSTEM
# Welcome to the CMake build system for SIMPLE.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6.2)
# Include necessary submodules

project (SIMPLE)

set ( SAGAENGINE ../../../.. )
set ( OGRE /usr/include/OGRE /usr/include/OGRE/Overlay )
set ( OIS /usr/include/OIS )
include_directories( ${INCLUDE_DIRECTORIES} ${OGRE} ${OIS} )

add_definitions(-DSE_STEP_SHIFT=4)
add_definitions(-DIS_OGRE)
add_definitions(-DDEBUG_LEVEL_2)
add_definitions(-D_SimpleLogicExport=)
add_definitions(-D_SimpleUiExport=)
add_definitions(-D_SeOgreExport=)
add_definitions(-D_SeBasicExport=)
add_definitions(-D_SeClientExport=)
add_definitions(-D_SePcExport=)
add_definitions(-D_SeCoreExport=)
add_definitions(-D_SeOdeExport=)
add_definitions(-D_SeEditorExport=)
add_definitions(-D_SeAudiereExport=)
add_definitions(-D_SePulseaudioExport=)
add_definitions(-D_NavMeshUiExport=)
add_definitions(-D_NavMeshLogicExport=)
add_definitions(-DSE_INTERNAL)
add_definitions(-DSE_STATIC)
add_definitions(-DNO_CEGUI)

# Internal
include(../cmake/modules/FindStart.cmake)
include(../cmake/modules/FindLogic.cmake)
include(../cmake/modules/FindOgreUi.cmake)

# SagaEngine
include(../cmake/modules/FindCore.cmake)
include(../cmake/modules/FindBasic.cmake)
include(../cmake/modules/FindClient.cmake)
include(../cmake/modules/FindPlatformPc.cmake)
include(../cmake/modules/FindOgre.cmake)
include(../cmake/modules/FindEditor.cmake)



add_executable (simple ${SIMPLE_START} ${SIMPLE_LOGIC} ${SIMPLE_UI} ${SE_CORE} ${SE_BASIC} ${SE_CLIENT} ${SE_PLATFORM} ${SE_OGRE} ${SE_PULSEAUDIO} ${SE_EDITOR})
# for ogre module
target_link_libraries(simple OgreMain OgreOverlay OIS)
# for pulseaudio module
target_link_libraries(simple pulse vorbisfile)
target_link_libraries(simple boost_system dl pthread)
