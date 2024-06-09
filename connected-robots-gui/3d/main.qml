import QtQuick
import QtQuick3D
import Qt3D.Core 2.15
import QtQuick3D.Helpers

import RobotArm 1.0

View3D {
    anchors.fill: parent
    camera: camera

    environment: SceneEnvironment {
        clearColor: "black"
        backgroundMode: SceneEnvironment.Color
        antialiasingQuality: SceneEnvironment.VeryHigh
        antialiasingMode: SceneEnvironment.MSAA
    }

    DirectionalLight {
        id: light1
        eulerRotation.x: -15
        eulerRotation.y: 0
        brightness: 1.0
    }

    DirectionalLight {
        id: light2
        eulerRotation.x: -15
        eulerRotation.y: 120
        brightness: 1.0
    }

    DirectionalLight {
        id: light3
        eulerRotation.x: -15
        eulerRotation.y: 240
        brightness: 1.0
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(250, 300, 250)
        eulerRotation.x: -15
        eulerRotation.y: 45
        fieldOfView: 100.0
        fieldOfViewOrientation: PerspectiveCamera.Horizontal
    }

    RobotArm {
        objectName: "robotArm"
    }

    AxisHelper {
        enableAxisLines: false
        gridOpacity: 0.7
        gridColor: "green"
    }

    WasdController {
        controlledObject: camera
    }
}
