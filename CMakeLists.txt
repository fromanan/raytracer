cmake_minimum_required(VERSION 3.17)
project(Raytracer)

set(EXEC_NAME Raytracer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

add_definitions(-D_AFXDLL)
add_definitions(-DGLEW_STATIC)

set(PROJECT_NAME "Project1")
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
set(GLEW_ROOT "${PROJECT_ROOT}/ShaderWnd/external/glew-1.9.0")
set(GLM_ROOT "${PROJECT_ROOT}/ShaderWnd/external/glm")
set(GRAPHICS_DIR "${PROJECT_ROOT}/graphics")

#find_package(GLEW REQUIRED)

set(INCLUDE_DIR
        ${PROJECT_ROOT}
        ${PROJECT_ROOT}/graphics
        ${PROJECT_ROOT}/models
        ${PROJECT_ROOT}/res
        ${PROJECT_ROOT}/ShaderWnd
        ${PROJECT_ROOT}/ShaderWnd/external
        ${GLEW_ROOT}
        ${GLEW_ROOT}/include
        ${GLEW_ROOT}/include/GL
        ${GLM_ROOT}
        ${GLM_ROOT}/core
        ${GLM_ROOT}/gtc
        ${PROJECT_ROOT}/textures
)

include_directories(${INCLUDE_DIR})

set(GRAPHICS_FILES
        ${GRAPHICS_DIR}/accjitter.cpp
        ${GRAPHICS_DIR}/accjitter.h
        ${GRAPHICS_DIR}/GrCamera.cpp
        ${GRAPHICS_DIR}/GrCamera.h
        ${GRAPHICS_DIR}/GrCubeTexture.cpp
        ${GRAPHICS_DIR}/GrCubeTexture.h
        ${GRAPHICS_DIR}/GrObject.cpp
        ${GRAPHICS_DIR}/GrObject.h
        ${GRAPHICS_DIR}/GrPoint.h
        ${GRAPHICS_DIR}/GrRenderer.cpp
        ${GRAPHICS_DIR}/GrRenderer.h
        ${GRAPHICS_DIR}/GrTexture.cpp
        ${GRAPHICS_DIR}/GrTexture.h
        ${GRAPHICS_DIR}/GrTransform.cpp
        ${GRAPHICS_DIR}/GrTransform.h
        ${GRAPHICS_DIR}/GrVector.h
        ${GRAPHICS_DIR}/GrVRMLFactory.cpp
        ${GRAPHICS_DIR}/GrVRMLFactory.h
        ${GRAPHICS_DIR}/jitter.h
        ${GRAPHICS_DIR}/libRayIntersection.h
        ${GRAPHICS_DIR}/libvrml.h
        ${GRAPHICS_DIR}/Nurbs.cpp
        ${GRAPHICS_DIR}/Nurbs.h
        ${GRAPHICS_DIR}/OpenGLRenderer.cpp
        ${GRAPHICS_DIR}/OpenGLRenderer.h
        ${GRAPHICS_DIR}/OpenGLWnd.cpp
        ${GRAPHICS_DIR}/OpenGLWnd.h
        ${GRAPHICS_DIR}/RayIntersection.h
        ${GRAPHICS_DIR}/Texture.cpp
        ${GRAPHICS_DIR}/Texture.h
)

set(GLEW_FILES
        ${GLEW_ROOT}/include/GL/glew.h
        ${GLEW_ROOT}/include/GL/glext.h
        ${GLEW_ROOT}/include/GL/wglew.h
        ${GLEW_ROOT}/include/GL/wglext.h
)

set(GLM_FILES
        ${GLM_ROOT}/core/_detail.hpp
        ${GLM_ROOT}/core/_fixes.hpp
        ${GLM_ROOT}/core/_swizzle.hpp
        ${GLM_ROOT}/core/_swizzle_func.hpp
        ${GLM_ROOT}/core/_vectorize.hpp
        ${GLM_ROOT}/core/dummy.cpp
        ${GLM_ROOT}/core/func_common.hpp
        ${GLM_ROOT}/core/func_common.inl
        ${GLM_ROOT}/core/func_exponential.hpp
        ${GLM_ROOT}/core/func_exponential.inl
        ${GLM_ROOT}/core/func_geometric.hpp
        ${GLM_ROOT}/core/func_geometric.inl
        ${GLM_ROOT}/core/func_integer.hpp
        ${GLM_ROOT}/core/func_integer.inl
        ${GLM_ROOT}/core/func_matrix.hpp
        ${GLM_ROOT}/core/func_matrix.inl
        ${GLM_ROOT}/core/func_noise.hpp
        ${GLM_ROOT}/core/func_noise.inl
        ${GLM_ROOT}/core/func_packing.hpp
        ${GLM_ROOT}/core/func_packing.inl
        ${GLM_ROOT}/core/func_trigonometric.hpp
        ${GLM_ROOT}/core/func_trigonometric.inl
        ${GLM_ROOT}/core/func_vector_relational.hpp
        ${GLM_ROOT}/core/func_vector_relational.inl
        ${GLM_ROOT}/core/hint.hpp
        ${GLM_ROOT}/core/intrinsic_common.hpp
        ${GLM_ROOT}/core/intrinsic_common.inl
        ${GLM_ROOT}/core/intrinsic_exponential.hpp
        ${GLM_ROOT}/core/intrinsic_exponential.inl
        ${GLM_ROOT}/core/intrinsic_geometric.hpp
        ${GLM_ROOT}/core/intrinsic_geometric.inl
        ${GLM_ROOT}/core/intrinsic_matrix.hpp
        ${GLM_ROOT}/core/intrinsic_matrix.inl
        ${GLM_ROOT}/core/intrinsic_trigonometric.hpp
        ${GLM_ROOT}/core/intrinsic_trigonometric.inl
        ${GLM_ROOT}/core/intrinsic_vector_relational.hpp
        ${GLM_ROOT}/core/intrinsic_vector_relational.inl
        ${GLM_ROOT}/core/setup.hpp
        ${GLM_ROOT}/core/type.hpp
        ${GLM_ROOT}/core/type_float.hpp
        ${GLM_ROOT}/core/type_gentype.hpp
        ${GLM_ROOT}/core/type_gentype.inl
        ${GLM_ROOT}/core/type_half.hpp
        ${GLM_ROOT}/core/type_half.inl
        ${GLM_ROOT}/core/type_int.hpp
        ${GLM_ROOT}/core/type_mat.hpp
        ${GLM_ROOT}/core/type_mat.inl
        ${GLM_ROOT}/core/type_mat2x2.hpp
        ${GLM_ROOT}/core/type_mat2x2.inl
        ${GLM_ROOT}/core/type_mat2x3.hpp
        ${GLM_ROOT}/core/type_mat2x3.inl
        ${GLM_ROOT}/core/type_mat2x4.hpp
        ${GLM_ROOT}/core/type_mat2x4.inl
        ${GLM_ROOT}/core/type_mat3x2.hpp
        ${GLM_ROOT}/core/type_mat3x2.inl
        ${GLM_ROOT}/core/type_mat3x3.hpp
        ${GLM_ROOT}/core/type_mat3x3.inl
        ${GLM_ROOT}/core/type_mat3x4.hpp
        ${GLM_ROOT}/core/type_mat3x4.inl
        ${GLM_ROOT}/core/type_mat4x2.hpp
        ${GLM_ROOT}/core/type_mat4x2.inl
        ${GLM_ROOT}/core/type_mat4x3.hpp
        ${GLM_ROOT}/core/type_mat4x3.inl
        ${GLM_ROOT}/core/type_mat4x4.hpp
        ${GLM_ROOT}/core/type_mat4x4.inl
        ${GLM_ROOT}/core/type_size.hpp
        ${GLM_ROOT}/core/type_vec.hpp
        ${GLM_ROOT}/core/type_vec.inl
        ${GLM_ROOT}/core/type_vec1.hpp
        ${GLM_ROOT}/core/type_vec1.inl
        ${GLM_ROOT}/core/type_vec2.hpp
        ${GLM_ROOT}/core/type_vec2.inl
        ${GLM_ROOT}/core/type_vec3.hpp
        ${GLM_ROOT}/core/type_vec3.inl
        ${GLM_ROOT}/core/type_vec4.hpp
        ${GLM_ROOT}/core/type_vec4.inl
        ${GLM_ROOT}/gtc/constants.hpp
        ${GLM_ROOT}/gtc/constants.inl
        ${GLM_ROOT}/gtc/epsilon.hpp
        ${GLM_ROOT}/gtc/epsilon.inl
        ${GLM_ROOT}/gtc/half_float.hpp
        ${GLM_ROOT}/gtc/half_float.inl
        ${GLM_ROOT}/gtc/matrix_access.hpp
        ${GLM_ROOT}/gtc/matrix_access.inl
        ${GLM_ROOT}/gtc/matrix_integer.hpp
        ${GLM_ROOT}/gtc/matrix_inverse.hpp
        ${GLM_ROOT}/gtc/matrix_inverse.inl
        ${GLM_ROOT}/gtc/matrix_transform.hpp
        ${GLM_ROOT}/gtc/matrix_transform.inl
        ${GLM_ROOT}/gtc/noise.hpp
        ${GLM_ROOT}/gtc/noise.inl
        ${GLM_ROOT}/gtc/quaternion.hpp
        ${GLM_ROOT}/gtc/quaternion.inl
        ${GLM_ROOT}/gtc/random.hpp
        ${GLM_ROOT}/gtc/random.inl
        ${GLM_ROOT}/gtc/reciprocal.hpp
        ${GLM_ROOT}/gtc/reciprocal.inl
        ${GLM_ROOT}/gtc/swizzle.hpp
        ${GLM_ROOT}/gtc/swizzle.inl
        ${GLM_ROOT}/gtc/type_precision.hpp
        ${GLM_ROOT}/gtc/type_precision.inl
        ${GLM_ROOT}/gtc/type_ptr.hpp
        ${GLM_ROOT}/gtc/type_ptr.inl
        ${GLM_ROOT}/gtc/ulp.hpp
        ${GLM_ROOT}/gtc/ulp.inl
        ${GLM_ROOT}/glm.hpp
)

set(RESOURCE_FILES
        ${PROJECT_ROOT}/models/BLUEGILL.bmp
        ${PROJECT_ROOT}/res/${PROJECT_NAME}.ico
        ${PROJECT_ROOT}/res/${PROJECT_NAME}.rc2
        ${PROJECT_ROOT}/res/Toolbar.bmp
        ${PROJECT_ROOT}/textures/marble02.bmp
        ${PROJECT_ROOT}/textures/marble03.bmp
        ${PROJECT_ROOT}/textures/marble10.bmp
        ${PROJECT_ROOT}/textures/plank01.bmp
        ${PROJECT_ROOT}/textures/sunset.bmp
        ${PROJECT_ROOT}/textures/wood_tile.bmp
        ${PROJECT_ROOT}/textures/worldmap.bmp
)

set(PROJECT_FILES
        ${PROJECT_ROOT}/ShaderWnd/glew.cpp
        ${PROJECT_ROOT}/ShaderWnd/ShaderWnd.cpp
        ${PROJECT_ROOT}/ShaderWnd/ShaderWnd.h
        ${PROJECT_ROOT}/ChildView.cpp
        ${PROJECT_ROOT}/ChildView.h
        ${PROJECT_ROOT}/CMyRaytraceRenderer.cpp
        ${PROJECT_ROOT}/CMyRaytraceRenderer.h
        ${PROJECT_ROOT}/Debugger.h
        ${PROJECT_ROOT}/framework.h
        ${PROJECT_ROOT}/MainFrm.cpp
        ${PROJECT_ROOT}/MainFrm.h
        ${PROJECT_ROOT}/pch.cpp
        ${PROJECT_ROOT}/pch.h
        ${PROJECT_ROOT}/Poisson.h
        ${PROJECT_ROOT}/${PROJECT_NAME}.aps
        ${PROJECT_ROOT}/${PROJECT_NAME}.cpp
        ${PROJECT_ROOT}/${PROJECT_NAME}.h
        ${PROJECT_ROOT}/${PROJECT_NAME}.rc
        ${PROJECT_ROOT}/resource.h
        ${PROJECT_ROOT}/ShaderUtilities.h
        ${PROJECT_ROOT}/targetver.h
        ${PROJECT_ROOT}/libRayIntersection.dll
        ${PROJECT_ROOT}/libRayIntersection.lib
)

set(SOURCE_FILES
        ${GRAPHICS_FILES}
        ${GLEW_FILES}
        ${GLM_FILES}
        ${RESOURCE_FILES}
        ${PROJECT_FILES}
)

add_executable(${EXEC_NAME} WIN32 ${SOURCE_FILES})

#target_link_libraries(Raytracer ${GRAPHICS_DIR}/libRayIntersection)
#target_link_libraries(Raytracer LINK_PUBLIC ${GRAPHICS_DIR}/libRayIntersection.lib)
#target_link_libraries(Raytracer ${CMAKE_CURRENT_SOURCE_DIR}/libRayIntersection.lib)

#target_link_libraries(Raytracer ${GRAPHICS_DIR}/libRayIntersection)

#add_library(${PROJECT_ROOT}/libRayIntersection.dll DYNAMIC IMPORTED)
#add_library(${GRAPHICS_DIR}/libRayIntersection STATIC IMPORTED)
#add_library(${PROJECT_ROOT}/libRayIntersection.lib STATIC IMPORTED)

#target_link_libraries(Raytracer libRayIntersection)

#target_link_libraries(Raytracer ${GRAPHICS_DIR}/CRayIntersection)

add_library(CRayIntersection STATIC ${GRAPHICS_DIR}/libRayIntersection.h)
set_target_properties(CRayIntersection PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(Raytracer ${GRAPHICS_DIR}/libRayIntersection.lib)

add_library(VRML STATIC ${GRAPHICS_DIR}/libvrml.h)
set_target_properties(VRML PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(Raytracer ${GRAPHICS_DIR}/libvrml.lib)