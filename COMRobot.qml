import QtQuick 
import ProxyItem

 ProxyItem {
    id:proxyItem
    
    Component.onCompleted:{
       
    }
    property string thenReplay: ""
    property string whenParsed:"aa550100"

    function onParsed(data)
    {
        console.log("parsed:",data)
        if(data == whenParsed)//will reply automatically when ${whenParsed} parsed
        { 
            if(thenReplay!="")
            {
                var request = {}             
                request["data"] = thenReplay
                proxyItem.invoke("write",request)
            }       
        }
    }
    //
    onRequest:function(method,request){
        if(method == "read")
        {
            var data = request["data"]
            parse(data)       
        }
    }
    property var buf:""

    /////////////////////demo format//////////////////////////////
    //  |  header |command code|data length|data1|data2|...|DATAN|
    //  |0xaa 0x55|    CMD     |    N      |Data1|Data2|...|DataN|
    function parse(data){
        buf+=data
        while(buf.length>=8)
        {
            if(buf.substr(0,4) == "aa55")//header
            {
                var len = buf.substr(6,2)
                len = parseInt("0x"+len)*2 + 8
                if(buf.length>=len)
                {
                    var parsed = buf.substr(0,len);
                    var whould_be_left = buf.length - len
                    buf = buf.substr(len,whould_be_left)
                    onParsed(parsed)
                }
                else
                {
                    break;//data was not completed ,wait for next data 
                }
            }
            else{
                 var head = buf.substr(0,2);
                 console.log("header unsupported:"+head)
                 buf = buf.substr(2,buf.length-2);
            }
        }
        if(buf.length!=0)
        {
            console.log("unparsed:",buf)
        }
    }
}