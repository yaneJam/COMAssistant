#include "stdafx.h"
//#include "Command.h"
#include "COMCommunication.h"
#include "COMCommunicatorImp.h"

COMCommunicator::COMCommunicator()
{
	imp = new COMCommunicatorImp(this);
    
}

COMCommunicator::~COMCommunicator()
{
	Close_Connect();
	delete imp;
	imp = NULL;
}

yModule::RESULT COMCommunicator::doSlot(_tsptr<yCMD> cmd)
{
	MACRO_doSlot(connect);
	MACRO_doSlot(disconnect);
	MACRO_doSlot(send);
    MACRO_doSlot(getContext);

}
yModule::RESULT COMCommunicator::getContext(_tsptr<yCMD>  cmd)
{
    GetComList_Reg();
    if (cmd && cmd->callback)
    {
        DEBUG_LOG("%s", m_context->asContent(false).c_str());
        cmd->callback(yCMD::SUCCESS, m_context);

    }
    return yModule::SUCCESS;
}

yModule::RESULT COMCommunicator::connect(_tsptr<yCMD>  cmd)
{
    if (cmd->request)
    {
        _tstring Port = (*cmd->request)["Port"].asString();
        _tstring BaudRate = (*cmd->request)["BaudRate"].asString();
        if (!Port.empty() && !BaudRate.empty())
        {
            connectting_callback = cmd;
            new yThread([=] {
                if (Init_Connect(Port, BaudRate) == TRUE)
                {
                    
                    cmd->callback(yCMD::SUCCESS, NULL);

                    _tcmd(connected);
                    emitSignal("connected", connected);
                }
                _tsptr<yValue> response(new yValue);
                (*response)["error"] = ANSI2UTF8(_tstring("failed"));
                cmd->callback(yCMD::FAILED, response);
                });
            return yModule::SUCCESS;
        }
    }
    else
    {
        _tsptr<yValue> response(new yValue);
        (*response)["error"] = ANSI2UTF8(_tstring("no request"));
        cmd->callback(yCMD::FAILED, response);
    }
    return yModule::FAILED;
   
}
yModule::RESULT COMCommunicator::disconnect(_tsptr<yCMD>  cmd)
{
    Close_Connect();
    _tcmd(disconnected);
    emitSignal("disconnected", disconnected);
    return yModule::SUCCESS;
}

yModule::RESULT COMCommunicator::send(_tsptr<yCMD>  cmd)
{
	auto data = (*cmd->request)["data"].asString();

    //砍掉无用字符
    _tstring compose;
    for (int i = 0; i < data.size(); i++)
    {
        bool bresult = true;
        char sz = data[i];
        if (sz >= '0' && sz <= '9');
        else if (sz >= 'a' && sz <= 'f');
        else if (sz >= 'A' && sz <= 'F');
        else  if (sz == 'x' && i >= 0 && data[i - 1] == '0');//"0x" 
        else
        {
            bresult = false;
        }
        if (bresult)
        {
            compose += sz;
        }
    }
	int lastindex = 0;
    unsigned char* hex = new unsigned char[compose.size() / 2];
    int len = 0;
	for (int i = 0; i < compose.size()/2; i++)
	{
        unsigned char  temp;
        bool result = false;
        char sz1 = compose[i*2];
        char sz2 = compose[i * 2 + 1];
        //先判断0x前缀
        if (sz2 >= '0' && sz2 <= '9')
        {
            temp = sz2 - '0';
            result = true;
        }
        else if (sz2 >= 'A' && sz2 <= 'F')
        {
            temp = sz2 - 'A' + 10;
            result = true;
        }
        else if (sz2 >= 'a' && sz2 <= 'f')
        {
            temp = sz2 - 'a' + 10;
            result = true;
        }
        else
        {
            if (sz2!='x')
            {
                ERROR_LOG("should not be here");
            }
        }

        if (result == true)
        {
            if (sz1 >= '0' && sz1 <= '9')
            {
                temp =  (sz1 - '0' << 4)+ temp;
            }
            else if (sz1 >= 'A' && sz1 <= 'F')
            {
                temp =   ((sz1 - 'A' + 10) << 4) + temp;
            }
            else if (sz1 >= 'a' && sz1 <= 'f')
            {
               temp = ((sz1 - 'a' + 10) << 4)+temp;
            }
            else
            {
                if (sz1 != 'x')
                {
                    ERROR_LOG("should not be here");
                }
                result = false;
            }
        }

        if (result == true)
        {
            hex[len] = temp;
            len++;
        }
	}
    if (len > 0)
    {
        imp->DirectlyWrite(hex, len);
    }

    return yModule::SUCCESS;
}


