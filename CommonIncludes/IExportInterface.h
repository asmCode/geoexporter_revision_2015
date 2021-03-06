#pragma once

#include "IProgressSubject.h"

class ExpInterface;
class Interface;

class IExportInterface : public IProgressSubject
{
public:
	virtual bool DoExport(const wchar_t *name, ExpInterface *ei, Interface *max_interface) = 0;
	virtual const char *GetResultMessage() = 0;
};
