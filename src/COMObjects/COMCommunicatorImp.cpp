#include "stdafx.h"
//#include "Command.h"
#include "COMCommunicatorImp.h"
#include "COMCommunication.h"
#include <vector>



COMCommunicatorImp::COMCommunicatorImp(COMCommunicator* communicator)
{
	memset(COM_cRecevieData, '\0', MAX_BUF_SIZE);
	COM_DataPos = 0;
	owner = communicator;
	m_hReInitPortThread = NULL;
	m_hConnectThread = NULL;
	m_bConnectRun = FALSE;
	
}

COMCommunicatorImp::~COMCommunicatorImp()
{
	Close_MainCtrlConnect();
}

BOOL COMCommunicatorImp::SendData(unsigned char Command, unsigned char *DataBuf, unsigned char DataLen)
{
	int BufLen = 0;
	int SendLen = 0;
	unsigned char send_data[128] = { 0 };

	send_data[BufLen++] = 0xAA;
	send_data[BufLen++] = 0x55;
	send_data[BufLen++] = Command;
	send_data[BufLen++] = DataLen;
	for (int i = 0; i < DataLen; i++)
	{
		send_data[BufLen++] = DataBuf[i];
	}
	SendLen = BufLen;
	if (DirectlyWrite(send_data, SendLen))
	{
		return true;
	}
	return false;
}

BOOL COMCommunicatorImp::SendCommand(unsigned char Command, unsigned char *DataBuf = NULL, unsigned char DataLen = 0)
{
	return SendData(Command, DataBuf, DataLen);
}


