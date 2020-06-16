import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
Rectangle
{
    property alias text:idText.text
    property string icoNormal: "image/fileinfo-128.png"
    property string icoHover: "image/sfileinfo-128.png"

    signal clicked(bool isclicked);

    id: idButton

    border.color: "#a0a0a0"
    border.width: idMouseArea.containsMouse?(idMouseArea.pressed?3:2):1
    smooth: true
    radius: 5
    clip: true
    color: idMouseArea.containsMouse?(idMouseArea.pressed?"#e0e0e0":"#e5e5e5"):"#f0f0f0"
    Text {
        id: idText
        text: parent.btnName
        anchors.centerIn: parent
    }
    Image {
        id: idImage
        fillMode: Image.PreserveAspectFit
        source: idMouseArea.containsMouse?icoHover:icoNormal
        width: idText.height*1.5
        height: idText.height*1.5
        x:idButton.width/6-width/2
        y:idButton.height/2-height/2
    }
    MouseArea{
        id:idMouseArea
        hoverEnabled: true
        anchors.fill: idButton
        onClicked: idButton.clicked(true)
    }

}
