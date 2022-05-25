
# V3DLib include dir
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project(v3dlib C CXX)
##############################################################################
# CXX Flags

SET(CMAKE_CXX_FLAGS_RELEASE "-DEIGEN_NO_DEBUG -DNDEBUG -DQPU_MODE")

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wno-unknown-pragmas \
       -Wno-sign-compare -Wno-unused-parameter -Wno-missing-field-initializers"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -DEIGEN_NO_DEBUG -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-Og -pg -g")
endif()

if(ANDROID)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  if(MKEDD_BENCHMARK)
    set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g")
  else()
    set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g -pg")
  endif()

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
  endif()
endif()

# Optimization flags
# string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(CMAKE_COMPILER_IS_GNUCXX AND (BUILD_TYPE STREQUAL "RELEASE"))
  if(CMAKE_CROSSCOMPILING)
    if((CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64") OR
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72-aarch64") OR # RaspberryPi 4
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72-aarch32") OR # RaspberryPi 4
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72"))           # RaspberryPi 4
    set(CMAKE_CXX_FLAGS_RELEASE
          "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -mcpu=cortex-a72 \
           -march=armv8-a+crc -mtune=cortex-a72 -ffast-math -fgcse-lm \
           -fgcse-sm -fgcse-las -fgcse-after-reload -Wno-psabi"
      )    
    elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "armv6l") OR
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm1176jzfs"))  # RaspberryPi 0
      set(CMAKE_CXX_FLAGS_RELEASE
          "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -march=armv6zk \
          -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -ffast-math \
          -funsafe-math-optimizations -fgcse-lm -fgcse-sm -fgcse-las \
          -fgcse-after-reload -Wno-psabi"
      )
    elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l") OR
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53-aarch64") OR # RaspberryPi 3
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53-aarch32") OR # RaspberryPi 3
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53"))           # RaspberryPi 3
	     set(CMAKE_CXX_FLAGS_RELEASE
	     "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -mcpu=cortex-a53 \
	     -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 \
	     -mfloat-abi=hard -ffast-math -fgcse-lm -fgcse-sm -fgcse-las \
	     -fgcse-after-reload -Wno-psabi")
    else()
      message(FATAL_ERROR "Unrecognized crosscompiler architecture, CMAKE_SYSTEM_PROCESSOR='${CMAKE_SYSTEM_PROCESSOR}'")
    endif()
  else()
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize \
        -march=native -mtune=native -ffast-math"
    )
  endif()
endif()



SET(V3DLIB "v3dlib")

SET(MESA_DIR ${CMAKE_CURRENT_LIST_DIR}/mesa)
INCLUDE_DIRECTORIES(${MESA_DIR}/include)
INCLUDE_DIRECTORIES(${MESA_DIR}/src)


SET(V3DLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/Lib)

INCLUDE_DIRECTORIES(${V3DLIB_DIR})
INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Common)

INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Kernels)
INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Liveness)

INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Support)

INCLUDE_DIRECTORIES(${V3DLIB_DIR}/v3d)

INCLUDE_DIRECTORIES(${V3DLIB_DIR}/vc4)

INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Target)
INCLUDE_DIRECTORIES(${V3DLIB_DIR}/Source)

file(
  GLOB
  V3D_SOURCES
  "Lib/*"
  "Lib/Common/*"
  "Lib/Kernels/*"
  "Lib/Liveness/*"
  "Lib/Support/*"
  "Lib/Target/*"
  "Lib/Target/instr/*"
  "Lib/Source/*"
  "Lib/v3d/*"
  "Lib/v3d/instr/*"
  "Lib/vc4/*"
  "Lib/vc4/DMA/*"
)

add_library(${V3DLIB}_static STATIC ${V3D_SOURCES})

message(STATUS "v3d set target properties = ${CMAKE_SOURCE_DIR}")

# include(ExternalProject)
# ExternalProject_Add( mesa
#   SOURCE_DIR ${MESA_DIR}
#   INSTALL_COMMAND cmake -E echo "Skipping install step."
#   CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
# )
# ExternalProject_Get_Property(mesa binary_dir)
# ExternalProject_Get_Property(mesa source_dir)

