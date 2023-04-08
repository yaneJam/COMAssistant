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

	//��ʼ�����ӣ�COM��ʼ�����ã�����״̬��Communication_Cmd_Check֪ͨ������Ϊ��CMD_RESPONSE_COMMUNICATION_STATE
	BOOL Init_Connect(std::string strPortName, std::string strBaudRate);
	//�ر�����
	void Close_Connect();
	int GetComList_Reg();
private:
	COMCommunicatorImp* imp;
	_tsptr<yCMD> connectting_callback;
};