#include "sgmexporter.h"
#include "AnimationExtractor.h"
#include <Utils/StringUtils.h>
#include <Utils/Log.h>
#include <IO/Path.h>
#include <decomp.h>

SGMExporter::SGMExporter()
{
}

SGMExporter::~SGMExporter()
{
}

bool SGMExporter::DoExport(const wchar_t *name, ExpInterface *ei, Interface *max_interface)
{
	m_filename = StringUtils::ToNarrow(name);
	Path path(m_filename);

	Log::StartLog(true, false, false);
	Log::LogT("=== exporting animation to file '%s'", m_filename.c_str());

	if (!InitializeScene())
		return false;

	std::vector<IGameNode*> gNodes;
	for (int i = 0; i < gScene->GetTopLevelNodeCount(); i++)
		FlattenNodes(gScene->GetTopLevelNode(i), gNodes);

	AnimationExtractor* animationExtractor = new AnimationExtractor(path.GetFilename(), gScene);

	for (uint32_t i = 0; i < gNodes.size(); i++)
		animationExtractor->AddNode(gNodes[i]);

	gScene->ReleaseIGame();

	animationExtractor->SaveToFile(m_filename);
	delete animationExtractor;

	return true;
}

bool SGMExporter::InitializeScene()
{
	gScene = GetIGameInterface();
	assert(gScene != NULL);

	IGameConversionManager *cm = GetConversionManager();
	assert(cm != NULL);
	cm->SetCoordSystem(IGameConversionManager::IGAME_OGL);

	if (!gScene->InitialiseIGame(false))
	{
		Log::LogT("error: couldnt initialize scene");
		return false;
	}

	gScene->SetStaticFrame(0);

	return true;
}

void SGMExporter::FlattenNodes(IGameNode* node, std::vector<IGameNode*>& nodes)
{
	if (node == NULL)
		return;

	nodes.push_back(node);

	for (int i = 0; i < node->GetChildCount(); i++)
		FlattenNodes(node->GetNodeChild(i), nodes);
}

void SGMExporter::RegisterObserver(IProgressObserver *observer)
{
	observers.push_back(observer);
}

void SGMExporter::UnregisterObserver(IProgressObserver *observer)
{
	//TODO:
}

void SGMExporter::SetProgressSteps(int progressSteps)
{
	std::vector<IProgressObserver*>::iterator i;
	for (i = observers.begin(); i != observers.end(); i++)
		(*i) ->SetProgressSteps(this, progressSteps);
}

void SGMExporter::StepProgress()
{
	std::vector<IProgressObserver*>::iterator i;
	for (i = observers.begin(); i != observers.end(); i++)
		(*i) ->StepProgress(this);
}

const char *SGMExporter::GetResultMessage()
{
	return "";
}
