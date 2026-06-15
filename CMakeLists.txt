cmake_minimum_required(VERSION 3.20)

# CMake project lives in code/; repository root is one level up.
get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectVersion)
include(DefaultPaths)

project(objectRecoStudy
    VERSION ${OBJECTRECOSTUDY_VERSION}
    DESCRIPTION "MFC object recognition and segmentation study"
    LANGUAGES CXX RC
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories (relative to repository root).
set(RUNTIME_OUTPUT_DIR "${PROJECT_ROOT_DIR}/msvc_release" CACHE PATH "Executable output directory")

# MFC shared DLL
set(CMAKE_MFC_FLAG 2)

if(MSVC)
    add_compile_options(/W3 /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX)
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo /l0x0804")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO")
endif()

# --- OpenCV ---
set(OpenCV_DIR "${OPENCV_DIR}")
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs dnn videoio)

# --- Generated version header ---
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/Version.h"
    @ONLY
)

set(OBJECTRECOSTUDY_SOURCES
    CaffeModelDialog.cpp
    classicalBayessianDlg.cpp
    ClassicalLinearDlg.cpp
    ClassicalStudy.cpp
    dnn_02_caffe_example_mobilenet.cpp
    mask_rcnn.cpp
    objectRecoStudy.cpp
    objectRecoStudyDlg.cpp
    ProjectPaths.cpp
    UiStrings.cpp
    UiHelpers.cpp
    ssdModelStudy.cpp
    stdafx.cpp
    TensorflowModelStudy.cpp
    TorchModelStudy.cpp
    objectRecoStudy.rc
)

add_executable(objectRecoStudy WIN32 ${OBJECTRECOSTUDY_SOURCES})

target_include_directories(objectRecoStudy PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
    "${OpenCV_INCLUDE_DIRS}"
    "${EIGEN3_INCLUDE_DIR}"
)

target_compile_definitions(objectRecoStudy PRIVATE
    UNICODE
    _UNICODE
    OBJECTRECOSTUDY_VERSION="${OBJECTRECOSTUDY_VERSION}"
    OBJECTRECOSTUDY_VERSION_MAJOR=${OBJECTRECOSTUDY_VERSION_MAJOR}
    OBJECTRECOSTUDY_VERSION_MINOR=${OBJECTRECOSTUDY_VERSION_MINOR}
    OBJECTRECOSTUDY_VERSION_PATCH=${OBJECTRECOSTUDY_VERSION_PATCH}
)

target_precompile_headers(objectRecoStudy PRIVATE stdafx.h)

target_link_libraries(objectRecoStudy PRIVATE ${OpenCV_LIBS})

if(MSVC)
    set_target_properties(objectRecoStudy PROPERTIES
        VS_GLOBAL_CharacterSet "Unicode"
        RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RUNTIME_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RUNTIME_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${RUNTIME_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${RUNTIME_OUTPUT_DIR}"
        PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/pdb"
    )
    # Avoid CMake setting ObjectFileName=$(IntDir) which overwrites all .obj files.
    set_property(TARGET objectRecoStudy PROPERTY VS_OBJECT_FILE_NAME "$(IntDir)%(Filename).obj")
endif()

# --- Post-build: sync runtime data ---
set(_input_dir "${RUNTIME_OUTPUT_DIR}/input")
add_custom_command(TARGET objectRecoStudy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_input_dir}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_input_dir}/models"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_ROOT_DIR}/output"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_ROOT_DIR}/models"
        "${_input_dir}/models"
    COMMENT "Sync models and input directory to runtime folder"
)

# Copy default media placeholders (optional; user may replace files).
if(EXISTS "${PROJECT_ROOT_DIR}/msvc_release/input/people.jpeg")
    # already present
elseif(EXISTS "D:/k8/media_images/xi_an_hot/people.jpeg")
    add_custom_command(TARGET objectRecoStudy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "D:/k8/media_images/xi_an_hot/people.jpeg"
            "${_input_dir}/people.jpeg"
    )
endif()

# --- Install ---
include(GNUInstallDirs)
install(TARGETS objectRecoStudy RUNTIME DESTINATION bin)
install(DIRECTORY "${PROJECT_ROOT_DIR}/models/"
    DESTINATION share/objectRecoStudy/models
    FILES_MATCHING
        PATTERN "*.pbtxt"
        PATTERN "*.names"
        PATTERN "*.txt"
        PATTERN "*.pb"
)
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/BUILD.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/RUN.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/algorithms.md"
    DESTINATION share/objectRecoStudy/docs
    OPTIONAL
)

message(STATUS "objectRecoStudy ${OBJECTRECOSTUDY_VERSION}")
message(STATUS "  PROJECT_ROOT : ${PROJECT_ROOT_DIR}")
message(STATUS "  RUNTIME_DIR  : ${RUNTIME_OUTPUT_DIR}")
message(STATUS "  OpenCV       : ${OpenCV_DIR}")
