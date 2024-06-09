import QtQuick
import QtQuick3D

Node {
    property double rotation1: 0.0
    property double rotation2: 0.0
    property double rotation3: 0.0
    property double rotation4: 0.0
    property double rotation5: 0.0
    property double rotation6: 0.0
    property double clawsAngle: 0.0

    eulerRotation.x: -90
    scale.x: 2
    scale.y: 2
    scale.z: 2
    // Materials
    DefaultMaterial {
        id: steel___Satin_material
        diffuseColor: "#ffa0a0a0"
    }
    // end of Materials

    Node {
        id: rootNode
        Node {
            id: roboterarm_v4
            Node {
                id: fu__v2_1
                x: -4.70957
                y: -1.53218
                z: 0.458942
                Node {
                    id: fu__v2
                    Model {
                        id: body1
                        scale.x: 10
                        scale.y: 10
                        scale.z: 10
                        source: "meshes/body1.mesh"
                        materials: [
                            steel___Satin_material
                        ]
                    }
                }
            }
            Node {
                eulerRotation.z: rotation1
                id: j1_Axe_v3_1
                x: -4.70957
                y: -1.53218
                z: 65.4589
                Node {
                    id: j1_Axe_v3
                    Model {
                        id: body1_1
                        scale.x: 10
                        scale.y: 10
                        scale.z: 10
                        source: "meshes/body1_1.mesh"
                        materials: [
                            steel___Satin_material
                        ]
                    }
                }

                Node {
                    eulerRotation.x: rotation2
                    id: j2___J3_Axe_v3_1
                    x: -39.7096 - parent.x
                    y: -1.53218 - parent.y
                    z: 130.459 - parent.z
                    Node {
                        id: j2___J3_Axe_v3
                        Model {
                            id: body5
                            scale.x: 10
                            scale.y: 10
                            scale.z: 10
                            source: "meshes/body5.mesh"
                            materials: [
                                steel___Satin_material
                            ]
                        }
                    }

                    Node {
                        eulerRotation.x: rotation3
                        id: j3___J4_Axe_v4_1
                        x: -29.7096 - parent.x - parent.parent.x
                        y: 76.8095 - parent.y - parent.parent.y
                        z: 241.521 - parent.z - parent.parent.z
                        Node {
                            id: j3___J4_Axe_v4
                            Model {
                                id: body1_2
                                scale.x: 10
                                scale.y: 10
                                scale.z: 10
                                source: "meshes/body1_2.mesh"
                                materials: [
                                    steel___Satin_material
                                ]
                            }
                        }

                        Node {
                            eulerRotation.y: rotation4
                            id: j4___J5_Axe_v2_1
                            x: -29.7096 - parent.x - parent.parent.x - parent.parent.parent.x
                            y: 49.2002 - parent.y - parent.parent.y - parent.parent.parent.y
                            z: 241.521 - parent.z - parent.parent.z - parent.parent.parent.z
                            rotation: Qt.quaternion(0.707107, 0, 0, -0.707107)
                            Node {
                                id: j4___J5_Axe_v2
                                Model {
                                    id: body1_3
                                    scale.x: 10
                                    scale.y: 10
                                    scale.z: 10
                                    source: "meshes/body1_3.mesh"
                                    materials: [
                                        steel___Satin_material
                                    ]
                                }
                            }

                            Node {
                                eulerRotation.x: rotation5
                                id: j5___J6_Axe_v4_1
                                x: -4.70957 - parent.x - parent.parent.x - parent.parent.parent.x - parent.parent.parent.parent.x
                                y: -60.7998 - parent.y - parent.parent.y - parent.parent.parent.y - parent.parent.parent.parent.y
                                z: 241.521 - parent.z - parent.parent.z - parent.parent.parent.z - parent.parent.parent.parent.z
                                rotation: Qt.quaternion(0.707107, 0, 0, -0.707107)
                                Node {
                                    id: j5___J6_Axe_v4
                                    Model {
                                        id: body1_4
                                        scale.x: 10
                                        scale.y: 10
                                        scale.z: 10
                                        source: "meshes/body1_4.mesh"
                                        materials: [
                                            steel___Satin_material
                                        ]
                                    }
                                }

                                Node {
                                    eulerRotation.x: rotation6
                                    id: j6_Axe_v3_1
                                    x: -4.70957 - parent.x - parent.parent.x - parent.parent.parent.x - parent.parent.parent.parent.x - parent.parent.parent.parent.parent.x
                                    y: -60.7998 - parent.y - parent.parent.y - parent.parent.parent.y - parent.parent.parent.parent.y - parent.parent.parent.parent.parent.y
                                    z: 231.521 - parent.z - parent.parent.z - parent.parent.parent.z - parent.parent.parent.parent.z - parent.parent.parent.parent.parent.z
                                    rotation: Qt.quaternion(-4.37114e-08, 1, 0, 0)
                                    Node {
                                        id: j6_Axe_v3
                                        Model {
                                            id: body1_5
                                            scale.x: 10
                                            scale.y: 10
                                            scale.z: 10
                                            source: "meshes/body1_5.mesh"
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
    }
}
