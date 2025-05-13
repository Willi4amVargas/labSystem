add_library(Laboratory SHARED
    laboratory.h
    laboratory.cpp
    include/Products.h
    models/Products.cpp
    include/Patients.h
    models/Patients.cpp
    include/Inventory.h
    models/Inventory.cpp
    include/LabTests.h
    models/LabTests.cpp
)

target_include_directories(Laboratory PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_SOURCE_DIR}/sqlite)

target_link_libraries(Laboratory PRIVATE sqlite)