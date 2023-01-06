include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Fallout 2 CE")
set(VITA_TITLEID  "FOUT00002")

set(EXECUTABLE_NAME fallout2-ce)
set(VITA_VERSION "01.00")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS}")
vita_create_self(${EXECUTABLE_NAME}.self ${EXECUTABLE_NAME} STRIPPED)

vita_create_vpk(${EXECUTABLE_NAME}.vpk ${VITA_TITLEID} ${EXECUTABLE_NAME}.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
    FILE ${CMAKE_SOURCE_DIR}/vita/sce_sys/icon0.png sce_sys/icon0.png
    FILE ${CMAKE_SOURCE_DIR}/vita/sce_sys/pic0.png sce_sys/pic0.png
    FILE ${CMAKE_SOURCE_DIR}/vita/sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
    FILE ${CMAKE_SOURCE_DIR}/vita/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
    FILE ${CMAKE_SOURCE_DIR}/vita/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
