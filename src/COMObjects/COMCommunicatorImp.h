#pragma once


#include "SerialPort.h"
#include "COMCommunicationTypedef.h"

typedef struct __SysParam
{
	std::string strPortName;
	std::string strBaudRate;
	BOOL bConnectFlag;
	BOOL bVersion;
	char cCANVersion[20];
	CONNECT_STATE ConnectState;		//下位机连接状态
	ACQUIREMODE	AcquireMode;
	EPID_STATE EpidState;
	EPID_MOVESTATE EPID_MoveState;
	BOOL EPID_Open_Ack;
	BOOL EPID_Close_Ack;
	BOOL Bed_XCtrl_Ack;
	BOOL Bed_XCtrl_Move2_Ack;
	BOOL Bed_YCtrl_Ack;
	BOOL Bed_YCtrl_Move2_Ack;
	BOOL Bed_ZCtrl_Ack;
	BOOL Bed_ZCtrl_Move2_Ack;
	TREATMENTBED_MOVESTATE Bed_MoveState;
	

}SysParam_Typedef;

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
	
public://变量

	SysParam_Typedef m_SysParam;		//系统参数


private:
	COMCommunicator* owner;
	unsigned char COM_cRecevieData[MAX_BUF_SIZE];//暂时放在这里
	int COM_DataPos;//暂时放在这里
	virtual void RecvPortData(unsigned char *Data, int len);
	BOOL SendData(unsigned char Command, unsigned char *DataBuf, unsigned char DataLen);
	BOOL SendCommand(unsigned char Command, unsigned char *DataBuf, unsigned char DataLen);
	void GetCommandData(unsigned char iCommand, unsigned char *CommandData, unsigned char len);
	void CheckCommand(int iCommand, unsigned char *CommandData, int len);
	void cmd_check(COMMUNICATOR_CMD cmd);


	HANDLE m_hReInitPortThread;
	BOOL IsPortExist(std::string strPortName);
	static DWORD WINAPI ReInitPortThreadFun(LPVOID lpParam);
	int ReInitPortRun();
	void StartReInitPortThread();
	void CloseReInitPortThread();

	HANDLE m_hConnectThread;
	BOOL m_bConnectRun;
	static DWORD WINAPI ConnectThreadFun(LPVOID lpParam);
	int ConnectTestRun();
	void StartConnectTestThread();
	void CloseConnectTestThread();
};