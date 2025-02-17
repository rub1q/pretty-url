#pragma once

#define PRETTYURL_VER_MAJOR     1
#define PRETTYURL_VER_MINOR     0
#define PRETTYURL_VER_PATCH     0
#define PRETTYURL_VER_TWEAK     0

#define PRETTYURL_VERSTR2(s) #s
#define PRETTYURL_VERSTR(s) VERSTR2(s)

#define PRETTYURL_VER_FILE_VER      VER_MAJOR,VER_MINOR,VER_PATCH,VER_TWEAK
#define PRETTYURL_VER_FILE_VER_STR  VERSTR(VER_MAJOR) "." VERSTR(VER_MINOR) "." VERSTR(VER_PATCH) "." VERSTR(VER_TWEAK)

#define PRETTYURL_VERSION           VER_FILE_VER_STR
