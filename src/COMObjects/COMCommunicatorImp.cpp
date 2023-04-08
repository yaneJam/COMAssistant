#include "stdafx.h"
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
	//����ǰ
	char* ASCII  = new char[len*2+1];//16����תASCII
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

	//����
	//for (int i = 0; i < nSize; ++i)
	//{
	//	if (Data[i - 1] == 0xAA && Data[i] == 0x55)//�յ�ָ��ͷ
	//	{
	//		COM_DataPos = 1;
	//		COM_cRecevieData[0] = 0xAA;
	//	}
	//	COM_cRecevieData[COM_DataPos++] = Data[i];
	//	if (COM_DataPos >= 4 && COM_cRecevieData[3] == COM_DataPos - 4)//�յ�����ָ��
	//	{
	//		int length = COM_cRecevieData[3];
	//		int cmd = COM_cRecevieData[2];
	//		

	//		//������
	//		char* ASCII = new char[COM_DataPos * 5 + 1];//16����תASCII
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



void COMCommunicatorImp::CloseReInitPortThread()
{
	if (m_hReInitPortThread != NULL)
	{
		WaitForSingleObject(m_hReInitPortThread, INFINITE);
		m_hReInitPortThread = NULL;
	}
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
	return 0;
}

BOOL COMCommunicatorImp::Init_MainCtrlConnect(std::string strPortName, std::string strBaudRate)
{
	BOOL bRes = FALSE;
	if (OpenPort(strPortName, strBaudRate))
	{
		bRes = TRUE;
	}
	else
	{
	}
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
	return FALSE;
}
