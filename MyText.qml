import QtQuick 2.12
import QtQuick.Controls 2.12


TextEdit {
    id: control

    property int textWidth: private_text.implicitWidth

    padding: 12
    font{
        family: "Microsoft YaHei"
        pixelSize: 14
    }
    color: "#666666"

    verticalAlignment: TextEdit.AlignVCenter
    horizontalAlignment: TextEdit.AlignLeft
    readOnly: true

    selectByMouse: true
    selectionColor: "black"
    selectedTextColor: "white"
    wrapMode: TextEdit.WrapAnywhere
    //background: Item{ }

    //用TextEdit的implicitWidth/contentWidth来算width总是会Binding loop
    //故拿一个Text计算文本宽度用于判断
    Text{
        id: private_text
        visible: false
        font: control.font
        padding: control.padding
        text: control.text
    }
}
