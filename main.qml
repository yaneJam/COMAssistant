import QtQuick 
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Universal  
//import QtQuick.Controls.Material  
import YQuick 1.0

import ProxyItem
Control {
	id:activeView
	width: parent.width
    height: parent.height
    //Material.theme: Material.Light
    //Material.background: Material.Teal
    //Material.accent: Material.color()

    Universal.theme: Universal.Olive
    Universal.accent: YConfig.themeColor
    //Universal.background: YConfig.themeColor
    //Universal.foreground: YConfig.themeColor
   
       
    Row {
       	width: parent.width
        height: parent.height
        anchors.centerIn: parent
        spacing: 0
        Rectangle {
            id:right_rect
            width: parent.width
            height: parent.height
            property bool showSend: false
            property bool repeatSend: false
            Column{
                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                          
                Rectangle{
                    id:title
                    height:50
                    width:parent.width
                    color:YConfig.themeColor
                    Row{
                        anchors.verticalCenter:parent.verticalCenter
                        Rectangle{
                            height:parent.height
                            width:5
                            radius:5
                            color:title.color
                        }
                        Rectangle{
                            height:35
                            width:35
                            radius:5
                            anchors.verticalCenter:parent.verticalCenter
                            color:YConfig.themeColor
                            
                            Image {
                                height:35
                                width:35
                                visible:true
                                anchors.fill:parent
                                source: "image/disconnected.png"   
                                id:img_disconnected
                            }
                            Image {
                                height:35
                                width:35
                                visible:false
                                anchors.fill:parent
                                id:img_connected
                                source: "image/connected.png"
                                RotationAnimation on rotation {
                                    from: 0
                                    to: 360
                                    running: true
                                    loops: Animation.Infinite
                                    duration: 2000
                                }
                            }
                        }
                        Label{
                            x:20
                            font.pixelSize : 20
                            anchors.verticalCenter:parent.verticalCenter
                            text: qsTr("串口调试助手")
                        }
                        
                        Rectangle{
                            height:parent.height
                            width:5
                            radius:5
                            color:title.color
                        }

                    }
                    
                    Rectangle {
                         color:YConfig.themeColor
                         width: 280
                         height: parent.height
                         anchors.right:parent.right
                         Grid {
                             anchors.verticalCenter:parent.verticalCenter
                             anchors.horizontalCenter:parent.horizontalCenter
                            columns: 4
                            spacing: 3

                            ComboBox {
                                width:80
                                id:cb_port
                                model: ListModel {
                                    id: cb_port_model
                                // ListElement { text: "COM1" }
                                // ListElement { text: "COM2" }
                                // ListElement { text: "COM3" }
                                }
                                onActivated: {
                                }
                            }
                            ComboBox {
                                id:cb_baudrate
                                width:80
                                model: [ "4800","9600", "19200", "38400", "57600","115200"]
                                onActivated: {
                                }
                            }
                            
                            Button{
                                id:btnConnect
                                width:80
                                text: "连接"
                                onClicked: {
                                    if(text == "连接")
                                    {
                                        var request = {}

                                        var port = cb_port.currentText;
                                        var baudrate = cb_baudrate.currentText
                                        console.log("port:"+port+" baudrate:"+baudrate)
                                        request["Port"] = port
                                        request["BaudRate"] = baudrate
                                        
                                        // var str_request = JSON.stringify(request)
                                        // toolpanel.invoke("connect",str_request)


                                        toolpanel.invoke("connect",request)

                                    }
                                    else if(text == "断开")
                                    {
                                         //var request = {}
                                        // var str_request = JSON.stringify(request)
                                        // toolpanel.invoke("disconnect",str_request)

                                         toolpanel.invoke("disconnect")
                                    }
                                }
                            }
                           

                        }
                    }
                }
               
                ListView {
                    id: messagelistview
                    height: parent.height-sendData.height-buttons.height -tools.height -title.height 
                    width: parent.width
                    property var isHex: cb_format.currentText =="Hex"
                    clip:true
                    header: Rectangle{
                        id:head
                        width:parent.width
                        height:10
                    }
                    footer: Rectangle{
                        id:head
                        width:parent.width
                        height:10
                    }
                    delegate:MyMessage{
                        isHex:messagelistview.isHex
                        height:70
                        width:messagelistview.width
                    }
                    model: ListModel {
                        id: messageList 
                        
                        // ListElement {
                        //     text: "aa 55 01 00"
                        //     datatime:"11:18:56:567"
                        // }  
                    }
                     //竖向滚动条
                    ScrollBar.vertical: ScrollBar {
                        id: scroll_vertical
                        contentItem: Item{
                            visible: (scroll_vertical.size<1.0)
                            implicitWidth: 10
                            Rectangle{
                                anchors.centerIn: parent
                                width: parent.width
                                height: parent.height>20?parent.height:20
                                color: (scroll_vertical.hovered||scroll_vertical.pressed)
                                    ? Qt.darker("#A4ACC6")
                                    : "#A4ACC6"
                            }
                        }
                    }
                }
                
                
                Rectangle{
                    id:tools
                    height:40
                    width:parent.width
                    color:YConfig.themeColor
                    Row{
                        anchors.verticalCenter:parent.verticalCenter
                        spacing:3
                        Rectangle{
                            width:3
                            height:parent.height
                            color:YConfig.themeColor
                        }
                        Button { 
                                text:"自动回复1"
                                onClicked:{
                                    //when recieved and parsed ${whenParsed} then robot will reply ${thenReplay} automatically
                                    robot.whenParsed ="aa550100"
                                    robot.thenReplay="aa 55 01 02 02 00"
                                    //robot.thenReplay = "0xaa 0x55 0x01 0x02 0x02 0x00"//this format will do
                                    //robot.thenReplay = "aa5501020100"//this format  will do
                                }
                        }
                        Button { 
                            text:"自动回复2"
                            onClicked:{
                                robot.whenParsed = "aa550100"
                                robot.thenReplay="aa 55 01 02 01 00"
                            }
                        }
                        Button { 
                            text:send_timer.running == false?"定时发送":"停止"
                            Timer {
                                id: send_timer
                                interval: 1000//100ms
                                repeat: true
                                triggeredOnStart: false
                                running: false
                                onTriggered: {
                                    var request = {}
                                    request["data"] = "aa550100"//utf8ToHex("你好")//"aa550100"
                                    //var str_request = JSON.stringify(request)
                                    //toolpanel.invoke("send",str_request)
                                    toolpanel.invoke("send",request)

                                }
                            }
                             onClicked:{
                                 if(send_timer.running == false)
                                 {
                                    send_timer.start()
                                 }
                                 else{
                                    send_timer.stop()
                                 }
                              
                            }
                        }
                        Button { 
                            text:"测试按钮4"
                            onClicked:{
                               var request = {}
                                request["data"] = "aa55020100"
                                //var str_request = JSON.stringify(request)
                                //toolpanel.invoke("send",str_request)

                                toolpanel.invoke("send",request)
                            }
                        }
                        Button { 
                            text:"测试按钮5"
                            onClicked:{
                                var request = {}
                                request["data"] = "aa55020101"
                                //var str_request = JSON.stringify(request)
                                //toolpanel.invoke("send",str_request)


                                 toolpanel.invoke("send",request)
                            }
                        }
                        Button { 
                            text:"测试按钮6"
                            onClicked:{
                               robot.whenParsed = ""
                               robot.thenReplay = ""
                            }
                        }
                    }
                }
                Row{
                    width: parent.width
                    height: 100
                    ScrollView {
                        id: sendData
                        width: parent.width
                        height: 100
                        clip: true
                        ScrollBar.horizontal: ScrollBar{ visible: false }
                      
                        TextArea{
                            id: textarea_1
                            width: sendData.width
                            padding: 15
                            color: "black"
                            placeholderTextColor: "gray"
                            selectByMouse: true
                            selectByKeyboard: true
                            selectedTextColor: "white"
                            selectionColor: "black"
                            font{
                                family: "SimSun"
                                pixelSize: 16
                            }
                            //截取超出部分
                            //clip: true
                            //默认Text.QtRendering看起来比较模糊
                            renderType: Text.NativeRendering
                            //文本换行，默认NoWrap
                            wrapMode: TextEdit.Wrap
                        }
                    }
                    
                   
                }
                    Rectangle
                    {
                        id:buttons
                        height:40
                        width:parent.width
                        color:YConfig.themeColor
                        Row{
                        spacing :3
                    
                        anchors.verticalCenter:parent.verticalCenter
                        anchors.right: parent.right
                        Rectangle{
                            width:50
                            height:parent.height
                            color:YConfig.themeColor
                        }
                        ComboBox {
                            id:cb_format
                            width:80
                            model: [ "Hex","UTF8"]
                            onActivated: {
                                //文字提示
                                if(currentText=="Hex"){
                                    textarea_1.placeholderText= "示例:\n0xaa,0x55,0x30,0x01,0x01\n0xaa 0x55 0x30 0x01 0x01\naa 55 01 02 02 00\naa55300101"
                                }
                                else{
                                    textarea_1.placeholderText= "请输入文字"
                                }
                            }
                            Component.onCompleted:{
                                if(currentText=="Hex")
                                {
                                    textarea_1.placeholderText= "示例:\n0xaa,0x55,0x30,0x01,0x01\n0xaa 0x55 0x30 0x01 0x01\naa 55 01 02 02 00\naa55300101"
                                }
                                else{
                                    textarea_1.placeholderText= "请输入文字"
                                }
                            }
                        }
                        Button{
                            id:btnSend
                            width:80
                            anchors.verticalCenter:parent.verticalCenter
                            text: "发送"
                            enabled : false
                            onClicked: {
                                var request = {}

                                if(cb_format.currentText=="UTF8")
                                {
                                    console.log("text:"+textarea_1.text)
                                    var hex =  utf8ToHex(textarea_1.text)
                                    request["data"] = hex
                                    console.log("hex:"+hex)
                                }
                                else
                                {
                                    request["data"] = textarea_1.text
                                }                                
                                //var str_request = JSON.stringify(request)
                                //toolpanel.invoke("send",str_request)
                                toolpanel.invoke("send",request)

                            }
                        }
                        
                        Rectangle{
                            width:20
                            height:parent.height
                            color:YConfig.themeColor
                        }
                    }
                }
            }
            
        }
    }

    ProxyItem {
        id:toolpanel
        Component.onCompleted:{
            toolpanel.registerYModule("toolpanel")
            cb_port_model.clear()
            var response = toolpanel.invoke("getCOMContext")
            var SerialComm = response["SerialComm"]

            
            for(var i = 0;i <SerialComm.length;i++)
            {   
                cb_port_model.append({text:SerialComm[i]})
            }
          cb_port.currentIndex =0
        }
        onRequest:function(method,request){
            console.log(method)
            if(method == "messageRecieved")
            {
                var data = request["data"]
                var datetime = request["datetime"];
                messageList.append({data: data,sender: "remote",datatime:datetime})
                messagelistview.positionViewAtEnd()

            }
            else if(method == "messageSend")
            {
               var data = request["data"]
               var time = new Date()
               //
               var datetime = time.toLocaleTimeString()+":"+time.getMilliseconds();
                messageList.append({data: data,sender: "local",datatime:datetime})
                messagelistview.positionViewAtEnd()
            }
            else if(method == "connected")
            {
                btnConnect.text = "断开"
                btnSend.enabled = true
                img_connected.visible = true 
                img_disconnected.visible = false
            }
            else if(method == "disconnected")
            {
                btnConnect.text = "连接"
                btnSend.enabled = false
                img_connected.visible = false
                img_disconnected.visible = true
            }
        }
     }
     COMRobot{
        id:robot 
        Component.onCompleted:{
            robot.registerYModule("robot")//need to register to recieved signal 
        }
     }
   Component.onCompleted: {      
        cppProxy.startCppSequence("COMAssistant.cfg")
        YConfig.currentTheme = 6
        window.width = 540
        window.height = 630
    }
    Component.onDestruction: {                    
        toolpanel.unRegisterYModule()
        robot.unRegisterYModule()
        cppProxy.stopCppSequence()  
    }

      // this function is generated by Bing AI  :)
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


    // this function is generated by Bing AI ,and noted by Bing AI ,i changed var bytes to hex. :)
    function utf8ToHex(str) {
        let hex = ''
        // 创建一个数组，用来存放字节序列
        //var bytes = [];
        // 遍历字符串中的每个字符
        for (var i = 0; i < str.length; i++) {
            // 获取当前字符的Unicode码点值
            var code = str.charCodeAt(i);
            // 判断当前字符需要几个字节来编码
            if (code < 0x80) { // 如果小于128（即ASCII码），则只需要一个字节
            //   bytes.push(code);
            hex+=code.toString(16)

            } 
            else if (code < 0x800) { // 如果小于2048（即扩展ASCII码），则需要两个字节
            //   // 第一个字节以110开头，后面跟着前5位码点值
            //   bytes.push(0xc0 | (code >> 6));
            //   // 第二个字节以10开头，后面跟着后6位码点值
            //   bytes.push(0x80 | (code & 0x3f));
            hex+=(0xc0 | (code >> 6)).toString(16)+
                (0x80 | (code & 0x3f)).toString(16)
            }
            else if (code < 0xd800 || code >= 0xe000) { // 如果小于55296或大于等于57344（即基本多文种平面之外），则需要三个字节
            //   // 第一个字节以1110开头，后面跟着前4位码点值
            //   bytes.push(0xe0 | (code >> 12));
            //   // 第二个字节以10开头，后面跟着中间6位码点值
            //   bytes.push(0x80 | ((code >> 6) & 0x3f));
            //   // 第三个字节以10开头，后面跟着后6位码点值
            //   bytes.push(0x80 | (code & 0x3f));
            hex+=(0xe0 | (code >> 12)).toString(16)+ 
                (0x80 | ((code >> 6) & 0x3f)).toString(16)+
                (0x80 | (code & 0x3f)).toString(16)
            } 
            else 
            { // 如果在55296到57343之间（即代理对区域），则需要四个字节
            // 获取下一个字符的Unicode码点值，并组合成一个32位整数
            i++;
            code = ((code & 0x3ff) << 10) | (str.charCodeAt(i) & 0x3ff);
            code += 0x10000;
            // 第一个字节以11110开头，后面跟着前3位码点值
            //   bytes.push(0xf0 | (code >> 18));
            //   // 第二个字节以10开头，后面跟着第4到第9位码点值
            //   bytes.push(0x80 | ((code >> 12) & 0x3f));
            //   // 第三个字节以10开头，后面跟着第10到第15位码点值
            //   bytes.push(0x80 | ((code >> 6) & 0x3f));
            //   // 第四个字节以10开头，后面跟着第16到第21位码点值
            //   bytes.push(0x80 | (code & 0x3f));
            hex+=(0xf0 | (code >> 18)).toString(16)+ 
            (0x80 | ((code >> 12) & 0x3f)).toString(16)+
                (0x80 | ((code >> 6) & 0x3f)).toString(16)+
                (0x80 | (code & 0x3f)).toString(16)
            }   
        }
        // 返回转换好的字节数组
        //return bytes
        return hex
  
    }
}