# set(MESA_LIBRARY "${binary_dir}/libmesa_static.a")
# set(MESA_INCLUDE_DIRS "${source_dir}/mesa/src/drm"
#                       "${source_dir}/mesa/include"
#                       "${source_dir}/mesa/include/drm-uapi"
#                       "${source_dir}/mesa/src"
#                       "${source_dir}/mesa/src/compiler"
#                       "${source_dir}/mesa/src/loader"
#                       "${source_dir}/mesa/src/broadcom"
#                       "${source_dir}/mesa/src/util"
#                       "${source_dir}/mesa/src/gallium/winsys"
#                       "${source_dir}/mesa/src/gallium/drivers"
#                       "${source_dir}/mesa/src/gallium/drivers/v3d"
#                       "${source_dir}/mesa/src/gallium/include"
#                       "${source_dir}/mesa/src/gallium/auxiliary"
#                       "${source_dir}/mesa/src/mesa")

# message (STATUS "mesa library path = ${MESA_LIBRARY}" )
# message (STATUS "mesa include path = ${MESA_INCLUDE_DIRS}" )


# add_dependencies(${V3DLIB}_static mesa)
# target_include_directories(${V3DLIB}_static PUBLIC ${MESA_INCLUDE_DIRS})
# target_link_libraries(${V3DLIB}_static PUBLIC ${MESA_LIBRARY})


set(XX_FLAGS "-MMD -MP -MF\"$(@:%.o=%.d)\"")
set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} \
      -std=c++17 -g -Wall -O3 -pthread \
      -D DEBUG \
      -D_GNU_SOURCE \
      -D MAJOR_IN_SYSMACROS \
      -D HAVE_PTHREAD \
      -D HAVE_TIMESPEC_GET \
      -D HAVE_ENDIAN_H \
      -D V3D_VERSION=33 \
      -D GALLIUM_STATIC_TARGETS \
      -D HAVE_LIBDRM \
      -D USE_DRICONF \
      -D GALLIUM_V3D \
      ${XX_FLAGS}"
)

set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS}  -g -Wall -O3 -pthread \
      -D DEBUG \
      -D_GNU_SOURCE \
      -D MAJOR_IN_SYSMACROS \
      -D HAVE_PTHREAD \
      -D HAVE_TIMESPEC_GET \
      -D HAVE_ENDIAN_H \
      -D V3D_VERSION=33 \
      -D GALLIUM_STATIC_TARGETS \
      -D HAVE_LIBDRM \
      -D USE_DRICONF \
      -D GALLIUM_V3D \
      ${XX_FLAGS}"
)

set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -DEIGEN_NO_DEBUG -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -pg -g")

set(CMAKE_C_FLAGS_RELEASE
    "${CMAKE_C_FLAGS_RELEASE} -DEIGEN_NO_DEBUG -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -pg -g")


# Optimization flags
# string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(CMAKE_COMPILER_IS_GNUCXX AND (BUILD_TYPE STREQUAL "RELEASE"))
  if(CMAKE_CROSSCOMPILING)
    if((CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64") OR
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72-aarch64") OR # RaspberryPi 4
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72-aarch32") OR # RaspberryPi 4
       (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a72"))           # RaspberryPi 4
    set(CMAKE_CXX_FLAGS_RELEASE
          "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -mcpu=cortex-a72 \
           -march=armv8-a+crc -mtune=cortex-a72 -ffast-math -fgcse-lm \
           -fgcse-sm -fgcse-las -fgcse-after-reload"
      )    
    elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "armv6l") OR
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm1176jzfs"))  # RaspberryPi 0
      set(CMAKE_CXX_FLAGS_RELEASE
          "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -march=armv6zk \
          -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -ffast-math \
          -funsafe-math-optimizations -fgcse-lm -fgcse-sm -fgcse-las \
          -fgcse-after-reload -Wno-psabi"
      )
    elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l") OR
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53-aarch64") OR # RaspberryPi 3
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53-aarch32") OR # RaspberryPi 3
           (CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-a53"))           # RaspberryPi 3
	     set(CMAKE_CXX_FLAGS_RELEASE
	     "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -mcpu=cortex-a53 \
	     -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 \
	     -mfloat-abi=hard -ffast-math -fgcse-lm -fgcse-sm -fgcse-las \
	     -fgcse-after-reload")
    else()
      message(FATAL_ERROR "Unrecognized crosscompiler architecture, CMAKE_SYSTEM_PROCESSOR='${CMAKE_SYSTEM_PROCESSOR}'")
    endif()
  else()
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize \
        -march=native -mtune=native -ffast-math"
    )
  endif()
endif()



message(STATUS "MESA CXX_FLAGS ... ${CMAKE_CXX_FLAGS}")
message(STATUS "MESA C_FLAGS ... ${CMAKE_C_FLAGS}")
message(STATUS "MESA CXX_FLAGS_RELEASE ... ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "MESA C_FLAGS_RELEASE ... ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "MESA CXX_FLAGS_DEBUG ... ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "MESA C_FLAGS_DEBUG ... ${CMAKE_C_FLAGS_DEBUG}")




SET(MESA_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/mesa)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/drm)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/include)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/include/drm-uapi)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/compiler)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/loader)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/broadcom)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/util)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/gallium/winsys)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/gallium/drivers)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/gallium/drivers/v3d)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/gallium/include)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/gallium/auxiliary)
INCLUDE_DIRECTORIES(${MESA_LIB_DIR}/src/mesa)

