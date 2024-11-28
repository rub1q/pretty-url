function(create_version_file)

file(WRITE include/prettyurl/version.hpp
  "#pragma once\n\n"
  
  "#define PRETTYURL_VER_MAJOR     ${CMAKE_PROJECT_VERSION_MAJOR}\n"
  "#define PRETTYURL_VER_MINOR     ${CMAKE_PROJECT_VERSION_MINOR}\n"
  "#define PRETTYURL_VER_PATCH     ${CMAKE_PROJECT_VERSION_PATCH}\n"
  "#define PRETTYURL_VER_TWEAK     ${CMAKE_PROJECT_VERSION_TWEAK}\n\n"

  "#define PRETTYURL_VERSTR2(s) #s\n"
  "#define PRETTYURL_VERSTR(s) VERSTR2(s)\n\n"

  "#define PRETTYURL_VER_FILE_VER      VER_MAJOR,VER_MINOR,VER_PATCH,VER_TWEAK\n"
  "#define PRETTYURL_VER_FILE_VER_STR  VERSTR(VER_MAJOR) \".\" VERSTR(VER_MINOR) \".\" VERSTR(VER_PATCH) \".\" VERSTR(VER_TWEAK)\n\n"

  "#define PRETTYURL_VERSION           VER_FILE_VER_STR\n"
)

endfunction()