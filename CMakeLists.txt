cmake_minimum_required(VERSION 3.27)
project(CityOfWeirdFishes)

set(CMAKE_CXX_STANDARD 17)


if (WIN32)
    include_directories("C:/Program Files/SFML-2.6.1/include")
    link_directories("C:/Program Files/SFML-2.6.1/lib")
elseif (UNIX AND NOT APPLE)
    include_directories("/home/ganymede/lib/SFML-2.6.1/include")
    link_directories("/home/ganymede/lib/SFML-2.6.1/lib")
endif ()

add_executable(CityOfWeirdFishes main.cpp
        src/IO/ReaderParameters.h
        src/sim/roads/rTransRNodeToRRNode.h
        src/sim/roads/node/rNode.h
        src/sim/roads/node/rRNode.h
        src/sim/structure/grids/transformation/gBaseToGradientMinimum.h
        src/sim/structure/grids/gIGrid.h
        src/sim/structure/grids/gBasicGrid.h
        src/sim/structure/grids/transformation/gBasicTransformations.h
        src/display/rGlobal.cpp
        src/display/rGlobal.h
        src/display/rPileMenus.h
        src/display/layers/gDispLayers.h
        src/display/layers/gILayer.h
        src/display/layers/tileset/gTileset.h
        src/display/layers/implementation/gLayerAirPollution.h
        src/display/layers/implementation/gLayerCity.h
        src/sim/structure/grids/transformation/gBaseToPattern.h
        src/sim/structure/grids/transformation/gBaseToBorderDetection.h
        src/sim/roads/rNodeFromGrid.h
        src/common/RelPath.h
        src/display/menus/rIMenu.h
        src/display/menus/implementation/rSelOptMenu.cpp
        src/display/menus/implementation/rSelOptMenu.h
        src/sim/roads/transportation/gUniqueId.h
        src/sim/roads/transportation/rToFollowDest.h
        src/sim/roads/node/RNodeAttrs/rRMailbox.h
        src/sim/roads/node/rInfoDists.h
        src/display/CameraControls.h
        src/sim/structure/grids/transformation/gBaseToWFC.h
        src/sim/structure/grids/transformation/gBaseToRiver.h
        src/sim/structure/grids/transformation/gBaseToLineRoads.h
        SimInitialize.h
#        SimInitializePreb.h
        src/sim/roads/transportation/rActiveVehicles.h
        src/display/menus/implementation/rBaseMenu.h
        src/display/layers/implementation/gLayerTransit.h
        src/display/rRemoteUpdateGrid.h
        src/display/menus/implementation/rRoadViewMenu.h
        src/display/menus/implementation/rCellViewMenu.h
        src/sim/structure/obj/sCommon.h
        src/sim/sMainSimulator.h
        src/display/transformations/gTransSimToDisp.h
        src/sim/structure/grids/transformation/sGridToSimulator.h
        src/sim/structure/obj/sTotalCompany.h
)

#        src/tests/tst_Reader.h
#        src/tests/tst_TransRNodeRRNode.h
#        src/tests/tst_gBasicGrid.h
#        src/tests/tst_gBtoGM.h
#        src/tests/tst_gGaussian.h
#        src/tests/tst_gBasicDisplay.h
#        src/tests/tst_gBasicPattern.h
#        src/tests/tst_gGenericKernel.h
#        src/tests/tst_gExtractRoads.h
#        src/tests/tst_gExtractRoadsCompressed.h
#        src/tests/tst_gMenus.h
#        src/tests/tst_gERCComm.h
#        src/tests/tst_gBasicRiver.h
#        src/tests/tst_gBasicWFC.h
#        src/tests/tst_gBasicWFCBasic.h
#        src/tests/tst_gBasicDisplayGround.h
#        src/tests/tst_gVeh.h)

target_link_libraries(CityOfWeirdFishes sfml-graphics sfml-window sfml-system)