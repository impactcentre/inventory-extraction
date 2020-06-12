################################################################################
# 
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################
# MAIN CMAKELISTS FILE FOR CONFIGURING PROJECT BINARIES
############################################################################

# Indicate CMake 2.7 and above that we don't want to mix relative
#  and absolute paths in linker lib lists.
# Run "cmake --help-policy CMP0003" for more information.
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Set default build type to Debug if not set:
IF (UNIX) # only needed for unic systems
IF (NOT CMAKE_BUILD_TYPE)
    message("Warning: build type not specified, defaulting to Debug!")
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF (NOT CMAKE_BUILD_TYPE)
message("cmake build type is: " ${CMAKE_BUILD_TYPE})
ENDIF(UNIX)

# FIND AND CONFIG OPENMP:
SET (USE_OPENMP 1 CACHE BOOL "Use OpenMP?")
IF (USE_OPENMP)
    find_package(OpenMP) # find_package(OpenMP) defines the OpenMP_C_FLAGS and OpenMP_CXX_FLAGS variables
ELSE(USE_OPENMP)
    SET(OpenMP_CXX_FLAGS "") # delete OpenMP flags if not used
ENDIF(USE_OPENMP)

#message("value of USE_OPENMP: " ${USE_OPENMP})
message("OpenMP CXX flags : " ${OpenMP_CXX_FLAGS})

# FIND AND CONFIG QT 4:

# set qmake.exe path for windows here to find correct qt-libs etc.:
IF(WIN32)
    IF(MINGW)
        SET(QT_QMAKE_EXECUTABLE c:/programme/qt/4.6.3_mingw/bin/qmake.exe)
    ENDIF(MINGW)
    IF (MSVC)
        SET(QT_QMAKE_EXECUTABLE c:/programme/qt/4.6.3_msvc2008/bin/qmake.exe)
    ENDIF(MSVC)
ENDIF(WIN32)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
############################################################################

# FIND AND CONFIG BOOST: 
IF(UNIX)
#    set(BOOST_PATH /usr/include/boost/) # boost is usually included automatically on linux systems. set path here if not
ENDIF(UNIX)

# set boost path for windows systems:
IF(WIN32)
    set(BOOST_PATH C:/Programme/boost/boost_1_40_0)
	
    IF(MINGW)
		#set(BOOST_LIB_PATH ${BOOST_PATH}/lib/Debug) # debug lib path
		#set(BOOST_LIB_PATH ${BOOST_PATH}/lib/Release) # release lib path
    ENDIF(MINGW)
    IF (MSVC)
		#set(BOOST_LIB_PATH ${BOOST_PATH}/lib/Debug) # debug lib path
		#set(BOOST_LIB_PATH ${BOOST_PATH}/lib/Release) # release lib path
    ENDIF(MSVC)	

    #set(BOOST_LIBRARIES boost_thread-gcc34-mt-1_40)
    message("boost path = " ${BOOST_PATH})
ENDIF(WIN32)

############################################################################

# FIND AND CONFIG GNUWIN32:
IF(WIN32)
    IF(MINGW)
        set(GNUWIN32_PATH C:/programme/gnuwin32)
    ENDIF(MINGW)
    IF (MSVC)
          set(GNUWIN32_PATH ) # don't use gnuwin32 path in msvc
    ENDIF(MSVC)
ENDIF(WIN32)
############################################################################

# FIND AND CONFIG OPENCV: ------------------------- #
IF (WIN32)
	IF(MINGW)
		set (OpenCV_DIR C:/Programme/OpenCV-2.2.0/mingw) # build folder of opencv using mingw
	ENDIF(MINGW)
	IF (MSVC)
		set (OpenCV_DIR C:/Programme/OpenCV-2.2.0/vs2008) # build folder of opencv using msvc
	ENDIF(MSVC)
