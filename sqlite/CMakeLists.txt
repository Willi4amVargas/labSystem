add_library(sqlite SHARED
    sqlite3.c
)

target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_LIST_DIR})