
include(FetchContent)

FetchContent_Declare(iconv
    GIT_REPOSITORY "https://github.com/sonilyan/libiconv_for_fallout2ce.git"
)

FetchContent_GetProperties(iconv)
if (NOT iconv_POPULATED)
    FetchContent_Populate(iconv)
endif()

# Patch loop_wchar.h: Vita SDK's <wchar.h> properly declares mbrtowc,
# so the fallback `extern size_t mbrtowc ();` declaration causes a conflict.
file(READ "${iconv_SOURCE_DIR}/lib/loop_wchar.h" LOOP_WCHAR_CONTENT)
string(REPLACE "extern size_t mbrtowc ();" "/* Vita has a proper mbrtowc declaration in <wchar.h> */" LOOP_WCHAR_CONTENT "${LOOP_WCHAR_CONTENT}")
file(WRITE "${iconv_SOURCE_DIR}/lib/loop_wchar.h" "${LOOP_WCHAR_CONTENT}")

add_subdirectory(${iconv_SOURCE_DIR} ${iconv_BINARY_DIR} EXCLUDE_FROM_ALL)

set(ICONV_LIBRARIES "iconv-static" PARENT_SCOPE)
set(ICONV_INCLUDE_DIRS ${iconv_SOURCE_DIR} ${iconv_BINARY_DIR}/include PARENT_SCOPE)

