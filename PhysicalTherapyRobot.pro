#sudo apt-get install libglu1-mesa-dev freeglut3-dev

TEMPLATE = app

QT += quick network concurrent widgets
CONFIG += c++11

SOURCES += \
    *.cpp

#OTHER_FILES += \
#    main.qml \
#    AdsMaterial.qml \
#    AdsEffect.qml \
#    ShadowMapLight.qml \
#    ShadowMapFrameGraph.qml \
#    Lf0.qml \
#    Lf1.qml \
#    Lf2.qml \
#    Lf3.qml \
#    Lf4.qml \
#    Lf5.qml \
#    Lf6.qml \
#    Lf7.qml \
#    Lf8.qml \
#    SceneRoot.qml

RESOURCES += \
    *.qrc

HEADERS += \
*.h

INCLUDEPATH += \
#    /home/s/robot_am/GTEngine-master/Include \
#    /home/s/opencv450/ubuntu-18.04/x86_64/include/opencv4 \
#    /home/s/OrbbecSDK_v1.8.3/SDK/include \
#    /home/s/robot_am/onnxruntime/include
     E:\workspace\PhysicalTherapyRobot\OrbbecSDK_v1.10.12\SDK\include\libobsensor\hpp \
     E:\workspace\PhysicalTherapyRobot\GTEngine-master\Include \
     E:\workspace\PhysicalTherapyRobot\OrbbecSDK_v1.10.12\SDK\include \
##     E:\workspace\PhysicalTherapyRobot\opencv450\win\include \
     E:\workspace\PhysicalTherapyRobot\opencv430\win\include \
     E:\workspace\PhysicalTherapyRobot\opencv430\win\include\opencv2 \
     E:\workspace\PhysicalTherapyRobot\onnxruntime\v1.12.1\win\x64\include

LIBS += \
#    /home/s/opencv450/ubuntu-18.04/x86_64/lib/libopencv_world.so \
#    /home/s/opencv450/ubuntu-18.04/x86_64/lib/libopencv_img_hash.so \
#    /home/s/OrbbecSDK_v1.8.3/SDK/lib/libOrbbecSDK.so \
#    /home/s/OrbbecSDK_v1.8.3/SDK/lib/libdepthengine.so \
#    /home/s/robot_am/onnxruntime/lib/libonnxruntime.so
#     E:/workspace/PhysicalTherapyRobot/opencv450/win/x64-gpu/vc14/lib/opencv_world450.lib \
#     E:/workspace/PhysicalTherapyRobot/opencv450/win/x64-gpu/vc14/lib/opencv_img_hash450.lib \
      E:/workspace/PhysicalTherapyRobot/opencv430/win/x64/vc14/lib/opencv_img_hash430d.lib \
      E:/workspace/PhysicalTherapyRobot/opencv430/win/x64/vc14/lib/opencv_world430d.lib \
      E:/workspace/PhysicalTherapyRobot/OrbbecSDK_v1.10.12/SDK/lib/OrbbecSDK.lib \
#      E:/workspace/PhysicalTherapyRobot/OrbbecSDK_v1.10.12/SDK/lib/depthengine_2_0.dll \
      E:/workspace/PhysicalTherapyRobot/onnxruntime/v1.12.1/win/x64/lib/onnxruntime.lib


FORMS += \
*.ui