file (
  GLOB
  MESA_SOURCES
  "mesa/src/broadcom/cle/*"
  "mesa/src/broadcom/clif/*"
  "mesa/src/broadcom/common/*"
  "mesa/src/broadcom/compiler/*"
  "mesa/src/broadcom/qpu/*"
  "mesa/src/broadcom/simulator/*"
  
  "mesa/src/compiler/*"
  "mesa/src/compiler/glsl/*"
  "mesa/src/compiler/nir/*"

  "mesa/src/gallium/auxilary/*"
  "mesa/src/gallium/auxilary/cso_cache/*"
  "mesa/src/gallium/auxilary/driver_ddebug/*"
  "mesa/src/gallium/auxilary/driver_noop/*"
  "mesa/src/gallium/auxilary/driver_rbug/*"
  "mesa/src/gallium/auxilary/driver_trace/*"
  "mesa/src/gallium/auxilary/indices/*"
  "mesa/src/gallium/auxilary/nir/*"
  "mesa/src/gallium/auxilary/os/*"
  "mesa/src/gallium/auxilary/pipe-loader/*"
  "mesa/src/gallium/auxilary/renderonly/*"
  "mesa/src/gallium/auxilary/target-helpers/*"
  "mesa/src/gallium/auxilary/tgsi/*"
  "mesa/src/gallium/auxilary/util/*"
  
  "mesa/src/gallium/drivers/vc4/*"
  "mesa/src/gallium/drivers/v3d/*"
  
  "mesa/src/gallium/include/frontend/*"
  "mesa/src/gallium/include/pipe/*"

  "mesa/src/gallium/targets/osmesa/*"

  "mesa/src/gallium/targets/sw/dri/*"
  "mesa/src/gallium/targets/sw/kms-dri/*"
  "mesa/src/gallium/targets/sw/null/*"
  "mesa/src/gallium/targets/sw/wrapper/*"
  
  "mesa/src/gallium/targets/v3d/drm/*"

  "mesa/src/gallium/winsys/sw/dri/*"
  "mesa/src/gallium/winsys/sw/kms-dri/*"
  "mesa/src/gallium/winsys/sw/null/*"
  "mesa/src/gallium/winsys/sw/wrapper/*"
  "mesa/src/gallium/winsys/v3d/drm/*"

  
  "mesa/src/loader/*"

  "mesa/src/mesa/main/*"
  "mesa/src/mesa/program/*"
  
  "mesa/src/util/*"
  "mesa/src/util/format/*"
  "mesa/src/util/sha1/*"
)

set(MESA_LIB "mesa")
# set_target_properties(${MESA_LIB} PROPERTIES LINKER_LANGUAGE CXX)
# add_library(${MESA_LIB} SHARED ${SOURCES})
add_library(${MESA_LIB}_static STATIC ${MESA_SOURCES} )



# set_target_properties(${V3DLIB}_static PROPERTIES PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/Lib/V3DLib.h)
