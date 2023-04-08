#include "stdafx.h"


extern "C"
{
	COMOBJECTS_API yModuleHandle createModule(_tstring mapperType);

}

yModuleHandle createModule(_tstring mapperType)
{
	yModuleHandle handle;
	_tsptr<yModule> object;

	if (mapperType == "COMCommunicator")
	{
		object = _tsptr<COMCommunicator>(new COMCommunicator());
	}

	handle.obj = object;
	return handle;
}
