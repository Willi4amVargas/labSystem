add_library(Laboratory SHARED
    laboratory.h
    laboratory.cpp
    include/Products.h
    models/Products.cpp
    include/Patients.h
    models/Patiens.cpp
    include/Inventory.h
    models/Inventory.cpp
)

target_include_directories(Laboratory PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/include)
