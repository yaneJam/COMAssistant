#pragma once

#include <xstring>
#include <tchar.h>

#define MAX_BUF_SIZE	1024


class  CSerialPort
{

public:
	CSerialPort(void);
	~CSerialPort(void);
	BOOL OpenPort(std::string strPortName, std::string strBaudRate);
	void ClosePort();
	bool DirectlyWrite(unsigned char *outPacket, int outLength);// write data directly 
	virtual void RecvPortData(unsigned char *Data, int len) = 0;

private:
	BOOL m_portStatus;
	HANDLE m_portHandle;
	bool m_bRun;
	HANDLE m_hThread;
	CRITICAL_SECTION m_cs;
	static DWORD WINAPI ThreadFun(LPVOID lpParam);
	void StartRecvThread();
	void CloseRecvThread();
	BOOL Read(unsigned char * inputData, const unsigned int & sizeBuffer, unsigned long & length);
	BOOL Write(const unsigned char *outputData, const unsigned int & sizeBuffer, unsigned long & length);
	int Run();

};