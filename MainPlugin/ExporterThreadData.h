#pragma once

#include "..\CommonIncludes\IProgressObserver.h"
#include <max.h>
#include <string>

struct ExporterThreadData
{
	std::string engineFileName;
	std::string outputFileName;
	ExpInterface *expInterface;
	Interface *maxInterface;
	IProgressObserver *progressObserver;
};
