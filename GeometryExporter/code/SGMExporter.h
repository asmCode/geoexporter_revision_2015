#pragma once

#include "..\..\CommonIncludes\IProgressSubject.h"
#include "..\..\CommonIncludes\IProgressObserver.h"
#include "..\..\CommonIncludes\IExportInterface.h"
#include <vector>

namespace FuturisEngine
{
	namespace Export
	{
		class Model;
		class Mesh;
	}
}

class IGameScene;
class IGameNode;
class IGameMesh;

class SGMExporter : public IExportInterface
{
public:
	SGMExporter();
	~SGMExporter();

	bool DoExport(const wchar_t *name, ExpInterface *ei, Interface *i);

	const char *GetResultMessage();

	void RegisterObserver(IProgressObserver *observer);
	void UnregisterObserver(IProgressObserver *observer);

private:
	std::vector<IProgressObserver*> observers;
	std::string m_filename;

	IGameScene *m_scene;

	void SetProgressSteps(int progressSteps);
	void StepProgress();

	bool InitializeScene();
	void FlattenNodes(IGameNode* node, std::vector<IGameNode*>& nodes);

	void ExtractMesh(IGameNode* gNode, FuturisEngine::Export::Model* model);
	void ExtractVertices(IGameNode* gNode, IGameMesh* gMesh, FuturisEngine::Export::Mesh* mesh);
};
