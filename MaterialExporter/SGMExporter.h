#pragma once

#include <IO/Path.h>
#include <XML/XmlWriter.h>
#include "TexturesSlotsHelper.h"

#include "..\CommonIncludes\IProgressSubject.h"
#include "..\CommonIncludes\IProgressObserver.h"
#include "..\CommonIncludes\IExportInterface.h"

class IGameMaterial;

class SGMExporter : public IExportInterface
{
private:
	std::vector<IProgressObserver*> observers;
	char resultMessage[1024];

	std::string GetPoint3Txt(Point3 &value);
	void WriteMaterialParams(IGameMaterial *material, XmlWriter &xmlWriter);

	bool ExportMaterial(IGameMaterial *material, std::string basePath);
	bool WriteTexturesMaps(IGameMaterial *material, XmlWriter &xmlWriter);

	void SetProgressSteps(int progressSteps);
	void StepProgress();

public:
	SGMExporter();
	~SGMExporter();

	bool DoExport(const TCHAR *name, ExpInterface *ei, Interface *i); 
	const char *GetResultMessage();

	void RegisterObserver(IProgressObserver *observer);
	void UnregisterObserver(IProgressObserver *observer);
};
