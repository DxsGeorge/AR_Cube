# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.


set(pathToProject C:/Users/user/AndroidStudioProjects/AR_Cube)
set(pathToOpencv C:/Users/user/Desktop/opencv-3.4.1-android-sdk/OpenCV-android-sdk)

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${pathToOpencv}/sdk/native/jni/include
                    ${pathToProject}/app/src/main/cpp
                    ${pathToProject}/app/src/main/cpp/kociemba)

add_library( native-lib
             SHARED
             src/main/cpp/MyUtils.cpp
             src/main/cpp/ImageFilters.cpp
             src/main/cpp/native-lib.cpp
             src/main/cpp/Combinatorics.cpp
             src/main/cpp/Cube.cpp
             src/main/cpp/CubeParser.cpp
             src/main/cpp/FaceletCube.cpp
             src/main/cpp/MoveTable.cpp
             src/main/cpp/PruningTable.cpp
             src/main/cpp/RubiksCube.cpp
             src/main/cpp/Solver.cpp
             src/main/cpp/Vector.cpp
             src/main/cpp/Input.cpp
                )

add_library( lib_opencv SHARED IMPORTED )

set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathToProject}/app/src/main/jniLibs/${ANDROID_ABI}/libopencv_java3.so)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                       lib_opencv
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )