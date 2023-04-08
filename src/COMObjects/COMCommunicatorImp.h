#pragma once
#include "SerialPort.h"



class COMCommunicator;
class  COMCommunicatorImp : public CSerialPort
{

public:
	COMCommunicatorImp(COMCommunicator* communicator);
	~COMCommunicatorImp();

	BOOL Init_MainCtrlConnect(std::string strPortName, std::string strBaudRate);
	void Close_MainCtrlConnect();

public:

	BOOL IsConnect();
	
private:
	COMCommunicator* owner;
	unsigned char COM_cRecevieData[MAX_BUF_SIZE];//暂时放在这里
	int COM_DataPos;//暂时放在这里
	virtual void RecvPortData(unsigned char *Data, int len);
	BOOL SendData(unsigned char Command, unsigned char *DataBuf, unsigned char DataLen);
	BOOL SendCommand(unsigned char Command, unsigned char *DataBuf, unsigned char DataLen);
	void GetCommandData(unsigned char iCommand, unsigned char *CommandData, unsigned char len);
	void CheckCommand(int iCommand, unsigned char *CommandData, int len);


	HANDLE m_hReInitPortThread;
	BOOL IsPortExist(std::string strPortName);
	void CloseReInitPortThread();

	HANDLE m_hConnectThread;
	BOOL m_bConnectRun;
	static DWORD WINAPI ConnectThreadFun(LPVOID lpParam);
	int ConnectTestRun();
	void StartConnectTestThread();
	void CloseConnectTestThread();
};