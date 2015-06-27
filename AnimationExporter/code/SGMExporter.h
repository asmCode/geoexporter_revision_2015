#pragma once

#include <windows.h>
#include <IGame\igame.h>
#include <IGame\IConversionManager.h>
#include <vector>
#include <set>
#include <fstream>
#include <IO\BinaryWriter.h>

#include "..\..\CommonIncludes\IProgressSubject.h"
#include "..\..\CommonIncludes\IProgressObserver.h"
#include "..\..\CommonIncludes\IExportInterface.h"

enum AnimationType
{
	Position = 1,
	Rotation = 2,
	Scale = 3,
	Float = 4
};

class SGMExporter : public IExportInterface
{
private:
	IGameScene *gScene;
	std::string m_filename;

	std::vector<IProgressObserver*> observers;
	
	void SetProgressSteps(int progressSteps);
	void StepProgress();

public:
	SGMExporter();
	~SGMExporter();

	bool DoExport(const wchar_t *name, ExpInterface *ei, Interface *i);
	bool InitializeScene();
	const char *GetResultMessage();
	void FlattenNodes(IGameNode* node, std::vector<IGameNode*>& nodes);

	void RegisterObserver(IProgressObserver *observer);
	void UnregisterObserver(IProgressObserver *observer);
};
