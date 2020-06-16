import QtQuick 2.0

Item {
    signal clicked(bool isclicked)
    id: idRoot
    ButtonBase{
        id:idButton
        anchors.fill: idRoot
        text: "Hello"
        onClicked: idRoot.clicked(isclicked)
    }
}
