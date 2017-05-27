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
                        "Ray-Tri intersection with current point
    (White=hit,Green=miss)",
                        "Toogle Rotation",
                         "Show Normals as vectors",
                        "Sho AO output",
        ]




        Button {
            width: 300
            height: 32
            text: modelData + ' (' + index + ')'
            onClicked: launcher.clicked(index)

        }

    }
    Text {
        width:300
        height:64
        Slider{
                y:15
                id: epsilon
                minimumValue: -0.0001
                maximumValue: 0.0
                stepSize: 0.00001
                tickmarksEnabled: true
                width:300
                height:32
                onValueChanged: launcher.epsilonValue(epsilon.value)
        }
        text: qsTr("Slider is Epsilon Value used in AO Pass:"+epsilon.value)
    }

    Text {
        width:300
        height:64
        Slider{
                y:15
                id: stepSize
                minimumValue:0.0
                maximumValue: 2.0
                stepSize: 0.1
                tickmarksEnabled: true
                width:300
                height:32
                onValueChanged: launcher.stepValue(stepSize.value)
        }
        text: qsTr("Slider is StepSize Value used in AO Pass:"+stepSize.value)
    }


}
