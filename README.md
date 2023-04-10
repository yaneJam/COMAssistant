# COMAssistant
 a flexible serial port tool that can be reprogramed by qml ,cpp or diagram to make it perfect for you

# COMssistant 是什么？
>COMAssiant是一个可编程的串口调试工具，可以使用qml+cpp+状态事件图三种形式来更改它，同时示例提供了一个自动回复的聊天机器人，您可以自由的更改其中任何的部分,包括通信协议，界面逻辑等，让它成为一个合适您的业务的调试仿真工具

![图片](https://github.com/yaneJam/COMAssistant/raw/main/image/Chat.png)

# 如何在我的PC电脑上运行它
>如果需要编译cpp模块的代码，需要安装Visual Studio 2019

>下载yane.exe绿色版本，下载完后直接点击start.bat运行
[百度网盘](https://pan.baidu.com/s/1zNYYaz9_2bpl-U5ZKHxraA )
提取码：u7n5

>导入COMAssistant
点击左上方的打开按钮，注意此时代码已被导入到yane内部的路径"./lib/COMAssistant"

>运行COMAssistant
点击左下角的Menu，然后选择COMAssisttant

# 如何通过qml语言来更改它的界面和逻辑
>在Sequance打开COMAssitant后，点击sublime，即可以浏览和编辑COMAssitant的qml代码
>界面上采用了聊天的对话框风格(main.qml)，你可以使用qml语言来更改成你想要的界面
同时示例了一个聊天机器人（COMRobot.qml）也是使用qml来编程，对串口数据流进行砍包解包的，然后通过判断自动恢复，您可以参考该写法增加其他的qml模块


# 如何通过状态事件图来增加更多的消息相应
>事件图使用了隐式调用的设计模式，你可以通过绘制事件图来增加更多的事件响应
>状态的切换可以开启和关闭不同的事件图，从而实现切换，在不同的状态中开关不同的事件响应
![图片](https://github.com/yaneJam/COMAssistant/raw/main/image/Sequence.png)


# 如何编译cpp代码
>需要安装VisualStudio 2019
>左下角的Menu，点击“日志”，在弹出的对话框中看到编译过程的输出
>在Sequance导入或打开COMAssitant后，点击sublime，即可以在src文件夹中浏览和编辑COMAssitant的cpp代码
>点击build，即可以编译cpp代码，会在bin文件夹生成打包好的二进制包，会生成中间文件夹prj和inc
>如果编译不成功，可以用VisualStudio打开./lib/COMAssistant/prj/COMAssistant.sln 查看
