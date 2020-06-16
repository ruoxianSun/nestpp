import QtQuick 2.0
Item{
    id: idRoot
    signal clicked(bool f)
    ButtonBase{
        id:idButton
        anchors.fill: idRoot
        text: "MessageBox(qml)"
        onClicked: idRoot.clicked(isclicked)
    }
}