BOOL COMCommunicator::Init_Connect(std::string strPortName, std::string strBaudRate)
{
	return imp->Init_MainCtrlConnect(strPortName, strBaudRate);
}

void COMCommunicator::Close_Connect()
{
	imp->Close_MainCtrlConnect();
}


int  COMCommunicator::GetComList_Reg()
{
    HKEY hkey;
    int result;
    int i = 0;

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        _T("Hardware\\DeviceMap\\SerialComm"),
        NULL,
        KEY_READ,
        &hkey);


    _tsptr<yValue> context;
     __super::getContext(context);
     (*context).removeNode("SerialComm");
    if (ERROR_SUCCESS == result)   //   打开串口注册表   
    {
        TCHAR portName[0x100], commName[0x100];
        DWORD dwLong, dwSize;


        do
        {
            dwSize = sizeof(portName) / sizeof(TCHAR);
            dwLong = dwSize;
            result = RegEnumValue(hkey, i, portName, &dwLong, NULL, NULL, (LPBYTE)commName, &dwSize);
            if (ERROR_NO_MORE_ITEMS == result)
            {
                //   枚举串口
                break;   //   commName就是串口名字"COM4"
            }
            _tstring SerialComm = commName;
            
            (*context)["SerialComm"].pushBack(SerialComm);
      
           // cout << "当前串口号：" << commName[3] - '0' << endl; /*默认是字符型，需转换为阿拉伯数字*/
            i++;
        } while (1);

        RegCloseKey(hkey);
    }

    return i;
}



//
///*************************************************
//Function: 		hex2string
//Description: 	hex字符数组转换成string字符串，用于printf显示等
//Input: 			hex:原字符数组
//				len:hex数组的长度
//Output: 		ascII:输出的字符串
//				newlen:输出的字符串长度
//Return:
//*************************************************/
//void hex2string(char* hex, char* ascII, int len, int* newlen)
//{
//	int i = 0;
//	char newchar[100] = { 0 };
//	*newlen = len * 3;
//	for (i = 0; i < len; i++)
//	{
//		sprintf(newchar, "%02X ", hex[i]);
//		strcat(ascII, newchar);
//	}
//}
//
///*************************************************
//Function: 		string2hex
//Description: 	字符串转换成hex,要求str只能是大写字母ABCDEF和数字
//Input: 			str:要转换的字符串
//Output: 		hex:转换后的hex字符数组
//Return: 		0 成功
//                1 不符合规则，失败
//*************************************************/
//int string2hex(char* str, char* hex)
//{
//    int i = 0;
//    int j = 0;
//    unsigned char temp = 0;
//    int str_len = 0;
//    char str_cpy[100] = { '0' };
//    strcpy(str_cpy, str);
//    str_len = strlen(str_cpy);
//    if (str_len == 0)
//    {
//        return 1;
//    }
//    while (i < str_len)
//    {
//        if (str_cpy[i] >= '0' && str_cpy[i] <= 'F')
//        {
//            if ((str_cpy[i] >= '0' && str_cpy[i] <= '9'))
//            {
//                temp = (str_cpy[i] & 0x0f) << 4;
//            }
//            else if (str_cpy[i] >= 'A' && str_cpy[i] <= 'F')
//            {
//                temp = ((str_cpy[i] + 0x09) & 0x0f) << 4;
//            }
//            else
//            {
//                return 1;
//            }
//        }
//        else
//        {
//            return 1;
//        }
//        i++;
//        if (str_cpy[i] >= '0' && str_cpy[i] <= 'F')
//        {
//            if (str_cpy[i] >= '0' && str_cpy[i] <= '9')
//            {
//                temp |= (str_cpy[i] & 0x0f);
//            }
//            else if (str_cpy[i] >= 'A' && str_cpy[i] <= 'F')
//            {
//                temp |= ((str_cpy[i] + 0x09) & 0x0f);
//            }
//            else
//            {
//                return 1;
//            }
//        }
//        else
//        {
//            return 1;
//        }
//        i++;
//        hex[j] = temp;
//        //printf("%02x",temp);
//        j++;
//    }
//    //printf("\n");
//    return 0;
//}