void COMCommunicatorImp::RecvPortData(unsigned char *Data, int len)
{
	//砍包前
	char* ASCII  = new char[len*2+1];//16进制转ASCII
	memset(ASCII, 0, sizeof(char));
	for (int t = 0; t < len; t++)
	{
		sprintf(ASCII,"%s%02x", ASCII,Data[t]);
	}
	DEBUG_LOG("%s", ASCII);

	SYSTEMTIME	time;
	::GetLocalTime(&time);
	char datt[ 256];
	sprintf_s(datt, 256* sizeof(char), "%02d:%02d:%02d.%03d ",
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	
	_tcmd(cmd);
	(*cmd->request)["data"] = ASCII;
	(*cmd->request)["datetime"] = datt;
	owner->emitSignal("recieved",cmd);
	delete[] ASCII;


	//	static unsigned char DataBuf[256], DataLen = 0;
	//int Temp = 0;
	//int iCommand = 0;
	//if (len > MAX_BUF_SIZE)
	//{
	//	ERROR_LOG("buf oversize");
	//	}
	//int nSize = min(len, MAX_BUF_SIZE);

	//砍包
	//for (int i = 0; i < nSize; ++i)
	//{
	//	if (Data[i - 1] == 0xAA && Data[i] == 0x55)//收到指令头
	//	{
	//		COM_DataPos = 1;
	//		COM_cRecevieData[0] = 0xAA;
	//	}
	//	COM_cRecevieData[COM_DataPos++] = Data[i];
	//	if (COM_DataPos >= 4 && COM_cRecevieData[3] == COM_DataPos - 4)//收到完整指令
	//	{
	//		int length = COM_cRecevieData[3];
	//		int cmd = COM_cRecevieData[2];
	//		

	//		//砍包后
	//		char* ASCII = new char[COM_DataPos * 5 + 1];//16进制转ASCII
	//		memset(ASCII, 0, sizeof(char));
	//		for (int t = 0; t < COM_DataPos; t++)
	//		{
	//			sprintf(ASCII, "%s 0x%02x", ASCII, COM_cRecevieData[t]);
	//		}
	//		DEBUG_LOG("cut %s", ASCII);
	//		_tstring package = ASCII;
	//		_tcmd(recieved);
	//		(*recieved->request)["package"] = package;
	//		owner->emitSignal("recieved",recieved);


	//		DataLen = 0;
	//		for (int j = 4; j < COM_DataPos; j++)
	//		{
	//			DataBuf[DataLen++] = COM_cRecevieData[j];
	//		}
	//		DataBuf[DataLen] = '\0';
	//		GetCommandData(cmd, DataBuf, DataLen);

	//		memset(COM_cRecevieData, 0, MAX_BUF_SIZE);
	//		COM_DataPos = 0;
	//	}
	//}
}

void COMCommunicatorImp::GetCommandData(unsigned char Command, unsigned char *CommandData, unsigned char len)
{

}

DWORD WINAPI COMCommunicatorImp::ReInitPortThreadFun(LPVOID lpParam)
{
	COMCommunicatorImp* pHandler = (COMCommunicatorImp*)lpParam;
	pHandler->ReInitPortRun();
	return 0;
}

void COMCommunicatorImp::StartReInitPortThread()
{
	CloseReInitPortThread();
	m_hReInitPortThread = ::CreateThread(NULL, 0, ReInitPortThreadFun, this, 0, NULL);
}

void COMCommunicatorImp::CloseReInitPortThread()
{
	if (m_hReInitPortThread != NULL)
	{
		WaitForSingleObject(m_hReInitPortThread, INFINITE);
		m_hReInitPortThread = NULL;
	}
}

int COMCommunicatorImp::ReInitPortRun()
{
	Init_MainCtrlConnect(m_SysParam.strPortName, m_SysParam.strBaudRate);
	m_hReInitPortThread = NULL;
	m_bConnectRun = FALSE;
	return 0;
}


DWORD WINAPI COMCommunicatorImp::ConnectThreadFun(LPVOID lpParam)
{
	COMCommunicatorImp* pHandler = (COMCommunicatorImp*)lpParam;
	pHandler->ConnectTestRun();
	return 0;
}

void COMCommunicatorImp::StartConnectTestThread()
{
	CloseConnectTestThread();
	m_bConnectRun = TRUE;
	m_hConnectThread = ::CreateThread(NULL, 0, ConnectThreadFun, this, 0, NULL);
}

void COMCommunicatorImp::CloseConnectTestThread()
{
	if (m_hConnectThread != NULL)
	{
		m_bConnectRun = FALSE;
		WaitForSingleObject(m_hConnectThread, INFINITE);
		m_hConnectThread = NULL;
	}
}

int COMCommunicatorImp::ConnectTestRun()
{
	/*static int count = 0;
	while (m_bConnectRun)
	{
		if (m_SysParam.ConnectState == CONNECT_INIT_FAILED)
		{
			StartReInitPortThread();
			break;
		}
		if (IsConnect())
		{
			count = 0;
			if (m_SysParam.ConnectState == CONNECT_FAILED)
			{
				m_SysParam.ConnectState = CONNECT_SUCCESS;
				owner->Communication_Cmd_Check(CMD_RESPONSE_COMMUNICATION_STATE);
			}
		}
		else
		{
			if (m_SysParam.ConnectState != CONNECT_FAILED)
			{
				if (++count >= 5)
				{
					count = 0;
					m_SysParam.ConnectState = CONNECT_FAILED;
					owner->Communication_Cmd_Check(CMD_RESPONSE_COMMUNICATION_STATE);
				}
			}
		}
		Sleep(1000);
	}
	m_bConnectRun = FALSE;
	m_hConnectThread = NULL;*/
	return 0;
}

BOOL COMCommunicatorImp::Init_MainCtrlConnect(std::string strPortName, std::string strBaudRate)
{
	BOOL bRes = FALSE;
	m_SysParam.strPortName = strPortName;
	m_SysParam.strBaudRate = strBaudRate;
	if (OpenPort(strPortName, strBaudRate))
	{
		bRes = TRUE;
		/*if (IsConnect())
		{
			m_SysParam.ConnectState = CONNECT_SUCCESS;
			owner->Communication_Cmd_Check(CMD_RESPONSE_COMMUNICATION_STATE);
			bRes = TRUE;
		}
		else if (m_SysParam.ConnectState != CONNECT_FAILED)
		{
			m_SysParam.ConnectState = CONNECT_FAILED;
			owner->Communication_Cmd_Check(CMD_RESPONSE_COMMUNICATION_STATE);
		}*/
	}
	else
	{
		/*m_SysParam.ConnectState = CONNECT_INIT_FAILED;
		owner->Communication_Cmd_Check(CMD_RESPONSE_COMMUNICATION_STATE);*/
	}
	//StartConnectTestThread();
	return bRes;
}
//
void COMCommunicatorImp::Close_MainCtrlConnect()
{
	CloseReInitPortThread();
	CloseConnectTestThread();
	ClosePort();
}

BOOL COMCommunicatorImp::IsConnect()
{
	/*m_SysParam.bConnectFlag = FALSE;
	if (SendCommand(CMD_SYS_COMTEST))
	{
		Sleep(1000);
		if (m_SysParam.bConnectFlag == TRUE)
		{
			return TRUE;
		}
	}*/
	return FALSE;
}
