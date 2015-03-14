#pragma once

#include <windows.h>
#include <vector>
#include <map>

#include <IO\BinaryWriter.h>

#include <IGame\igame.h>
#include <IGame\IConversionManager.h>

#include "..\..\CommonIncludes\IProgressSubject.h"
#include "..\..\CommonIncludes\IProgressObserver.h"
#include "..\..\CommonIncludes\IExportInterface.h"

class GameObject;

class Ribbon;
class Source;
class Destination;
class StaticSource;
class StaticDestination;
class Path;
class Guy;
class TransformKey;
class Material;
class CustomSceneElement;
template <typename T> class Key;
class XmlWriter;

class SGMExporter : public IExportInterface
{
private:
	typedef std::map<std::string, Ribbon*> RibbonsMap;
	typedef std::map<std::string, Material*> MaterialsMap;
	typedef std::vector<CustomSceneElement*> CustomSceneElementsVector;

	std::vector<GameObject*> m_gameObjects;

	std::vector<IProgressObserver*> observers;
	std::string fileName;
	std::string sceneName;

	IGameScene *scene;

	void FilterSceneNodes(
		IGameNode *node,
		std::vector<IGameNode*>& sceneNodes,
		std::vector<IGameNode*>& staticNodes);

	void SetProgressSteps(int progressSteps);
	void StepProgress();

	RibbonsMap m_ribbons;
	CustomSceneElementsVector m_customSceneElements;
	std::vector<Guy*> m_guys;
	MaterialsMap m_materials;

	void AddToRibbon(const std::string& name, Source* source);
	void AddToRibbon(const std::string& name, Destination* destination);
	void AddToRibbon(const std::string& name, Path* path);
	void AddToRibbon(const std::string& name, StaticSource* source);
	void AddToRibbon(const std::string& name, StaticDestination* destination);
	void AddToGuys(Guy* guy);
	void AddToCustomSceneElements(CustomSceneElement* element);

	void ProcessSceneElement(IGameNode* node);

	Source* ProcessSource(IGameNode* node, const std::string& id);
	std::string GetMaterial(IGameNode* node);
	Destination* ProcessDestination(IGameNode* node, const std::string& id);
	StaticSource* ProcessStaticSource(IGameNode* node, const std::string& id);
	StaticDestination* ProcessStaticDestination(IGameNode* node, const std::string& id);
	Path* ProcessPath(IGameNode* node);
	void ProcessIntProperty(IGameNode* node, const std::string& name, std::vector<Key<int>*>& keys);
	void ProcessFloatProperty(IGameNode* node, const std::string& name, std::vector<Key<float>*>& keys);

	Guy* ProcessGuy(IGameNode* node, const std::string& guyId);
	CustomSceneElement* ProcessCustomSceneElement(IGameNode* node, const std::string& type, const std::string& id);

	Ribbon* GetRibbonByName(const std::string& name);
	Ribbon* GetOrCreateRibbon(const std::string& name);
	bool IsMaterialCollected(const std::string& name);
	void CollectMaterial(Material* material);

	void ExtractKeys(IGameControl *gControl, std::vector<TransformKey*>& keys);

	void WriteMaterials(XmlWriter& xml);
	void WritePath(XmlWriter& xml, Path* path);
	void WriteIntKeys(XmlWriter& xml, std::vector<Key<int>*>& keys);
	void WriteFloatKeys(XmlWriter& xml, std::vector<Key<float>*>& keys);
	void WriteStaticNodes(XmlWriter& xml, const std::vector<IGameNode*>& staticNodes);
	void WriteMaterial(XmlWriter& xml, Material* material);
	void WriteGuys(XmlWriter& xml);
	void WriteCustomSceneElements(XmlWriter& xml);
	void WriteMaterialAttribIfExist(XmlWriter& xml, const std::string& materialName);

	IGameProperty* GetProperty(IGameNode* node, const std::string& name);
	bool GetPropertyFloat(IGameNode* node, const std::string& name, float& value);
	bool GetPropertyBool(IGameNode* node, const std::string& name, bool& value);
	bool GetPropertyInt(IGameNode* node, const std::string& name, int& value);
	bool GetPropertyString(IGameNode* node, const std::string& name, std::string& value);

	void FlattenNodes(IGameNode* node, std::vector<IGameNode*>& nodes);

	bool WriteSceneToFile();

public:
	SGMExporter();
	~SGMExporter();

	bool DoExport(const TCHAR *name, ExpInterface *ei, Interface *i); 
	const char *GetResultMessage();

	void RegisterObserver(IProgressObserver *observer);
	void UnregisterObserver(IProgressObserver *observer);
};
