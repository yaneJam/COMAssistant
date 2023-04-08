#pragma once
#include <string>


class COMCommunicatorImp;


class  COMOBJECTS_API COMCommunicator :public yModule
{
	friend class COMCommunicatorImp;
public:
	COMCommunicator();
	~COMCommunicator();


	RESULT doSlot(_tsptr<yCMD> cmd)override;

	RESULT connect(_tsptr<yCMD>  cmd);
	RESULT disconnect(_tsptr<yCMD>  cmd);
	RESULT send(_tsptr<yCMD>  cmd);

	RESULT getContext(_tsptr<yCMD>  cmd);

	//初始化连接，COM初始化配置，连接状态在Communication_Cmd_Check通知，命令为：CMD_RESPONSE_COMMUNICATION_STATE
	BOOL Init_Connect(std::string strPortName, std::string strBaudRate);
	//关闭连接
	void Close_Connect();
	int GetComList_Reg();
private:
	COMCommunicatorImp* imp;
	_tsptr<yCMD> connectting_callback;
};