import QtQuick 2.12
import QtQuick.Layouts 1.12


Item {
    id: control

    width: parent.ListView.view.width
    height: Math.max(profileHeight,
                     content_item.height)

    //profile picture头像
    property int profileHeight: 32
    property int messageHeight: 40

    property int leftWidth: 73
    property int rightWidth: 73
    property int contentWidth: Math.max(10,control.width-control.leftWidth-control.rightWidth)
    property bool isUser: model.sender==="local"//false//(model.user===model.sender)
    property color messageBgColor: control.isUser?"#98E892":"#AAAAFF"//"#FFFFFF"

    default property alias contentItems: content_item.children
    property var isHex:true
    function hexToUtf8(hex){
       var utf8 = ""
       for(var i = 0 ; i<hex.length;i+=2)
       {
           var code = parseInt(hex.substr(i,2),16)
       
        if(code<128){
            utf8+=String.fromCharCode(code);

        }
        else if(code<224){
            var code2 = parseInt(hex.substr(i+2,2),16)
            utf8+=String.fromCharCode(((code&0x1F)<<6)|(code2&0x3f));
            i+=2;
        }
        else if(code<240)
        {
            var code2 = parseInt(hex.substr(i+2,2),16)
            var code3 = parseInt(hex.substr(i+4,2),16)
            utf8+=String.fromCharCode(((code&0x0F)<<12)|((code2&0x3F)<<6)|(code3&0x3f));
            i+=4;

        }
        else {
            var code2 = parseInt(hex.substr(i+2,2),16)
            var code3 = parseInt(hex.substr(i+4,2),16)
            var code4 = parseInt(hex.substr(i+6,2),16)
                utf8+=String.fromCharCode(((code&0x07)<<18)|((code2&0x3F)<<12)|(code3&0x3f)<<6)|(code4&0x3f);
            i+=6;
        }
        }
        return utf8
    }

    Item{
        id: left_item
        height: control.height
        width: control.leftWidth
        //目前头像为固定，动态值可以放model
        Image {
            visible: !control.isUser
            width: control.profileHeight
            height: control.profileHeight
            anchors.horizontalCenter: parent.horizontalCenter
            source: control.isUser?"image/profile_48_gray.png":"image/COM.png"
            //fillMode: Image.PreserveAspectFit
        }
    }

    Column{
        id: content_item
        x: control.leftWidth
        width: control.contentWidth
        spacing: 6

        //ColumnLayout在拉伸时计算有问题，暂时用Column+Row
        Row{
            width: control.contentWidth
            layoutDirection: control.isUser?Qt.RightToLeft:Qt.LeftToRight
            Rectangle{
                id: wrap_item
                radius: 4
                width: text_item.width
                height: text_item.height
                color: control.messageBgColor

                //指向发言人小三角
                Rectangle{
                    width: 10
                    height: 10
                    y: control.messageHeight/2-10
                    anchors.horizontalCenter: control.isUser?parent.right:parent.left
                    rotation: 45
                    color: control.messageBgColor
                }

                MyText{
                    id: text_item
                    text: isHex==true?model.data:hexToUtf8(model.data)//false//(model.user===model.sender)
                    //text: "blablabla"
                    width: Math.min(control.contentWidth,textWidth)
                }
            }
        }

        Row{
            width: control.contentWidth
            layoutDirection: control.isUser?Qt.RightToLeft:Qt.LeftToRight
            MyText{
                font.pixelSize:10
                text: model.datatime//"2023-03-18 16:50"
                padding: 0
            }
        }


        //layoutDirection: control.isUser?Qt.RightToLeft:Qt.LeftToRight
    }
    Item {
        id: right_item
        height: control.height
        width: control.rightWidth
        anchors.right: parent.right
        //头像没必要放两个，可以动态锚定
        //但是区分开后可以写一些差异化的操作
        Image {
            visible: control.isUser
            width: control.profileHeight
            height: control.profileHeight
            anchors.horizontalCenter: parent.horizontalCenter
            source: "image/profile_48_gray.png"
            //fillMode: Image.PreserveAspectFit
        }
    }

    //Component.onCompleted: {
    //    console.log('init',index);
    //}
    //Component.onDestruction: {
    //    console.log('free',index);
    //}
}
