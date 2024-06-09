import QtQuick
import QtQuick3D

Node {
    property double rotation1: 0.0
    property double rotation2: 0.0
    property double rotation3: 90.0
    property double rotation4: 0.0
    property double rotation5: 45.0
    property double rotation6: -30.0
    property bool clawsClosed: false

    // Materials
    DefaultMaterial {
        id: steel___Satin_material
        diffuseColor: "#ffa0a0a0"
    }
    // end of Materials

    Model {
        id: base
        rotation: Qt.quaternion(0.707107, -0.707107, 0, 0)
        scale.x: 100
        scale.y: 100
        scale.z: 100
        source: "meshes/base.mesh"
        materials: [
            steel___Satin_material
        ]
        Model {
            id: root
            eulerRotation.z: rotation1
            x: -3.72529e-09
            y: -1.86265e-09
            z: 0.65
            source: "meshes/root.mesh"
            materials: [
                steel___Satin_material
            ]
            Model {
                id: arm1
                eulerRotation.x: rotation2
                x: -5.58794e-08
                y: 0.00155703
                z: 0.657009
                source: "meshes/arm1.mesh"
                materials: [
                    steel___Satin_material
                ]
                Model {
                    id: arm2
                    eulerRotation.x: rotation3
                    y: 0.000437967
                    z: 1.34827
                    source: "meshes/arm2.mesh"
                    materials: [
                        steel___Satin_material
                    ]
                    Model {
                        id: arm3
                        eulerRotation.z: rotation4
                        x: 4.10213e-08
                        y: 0.197478
                        z: 0.279813
                        source: "meshes/arm3.mesh"
                        materials: [
                            steel___Satin_material
                        ]
                        Model {
                            id: arm4
                            eulerRotation.x: rotation5
                            x: 3.68214e-09
                            y: 0.000703633
                            z: 1.1
                            source: "meshes/arm4.mesh"
                            materials: [
                                steel___Satin_material
                            ]
                            Model {
                                eulerRotation.z: rotation6
                                id: clawRoot
                                x: 1.3411e-07
                                y: 0.000191689
                                z: 0.3
                                source: "meshes/clawRoot.mesh"
                                materials: [
                                    steel___Satin_material
                                ]
                                Model {
                                    id: claw1
                                    x: -0.15 + (clawsClosed ? 0.15 : 0)
                                    y: 9.59337e-05
                                    z: 0.150001
                                    source: "meshes/claw1.mesh"
                                    materials: [
                                        steel___Satin_material
                                    ]
                                }
                                Model {
                                    id: claw2
                                    x: 0.15 - (clawsClosed ? 0.15 : 0)
                                    y: 9.57549e-05
                                    z: 0.150001
                                    source: "meshes/claw2.mesh"
                                    materials: [
                                        steel___Satin_material
                                    ]
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
