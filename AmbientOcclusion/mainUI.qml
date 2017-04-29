import QtQuick 2.0
import QtQuick.Controls 1.1
import com.window.launcher 1.0
/*Rectangle {

   Launcher
   {
        id:launcher
   }
  /* width: 300
   height: 1024
   Button {
        id:button1
        width: parent.width
        height: 35
        text: "Toogle Rotation"
        onClicked: launcher.clicked(0)
    }
    Button {
         y :35
        width: parent.width
        height: 35
        text: "Toogle SecondPass"
        onClicked: launcher.clicked(1)
    }
    Button {
        y:70
        width: parent.width
        height: 35
        text: "Show Normals"
        onClicked: launcher.clicked(2)
    }
    Button {
        y:105
        width: parent.width
        height: 35
        text: "Render Point1 Texture"
        onClicked: launcher.clicked(2)
    }
    Button {
        y:105
        width: parent.width
        height: 35
        text: "Render Point2 Texture"
        onClicked: launcher.clicked(2)
    }
    Button {
        y:105
        width: parent.width
        height: 35
        text: "Render Point3 Texture"
        onClicked: launcher.clicked(2)
    }
    Button {
        y:105
        width: parent.width
        height: 35
        text: "Render Normals Texture"
        onClicked: launcher.clicked(2)
    }

    ListModel {
        id: contactsModel
        ListElement {
            name: "Bill Smith"
        }
        ListElement {
            name: "John Brown"
        }
        ListElement {
            name: "Sam Wise"
        }
    }
    ListView{
               id: contactsView
              anchors.left: parent.left
              anchors.top: parent.top
              width: parent.width
              height: parent.height
              orientation: Qt.Vertical
              spacing: 10
              model: contactsModel

    }
}
*/Column {
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
