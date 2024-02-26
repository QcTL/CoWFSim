cmake_minimum_required(VERSION 3.27)
project(CityOfWeirdFishes)

set(CMAKE_CXX_STANDARD 17)



if(WIN32)
include_directories("C:/Program Files/SFML-2.6.1/include")
link_directories("C:/Program Files/SFML-2.6.1/lib")
elseif(UNIX AND NOT APPLE)
include_directories("/home/ganymede/lib/SFML-2.6.1/include")
link_directories("/home/ganymede/lib/SFML-2.6.1/lib")
endif()

add_executable(CityOfWeirdFishes main.cpp
        src/IO/ReaderParameters.h
        src/tests/tst_Reader.h
        src/sim/roads/rTransRNodeToRRNode.h
        src/sim/roads/node/rNode.h
        src/sim/roads/node/rRNode.h
        src/sim/structure/grids/transformation/gBaseToGradientMinimum.h
        src/tests/tst_TransRNodeRRNode.h
        src/sim/structure/grids/gIGrid.h
        src/sim/structure/grids/gBasicGrid.h
        src/tests/tst_gBasicGrid.h
        src/tests/tst_gBtoGM.h
        src/sim/structure/grids/transformation/gBasicTransformations.h
        src/tests/tst_gGaussian.h
        src/display/rGlobal.cpp
        src/display/rGlobal.h
        src/display/rPileMenus.h
        src/display/layers/gSimLayers.h
        src/display/layers/gILayer.h
        src/display/layers/tileset/gTileset.h
        src/display/layers/implementation/gLayerAirPollution.h
        src/display/layers/implementation/gLayerCity.h
        src/tests/tst_gBasicDisplay.h
        src/sim/structure/grids/transformation/gBaseToPattern.h
        src/sim/structure/grids/transformation/gBaseToBorderDetection.h
        src/tests/tst_gBasicPattern.h
        src/tests/tst_gGenericKernel.h
        src/sim/roads/rNodeFromGrid.h
        src/tests/tst_gExtractRoads.h
        src/common/RelPath.h
        src/tests/tst_gExtractRoadsCompressed.h
        src/display/menus/rIMenu.h
        src/display/menus/implementations/rSelOptMenu.cpp
        src/display/menus/implementations/rSelOptMenu.h
        src/tests/tst_gMenus.h
        src/sim/roads/transportation/gUniqueId.h
        src/sim/roads/transportation/rToFollowDest.h
        src/sim/roads/node/RNodeAttrs/rRMailbox.h
        src/sim/roads/node/rInfoDists.h
        src/tests/tst_gERCComm.h
        src/display/CameraControls.h
        src/sim/structure/grids/transformation/gBaseToWFC.h
        src/sim/structure/grids/transformation/gBaseToRiver.h
        src/tests/tst_gBasicRiver.h
        src/tests/tst_gBasicWFC.h
        src/tests/tst_gBasicWFCBasic.h
        SimInitialize.h
        src/sim/structure/grids/transformation/gBaseToLineRoads.h
        src/tests/tst_gBasicDisplayGround.h
        SimInitializePreb.h
        src/sim/roads/transportation/rActiveVehicles.h
        src/tests/tst_gVeh.h
        src/display/menus/implementations/rBaseMenu.h
        src/display/layers/implementation/gLayerTransit.h
        src/display/rRemoteUpdateGrid.h
        src/display/menus/implementations/rRoadViewMenu.h
        src/sim/structure/grids/gSinNodes.h
)

target_link_libraries(CityOfWeirdFishes sfml-graphics sfml-window sfml-system)