ENDIF(WIN32)
IF(UNIX)
    # find_package should find OpenCV_DIR automatically on linux systems, elsewise specify it here!
    #set (OpenCV_DIR xxx)
ENDIF(UNIX)

FIND_PACKAGE( OpenCV REQUIRED )
message("opencv dir: "  ${OpenCV_DIR})
message("opencv include dirs: "  ${OpenCV_INCLUDE_DIRS})
message("opencv lib dirs: " ${OpenCV_LIB_DIR})
message("opencv libs: "  ${OpenCV_LIBS})

# set actually needed opencv libraries for debug and release mode: (i.e.: core, image processing, highgui and machine learning libraries)
IF(UNIX)
    set(MY_OPENCV_LIBRARIES_DEBUG opencv_core opencv_imgproc opencv_highgui opencv_ml)
    set(MY_OPENCV_LIBRARIES_RELEASE opencv_core opencv_imgproc opencv_highgui opencv_ml)
    
	set (MY_OPENCV_LIBRARIES
		debug opencv_core optimized opencv_core
		debug opencv_imgproc optimized opencv_imgproc
		debug opencv_highgui optimized opencv_highgui
		debug opencv_ml optimized opencv_ml)
ENDIF(UNIX)

IF(WIN32)
    set(MY_OPENCV_LIBRARIES_DEBUG opencv_core220d opencv_imgproc220d opencv_highgui220d opencv_ml220d)
    set(MY_OPENCV_LIBRARIES_RELEASE opencv_core220 opencv_imgproc220 opencv_highgui220 opencv_ml220)
    
	set (MY_OPENCV_LIBRARIES
		debug opencv_core220d optimized opencv_core220
		debug opencv_imgproc220d optimized opencv_imgproc220
		debug opencv_highgui220d optimized opencv_highgui220
		debug opencv_ml220d optimized opencv_ml220)
ENDIF(WIN32)

message("myopencv libs: " ${MY_OPENCV_LIBRARIES})
############################################################################

# CONFIGURE QT AND GUI SOURCES:
set(QT_USE_XML TRUE) # use XML - module for qt
include(${QT_USE_FILE}) # set up include and link dir's and defines for QT4
set(PATH_TO_GUI ${ABS_PATH_TO_SRC}/gui)
# set src for gui
set(GUI_SRCS
	${PATH_TO_GUI}/MainWindow.cpp
	${PATH_TO_GUI}/ImageViewer.cpp
	${PATH_TO_GUI}/ClusterMethodWidget.cpp
	${PATH_TO_GUI}/InputFilesWidget.cpp
	${PATH_TO_GUI}/FeatureVectorWidget.cpp
	${PATH_TO_GUI}/PreprocessingWidget.cpp
	
	${PATH_TO_GUI}/ClusterViewListWidget.cpp
	${PATH_TO_GUI}/PrototypeViewListWidget.cpp
	${PATH_TO_GUI}/ClusterViewWidget.cpp
	${PATH_TO_GUI}/ClusteringResultPrototypeViewWidget.cpp
	${PATH_TO_GUI}/ClusterSummaryWidget.cpp
	${PATH_TO_GUI}/ClassificationWidget.cpp
)
# set moc-headers for the sources:
set(GUI_MOC_HDRS
	${PATH_TO_GUI}/MainWindow.hpp
	${PATH_TO_GUI}/ImageViewer.hpp
	${PATH_TO_GUI}/ClusterMethodWidget.hpp
	${PATH_TO_GUI}/InputFilesWidget.hpp
	${PATH_TO_GUI}/FeatureVectorWidget.hpp
	${PATH_TO_GUI}/PreprocessingWidget.hpp
	
	${PATH_TO_GUI}/ClusterViewListWidget.hpp
	${PATH_TO_GUI}/PrototypeViewListWidget.hpp
	${PATH_TO_GUI}/ClusterViewWidget.hpp
	${PATH_TO_GUI}/ClusteringResultPrototypeViewWidget.hpp
	${PATH_TO_GUI}/ClusterSummaryWidget.hpp
	${PATH_TO_GUI}/ClassificationWidget.hpp
	
	${ABS_PATH_TO_SRC}/clustering/ClusteringController.hpp
	${ABS_PATH_TO_SRC}/clustering/algorithms/ClusterMethodI.hpp
	${ABS_PATH_TO_SRC}/clustering/algorithms/AgglomerativeClustering.hpp
)
# set ui files:
set(GUI_UIS
    ${PATH_TO_GUI}/ui/FineReaderXMLDataParserWidget.ui
    ${PATH_TO_GUI}/ui/ClusterMethodWidget.ui
    ${PATH_TO_GUI}/ui/InputFilesWidget.ui
    ${PATH_TO_GUI}/ui/FeatureVectorWidget.ui
    ${PATH_TO_GUI}/ui/PreprocessingWidget.ui
    ${PATH_TO_GUI}/ui/ClusterSummaryWidget.ui
    ${PATH_TO_GUI}/ui/ClassificationWidget.ui
)
# set resource files:
set(RCC_SRCS ${PATH_TO_GUI}/Resource.qrc)
set(WIN_RCS ${PATH_TO_GUI}/win_resource.rc)
# add resource files to QT4 resource variable:
QT4_ADD_RESOURCES(GUI_RCC_SRCS ${RCC_SRCS})
# wrap ui's:
QT4_WRAP_UI(GUI_UIS_H ${GUI_UIS})
# Generate a list of moc-src files out of the moc-headers:
QT4_WRAP_CPP(GUI_MOC_SRCS ${GUI_MOC_HDRS})
# message("moc files are: " ${GUI_MOC_SRCS})
############################################################################

