#include "stdafx.h"
#include "SerialPort.h"

CSerialPort::CSerialPort() :
m_portStatus(FALSE),
m_portHandle(NULL),
m_bRun(FALSE),
m_hThread(NULL)
{
	InitializeCriticalSection(&m_cs);
}

CSerialPort::~CSerialPort()
{
	ClosePort();
	DeleteCriticalSection(&m_cs);
}

BOOL CSerialPort::OpenPort(std::string strPortName, std::string strBaudRate)
{

	if (m_portStatus == FALSE)  // if port is opened already, not open port again.
	{
		m_portHandle = CreateFile(strPortName.c_str(),  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.

		if (INVALID_HANDLE_VALUE == m_portHandle)
		{
			/*CString strError = GetLastErrorMsg();
			TRACE("%s\n", strError);*/
			//DuiMessageBox(strError);
			return FALSE;
		}
		DCB m_portConfig;
		// Get current configuration of serial communication port.
		if (GetCommState(m_portHandle, &m_portConfig) == 0)
		{
			//DuiMessageBox(GetLastErrorMsg());
			return FALSE;
		}
		m_portConfig.ByteSize = 8;
		m_portConfig.StopBits = ONESTOPBIT;
		m_portConfig.Parity = NOPARITY;
		switch (atoi(strBaudRate.c_str()))
		{
		case 110:
			m_portConfig.BaudRate = CBR_110;
			break;
		case 300:
			m_portConfig.BaudRate = CBR_300;
			break;
		case 600:
			m_portConfig.BaudRate = CBR_600;
			break;
		case 1200:
			m_portConfig.BaudRate = CBR_1200;
			break;
		case 2400:
			m_portConfig.BaudRate = CBR_2400;
			break;
		case 4800:
			m_portConfig.BaudRate = CBR_4800;
			break;
		case 9600:
			m_portConfig.BaudRate = CBR_9600;
			break;
		case 14400:
			m_portConfig.BaudRate = CBR_14400;
			break;
		case 19200:
			m_portConfig.BaudRate = CBR_19200;
			break;
		case 38400:
			m_portConfig.BaudRate = CBR_38400;
			break;
		case 56000:
			m_portConfig.BaudRate = CBR_56000;
			break;
		case 57600:
			m_portConfig.BaudRate = CBR_57600;
			break;
		case 115200:
			m_portConfig.BaudRate = CBR_115200;
			break;
		case 128000:
			m_portConfig.BaudRate = CBR_128000;
			break;
		case 256000:
			m_portConfig.BaudRate = CBR_256000;
			break;
		default:
			m_portConfig.BaudRate = CBR_9600;
			break;
		}

		// Set current configuration of serial communication port.
		if (SetCommState(m_portHandle, &m_portConfig) == 0)
		{
			//DuiMessageBox(GetLastErrorMsg());
			return FALSE;
		}

		// instance an object of COMMTIMEOUTS.
		COMMTIMEOUTS comTimeOut;
		comTimeOut.ReadIntervalTimeout = MAXDWORD;
		comTimeOut.ReadTotalTimeoutMultiplier = MAXDWORD;
		comTimeOut.ReadTotalTimeoutConstant = 1;
		comTimeOut.WriteTotalTimeoutMultiplier = 1;
		comTimeOut.WriteTotalTimeoutConstant = 1;
		SetCommTimeouts(m_portHandle, &comTimeOut);		// set the time-out parameter into device control.

		PurgeComm(m_portHandle, PURGE_TXCLEAR | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_RXABORT);


		StartRecvThread();
		m_portStatus = TRUE;
		return TRUE;
	}
	else//已经打开，重新打开新的串口号
	{
		ClosePort();
		OpenPort(strPortName, strBaudRate);
	}
	return TRUE;
}

void CSerialPort::ClosePort()
{
	CloseRecvThread();
	if (m_portHandle != NULL)
	{
		CloseHandle(m_portHandle);
		m_portHandle = NULL;
	}
	m_portStatus = FALSE;
}

bool CSerialPort::DirectlyWrite(unsigned char *outPacket, int outLength)// write data directly 
{
	bool bresult = true;
	unsigned char m_sendBuffer[MAX_BUF_SIZE];
	unsigned int m_sendSize;
	if (outLength <= MAX_BUF_SIZE)
	{
		memcpy(m_sendBuffer, outPacket, outLength);
		m_sendBuffer[outLength] = 0;
		m_sendSize = outLength;
		unsigned long nWritten = 0;

		::EnterCriticalSection(&m_cs);
		if (Write(m_sendBuffer, m_sendSize, nWritten) == FALSE)
		{
			bresult = false;
		}
		::LeaveCriticalSection(&m_cs);
	}
	return bresult;
}

DWORD WINAPI CSerialPort::ThreadFun(LPVOID lpParam)
{
	CSerialPort* pHandler = (CSerialPort*)lpParam;
	pHandler->Run();
	return 0;
}

void CSerialPort::StartRecvThread()
{
	CloseRecvThread();
	m_bRun = TRUE;
	m_hThread = ::CreateThread(NULL, 0, ThreadFun, this, 0, NULL);
}

void CSerialPort::CloseRecvThread()
{
	if (m_hThread != NULL)
	{
		m_bRun = false;
		WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = NULL;
	}
}

int CSerialPort::Run()
{
	while (m_bRun && m_portStatus)
	{
		static unsigned char buf[MAX_BUF_SIZE];
		memset(buf, '\0', MAX_BUF_SIZE * sizeof(char));
		unsigned int BuffSize = MAX_BUF_SIZE;
		unsigned long len = 0;
		::EnterCriticalSection(&m_cs);
		BOOL res = Read(buf, BuffSize, len);
		::LeaveCriticalSection(&m_cs);
		if (res == TRUE && len > 0)
		{
			RecvPortData(buf, len);
		}
		Sleep(50);
	}
	m_bRun = FALSE;
	m_hThread = NULL;
	return 0;
}

BOOL CSerialPort::Read(unsigned char * inputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (ReadFile(m_portHandle,  // handle of file to read
		inputData,               // handle of file to read
		sizeBuffer,              // number of bytes to read
		&length,                 // pointer to number of bytes read
		NULL) == 0)              // pointer to structure for data
	{
		//MessageBox(0, GetLastErrorMsg(), _T("系统"));
		return FALSE;
	}
	//COM_LOG("READ:%s:message:%s, buffersize:%d, readsize:%d\n", __FUNCTION__, inputData, sizeBuffer, length);
	if (length > 0)
	{
		inputData[length] = NULL; // Assign end flag of message.
		return TRUE;
	}

	return FALSE;
}

BOOL CSerialPort::Write(const unsigned char * outputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (sizeBuffer > 0)
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			outputData,              // pointer to data to write to file
			sizeBuffer,              // number of bytes to write
			&length, NULL) == 0)      // pointer to number of bytes written
		{
			//DuiMessageBox(GetLastErrorMsg());
			return FALSE;
		}
		//COM_LOG("WRITE: %s:message:%s, buffersize:%d, writesize:%d\n", __FUNCTION__, outputData, sizeBuffer, length);
		return TRUE;
	}
	return FALSE;
}




