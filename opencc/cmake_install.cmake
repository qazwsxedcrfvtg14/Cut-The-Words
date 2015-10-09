# Install script for directory: D:/joe_data/Desktop/OpenCC-master/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/lib/opencc.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joe_data/Desktop/OpenCC-master/src/Debug/opencc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/lib/opencc.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joe_data/Desktop/OpenCC-master/src/Release/opencc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/lib/opencc.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joe_data/Desktop/OpenCC-master/src/MinSizeRel/opencc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/lib/opencc.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joe_data/Desktop/OpenCC-master/src/RelWithDebInfo/opencc.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/include/opencc/BinaryDict.hpp;/include/opencc/Common.hpp;/include/opencc/Config.hpp;/include/opencc/Conversion.hpp;/include/opencc/ConversionChain.hpp;/include/opencc/Converter.hpp;/include/opencc/DartsDict.hpp;/include/opencc/Dict.hpp;/include/opencc/DictEntry.hpp;/include/opencc/DictGroup.hpp;/include/opencc/Exception.hpp;/include/opencc/Export.hpp;/include/opencc/Lexicon.hpp;/include/opencc/MaxMatchSegmentation.hpp;/include/opencc/Optional.hpp;/include/opencc/PhraseExtract.hpp;/include/opencc/Segmentation.hpp;/include/opencc/Segments.hpp;/include/opencc/SerializableDict.hpp;/include/opencc/SimpleConverter.hpp;/include/opencc/TextDict.hpp;/include/opencc/UTF8StringSlice.hpp;/include/opencc/UTF8Util.hpp;/include/opencc/opencc.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/include/opencc" TYPE FILE FILES
    "D:/joe_data/Desktop/OpenCC-master/src/BinaryDict.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Common.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Config.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Conversion.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/ConversionChain.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Converter.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/DartsDict.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Dict.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/DictEntry.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/DictGroup.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Exception.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Export.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Lexicon.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/MaxMatchSegmentation.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Optional.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/PhraseExtract.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Segmentation.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/Segments.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/SerializableDict.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/SimpleConverter.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/TextDict.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/UTF8StringSlice.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/UTF8Util.hpp"
    "D:/joe_data/Desktop/OpenCC-master/src/opencc.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/joe_data/Desktop/OpenCC-master/src/tools/cmake_install.cmake")

endif()

