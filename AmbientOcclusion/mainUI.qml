import QtQuick 2.0
import QtQuick.Controls 1.1
import com.window.launcher 1.0
Column {
    spacing: 2
    Launcher
    {
         id:launcher
    }
    Repeater {
        model: [ "Render Point1 Texture",
                        "Render Point2 Texture",
                        "Render Point3 Texture",
                        "Render Normal Texture",
                        "Calculate Normals From Textures",
                        "Show AO output",
                        "Toogle Rotation",
                         "Show Normals as vectors",


        ]
        Button {
            width: 300
            height: 32
            text: modelData + ' (' + index + ')'
            onClicked: launcher.clicked(index)
        }
    }
}