# SET MATLAB DIRS HERE: (OPTIONAL)
#IF(WIN32)
#  set(MATLAB_ROOT C:/Programme/MATLAB/R2009a)
#  set(MATLAB_INCLUDE_DIR ${MATLAB_ROOT}/extern/include)
#  set(MATLAB_LIB_DIR ${MATLAB_ROOT}/extern/lib/win32/microsoft)
#  set(MATLAB_MEX_LIBRARY libmex)
#  set(MATLAB_MX_LIBRARY libmx)
#  set(MATLAB_ENG_LIBRARY libeng)
#  message("matlab root is: " ${MATLAB_ROOT})
#ENDIF(WIN32)
############################################################################

# FIND AND CONFIG FINEREADER ENGINE: (OPTIONAL)
# set finereader's path to corresponding environment variable:
#set(FINEREADER_PATH $ENV{FINEREADER_PATH})
#message("FineReader dir: " ${FINEREADER_PATH})
############################################################################

#set include paths: ------------------------------- #
# set qt related include paths: (specific variables set above!)
set(MY_QT_INCLUDE_DIRS
    ${QT_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    )

set(USER_DEFINED_INCLUDE_PATHS
    .
    ${GNUWIN32_PATH}/include # gnuwin32 incpath
    ${BOOST_PATH} # boost inc.path
    ${OpenCV_INCLUDE_DIRS} # OpenCV include paths
    ${MY_QT_INCLUDE_DIRS}
	# ${FINEREADER_PATH}/Inc # finereader include path
    #${MATLAB_INCLUDE_DIR}
    )
        
INCLUDE_DIRECTORIES (
	${USER_DEFINED_INCLUDE_PATHS}
	)
############################################################################

#set lib paths: ----------------------------------- #
set(USER_DEFINED_LIB_PATHS
	${GNUWIN32_PATH}/lib # gnuwin32 libpath
	${OpenCV_LIB_DIR}
	${BOOST_LIB_PATH}
	#${MATLAB_LIB_DIR}
)

LINK_DIRECTORIES(${USER_DEFINED_LIB_PATHS})
############################################################################

# set libraries needed from qt: ------------------- #
set (MY_QT_LIBRARIES
    ${QT_LIBRARIES} # core qt libraries
    ${QT_QTXML_LIBRARIES} # xml qt libraries
    )
############################################################################

# set project headers: ------------------------------------ #
#headers of core module:
set(SRC_HEADERS
    ${ABS_PATH_TO_SRC}/core_headers.h
    ${ABS_PATH_TO_SRC}/image_headers.h
    ${ABS_PATH_TO_SRC}/impact.h
    )

set(CORE_HEADERS
    ${ABS_PATH_TO_SRC}/core/core.h
    ${ABS_PATH_TO_SRC}/core/Exception.hpp
    ${ABS_PATH_TO_SRC}/core/float_types.hpp
    ${ABS_PATH_TO_SRC}/core/Index.hpp
    ${ABS_PATH_TO_SRC}/core/OpenCV.hpp
    ${ABS_PATH_TO_SRC}/core/OpenCVGUI.hpp
    ${ABS_PATH_TO_SRC}/core/PCA.hpp
    ${ABS_PATH_TO_SRC}/core/Vector2D.hpp
    ${ABS_PATH_TO_SRC}/core/Vector3D.hpp
 
    ${ABS_PATH_TO_SRC}/core/image/BinaryImageMoments.hpp
    ${ABS_PATH_TO_SRC}/core/image/BoundingBox.hpp
    ${ABS_PATH_TO_SRC}/core/image/GrayImage.hpp
    ${ABS_PATH_TO_SRC}/core/image/Histogram1D.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageFilter.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageIntegralInvariant.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageIterator.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageMoments.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageOperations.hpp
    ${ABS_PATH_TO_SRC}/core/image/ImageRotationInvariant.hpp
    ${ABS_PATH_TO_SRC}/core/image/InvariantHistogram.hpp
    ${ABS_PATH_TO_SRC}/core/image/Kernel.hpp
    ${ABS_PATH_TO_SRC}/core/image/Morphology.hpp
    ${ABS_PATH_TO_SRC}/core/image/types.h
    )
 #headers of util module:
set(UTIL_HEADERS
    ${ABS_PATH_TO_SRC}/util/platform.h
    ${ABS_PATH_TO_SRC}/util/std_util.h
    ${ABS_PATH_TO_SRC}/util/StopWatch.hpp
    ${ABS_PATH_TO_SRC}/util/ublas_util.h
    ${ABS_PATH_TO_SRC}/util/util.h
    ${ABS_PATH_TO_SRC}/util/ColorMap.hpp
    ${ABS_PATH_TO_SRC}/util/RandomNumber.hpp
    )
 #headers of ext module:
 set(EXT_HEADERS
    ${ABS_PATH_TO_SRC}/ext/cluster/cluster.h
    )
    
#headers of xml module:
set(XML_HEADERS
    ${ABS_PATH_TO_SRC}/xml/ClusteringResultXMLWriter.hpp
    ${ABS_PATH_TO_SRC}/xml/FineReaderXMLOutputHandler.hpp
    ${ABS_PATH_TO_SRC}/xml/Im2CharRectsXMLOutputHandler.hpp
    ${ABS_PATH_TO_SRC}/xml/ClusteringResultOutputHandler.hpp
    ${ABS_PATH_TO_SRC}/xml/xml.h
    )
#headers of clustering module:
set(CLUSTERING_HEADERS
    # basic objects:
    ${ABS_PATH_TO_SRC}/clustering/CharCluster.hpp
    ${ABS_PATH_TO_SRC}/clustering/CharFeatureCollection.hpp
    ${ABS_PATH_TO_SRC}/clustering/ClusteringController.hpp
    ${ABS_PATH_TO_SRC}/clustering/ClusteringResult.hpp
    ${ABS_PATH_TO_SRC}/clustering/DataStructs.hpp
    ${ABS_PATH_TO_SRC}/clustering/Document.hpp
    ${ABS_PATH_TO_SRC}/clustering/PreprocessingResults.hpp
    # data reader objects:
    ${ABS_PATH_TO_SRC}/clustering/data_reader/CharDataReaderI.hpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/FRXMLCharDataReader.hpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/Im2CharRectsCharDataReader.hpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/DatFileDataReader.hpp
    # clustering algorithms objects:
    ${ABS_PATH_TO_SRC}/clustering/algorithms/ClusterMethodI.hpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/KMeans.hpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/LeaderFollower.hpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/AgglomerativeClustering.hpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/DBSCANClustering.hpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/CLARANSClustering.hpp
    # feature extraction objects:
    ${ABS_PATH_TO_SRC}/clustering/features/AMIFeature.hpp
    ${ABS_PATH_TO_SRC}/clustering/features/BasicMomentsFeature.hpp
    ${ABS_PATH_TO_SRC}/clustering/features/HuMomentsFeature.hpp
    ${ABS_PATH_TO_SRC}/clustering/features/ImageCharFeatureI.hpp
    ${ABS_PATH_TO_SRC}/clustering/features/ImageIntegralInvariantFeature.hpp
    ${ABS_PATH_TO_SRC}/clustering/features/ProfilesFeatures.hpp
    )

# all objects summarized:
set(ALL_HEADERS
    ${SRC_HEADERS}
    ${CORE_HEADERS}
    ${UTIL_HEADERS}
    ${EXT_HEADERS}
    ${XML_HEADERS}
    ${CLUSTERING_HEADERS}
	)
############################################################################

# set project objects: ------------------------------------ #
#objects of core module:
set(CORE_OBJECTS
    ${ABS_PATH_TO_SRC}/core/PCA.cpp

    ${ABS_PATH_TO_SRC}/core/image/GrayImage.cpp
    ${ABS_PATH_TO_SRC}/core/image/BoundingBox.cpp
    ${ABS_PATH_TO_SRC}/core/image/Kernel.cpp
    ${ABS_PATH_TO_SRC}/core/image/ImageFilter.cpp
    ${ABS_PATH_TO_SRC}/core/image/ImageIterator.cpp
    ${ABS_PATH_TO_SRC}/core/image/BinaryImageMoments.cpp
    ${ABS_PATH_TO_SRC}/core/image/ImageMoments.cpp
    ${ABS_PATH_TO_SRC}/core/image/Morphology.cpp
    ${ABS_PATH_TO_SRC}/core/image/Histogram1D.cpp
    ${ABS_PATH_TO_SRC}/core/image/ImageIntegralInvariant.cpp
    ${ABS_PATH_TO_SRC}/core/image/ImageRotationInvariant.cpp
    ${ABS_PATH_TO_SRC}/core/image/InvariantHistogram.cpp
    )
 #objects of util module:
set(UTIL_OBJECTS
    ${ABS_PATH_TO_SRC}/util/util.cpp
    ${ABS_PATH_TO_SRC}/util/StopWatch.cpp
    ${ABS_PATH_TO_SRC}/util/ColorMap.cpp
    ${ABS_PATH_TO_SRC}/util/RandomNumber.cpp
    )
 #objects of ext module:
 set(EXT_OBJECTS
    ${ABS_PATH_TO_SRC}/ext/cluster/cluster.c
    )    
#objects of xml module:
set(XML_OBJECTS
    ${ABS_PATH_TO_SRC}/xml/FineReaderXMLOutputHandler.cpp
    ${ABS_PATH_TO_SRC}/xml/Im2CharRectsXMLOutputHandler.cpp
    ${ABS_PATH_TO_SRC}/xml/ClusteringResultXMLWriter.cpp
    ${ABS_PATH_TO_SRC}/xml/ClusteringResultOutputHandler.cpp
    )
#objects of clustering module:
set(CLUSTERING_OBJECTS
    # basic objects:
    ${ABS_PATH_TO_SRC}/clustering/DataStructs.cpp
    ${ABS_PATH_TO_SRC}/clustering/CharCluster.cpp
    ${ABS_PATH_TO_SRC}/clustering/CharFeatureCollection.cpp
    ${ABS_PATH_TO_SRC}/clustering/ClusteringController.cpp
    ${ABS_PATH_TO_SRC}/clustering/ClusteringResult.cpp
    ${ABS_PATH_TO_SRC}/clustering/Document.cpp
    ${ABS_PATH_TO_SRC}/clustering/PreprocessingResults.cpp
    # data reader objects:
    ${ABS_PATH_TO_SRC}/clustering/data_reader/CharDataReaderI.cpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/FRXMLCharDataReader.cpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/Im2CharRectsCharDataReader.cpp
    ${ABS_PATH_TO_SRC}/clustering/data_reader/DatFileDataReader.cpp
    # clustering algorithms objects:
    ${ABS_PATH_TO_SRC}/clustering/algorithms/KMeans.cpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/LeaderFollower.cpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/AgglomerativeClustering.cpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/DBSCANClustering.cpp
    ${ABS_PATH_TO_SRC}/clustering/algorithms/CLARANSClustering.cpp
    # feature extraction objects:
    ${ABS_PATH_TO_SRC}/clustering/features/ImageIntegralInvariantFeature.cpp
    ${ABS_PATH_TO_SRC}/clustering/features/BasicMomentsFeature.cpp
    ${ABS_PATH_TO_SRC}/clustering/features/AMIFeature.cpp
    ${ABS_PATH_TO_SRC}/clustering/features/HuMomentsFeature.cpp
    ${ABS_PATH_TO_SRC}/clustering/features/ProfilesFeatures.cpp
    )

# all objects summarized:
set(ALL_OBJECTS
    ${CORE_OBJECTS}
    ${UTIL_OBJECTS}
    ${EXT_OBJECTS}
    ${XML_OBJECTS}
    ${FINEREADER_OBJECTS}
    ${CLUSTERING_OBJECTS}
	)
set (GUI_DEPENDENCIES
    ${GUI_UIS_H}
    ${GUI_SRCS}
    ${GUI_MOC_SRCS}
    ${GUI_RCC_SRCS}
    ${WIN_RCS}
    ) 
############################################################################

# set additional flags: --------------------------- #
## set additional cxx flags for visual studio compiler:
IF (MSVC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /DWIN32 /D_WINDOWS /W3 /Zm1000 /EHsc /GR /wd4715 /wd4996 /wd4018 ${OpenMP_CXX_FLAGS}")
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${my_cxx_flags})
    ADD_DEFINITIONS(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
    message("CMAKE_CXX_FLAGS are: " ${CMAKE_CXX_FLAGS})
ENDIF(MSVC)

# set additional cxx flags for unix and mingw in debug mode:
IF(UNIX OR MINGW)
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}) # add openmp flags to general cxx flags
	SET(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -Wall -Wno-sign-compare") # statically set debug flags for unix and mingw
	message("CMAKE_CXX_FLAGS are: " ${CMAKE_CXX_FLAGS})
ENDIF(UNIX OR MINGW)
############################################################################

# add executables: -------------------------------- #
# inventory_extraction_tool - MAIN APPLICATION:
set(CURRENT_EXAMPLE inventory_extraction_tool)
add_executable(${CURRENT_EXAMPLE} ${CURRENT_EXAMPLE}.cpp
                ${ALL_OBJECTS}
                ${ALL_HEADERS}
                ${GUI_DEPENDENCIES}
                )
target_link_libraries(${CURRENT_EXAMPLE} 
    ${MY_OPENCV_LIBRARIES}
    #${OpenCV_LIBS} # all opencv libs
    ${MY_QT_LIBRARIES}
    ${BOOST_LIBRARIES})
#set(CMAKE_BUILD_TYPE Debug) # set debug built type for this target


