#include "sgmexporter.h"

#include "SceneElements/Ribbon.h"
#include "SceneElements/Source.h"
#include "SceneElements/Destination.h"
#include "SceneElements/Path.h"
#include "SceneElements/TransformKey.h"
#include "SceneElements/Key.h"
#include "SceneElements/Guy.h"
#include "SceneElements/CustomSceneElement.h"
#include "SceneElements/Material.h"
#include "SceneElements/StaticSource.h"
#include "SceneElements/StaticDestination.h"
#include "SceneElements/GameObject.h"

#include "../MaterialProcessor.h"
#include "Serializers/CustomParameterSerializer.h"

#include <XML/XmlWriter.h>
#include <Utils/StringUtils.h>
#include <Utils/Log.h>

#include <modstack.h>
#include <icustattribcontainer.h>
#include <custattrib.h>
#include <iparamb2.h>
//#include <IGame/IGameProperty.h>

std::string Vec3ToString(const sm::Vec3& value)
{
	char buff[1024];
	sprintf(buff, "%f;%f;%f", value.x, value.y, value.z);
	return buff;
}

SGMExporter::SGMExporter()
{
}

SGMExporter::~SGMExporter()
{
}

IGameControlType GetGameControlType(int propType)
{
	switch (propType)
	{
	case IGAME_FLOAT_PROP:
	case IGAME_INT_PROP:;
		return IGameControlType::IGAME_FLOAT;

	case IGAME_POINT3_PROP:
		return IGameControlType::IGAME_POINT3;
	}

	assert(false);
	return IGameControlType::IGAME_FLOAT;
}

bool SGMExporter::DoExport(const TCHAR *name, ExpInterface *ei, Interface *max_interface)
{
	scene = GetIGameInterface();
	assert(scene != NULL);

	sceneName = StringUtils::ToNarrow(scene->GetSceneFileName());
	sceneName.replace(sceneName.find(".max"), 4, "");

	fileName = StringUtils::ToNarrow(name) + sceneName + ".scene";

	Log::StartLog(true, false, false);
	Log::LogT("=== exporting scene to file '%s'", fileName.c_str());

	scene->SetStaticFrame(0);

	IGameConversionManager *cm = GetConversionManager();
	assert(cm != NULL);

	cm->SetCoordSystem(IGameConversionManager::IGAME_OGL);

	if (!scene->InitialiseIGame(false))
	{
		Log::LogT("error: couldnt initialize scene");
		return false;
	}

	/*
	std::vector<IGameNode*> sceneNodes;
	std::vector<IGameNode*> staticNodes;
	for (int i = 0; i < scene->GetTopLevelNodeCount(); i++)
		FilterSceneNodes(scene->GetTopLevelNode(i), sceneNodes, staticNodes);

	Log::LogT("found %d scene nodes", sceneNodes.size());

	for (int i = 0; i < sceneNodes.size(); i++)
		ProcessSceneElement(sceneNodes[i]);
	*/

	std::vector<IGameNode*> nodes;
	for (int i = 0; i < scene->GetTopLevelNodeCount(); i++)
		FlattenNodes(scene->GetTopLevelNode(i), nodes);

	for (uint32_t i = 0; i < nodes.size(); i++)
	{
		GameObject* gameObject = new GameObject();
		gameObject->SetFromNode(scene, nodes[i]);
		m_gameObjects.push_back(gameObject);
	}

	return WriteSceneToFile();
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

void SGMExporter::FilterSceneNodes(
	IGameNode *node,
	std::vector<IGameNode*>& sceneNodes,
	std::vector<IGameNode*>& staticNodes)
{
	static std::string SceneNodeName = "scene";

	std::string nodeName = StringUtils::ToNarrow(node->GetName());

	if (nodeName.find(SceneNodeName) == 0)
		sceneNodes.push_back(node);
	else
		staticNodes.push_back(node);

	for (int i = 0; i < node ->GetChildCount(); i++)
		FilterSceneNodes(node->GetNodeChild(i), sceneNodes, staticNodes);
}

void SGMExporter::ProcessSceneElement(IGameNode* node)
{
	std::string name = StringUtils::ToNarrow(node->GetName());

	std::vector<std::string> nameParts;
	StringUtils::Split(name, ".", nameParts);

	if (nameParts.size() < 2 ||
		nameParts[0] != "scene")
		return;

	if (nameParts[1] == "morph")
	{
		std::string morphType = nameParts[2];
		std::string morphId = nameParts[3];

		if (morphType == "src")
		{
			Source* source = ProcessSource(node, morphId);
			AddToRibbon(morphId, source);
		}
		else if (morphType == "dst")
		{
			Destination* destination = ProcessDestination(node, morphId);
			AddToRibbon(morphId, destination);
		}
		else if (morphType == "path")
		{
			Path* path = ProcessPath(node);
			AddToRibbon(morphId, path);
		}
		else if (morphType == "dst-static")
		{
			StaticDestination* destination = ProcessStaticDestination(node, morphId);
			AddToRibbon(morphId, destination);
		}
		else if (morphType == "src-static")
		{
			StaticSource* source = ProcessStaticSource(node, morphId);
			AddToRibbon(morphId, source);
		}
	}
	else if (nameParts[1] == "guy")
	{
		std::string guyId = nameParts[2];

		Guy* guy = ProcessGuy(node, guyId);
		AddToGuys(guy);
	}
	else // custom element with custom attributes
	{
		assert(nameParts.size() == 3);

		std::string type = nameParts[1];
		std::string id = nameParts[2];

		CustomSceneElement* element = ProcessCustomSceneElement(node, type, id);
		AddToCustomSceneElements(element);
	}
}

void SGMExporter::AddToRibbon(const std::string& name, Source* source)
{
	Ribbon* ribbon = GetOrCreateRibbon(name);
	if (ribbon->Source != NULL)
	{
		Log::LogT("Ribbon %s already have source", name.c_str());
		return;
	}

	ribbon->Source = source;
}

void SGMExporter::AddToRibbon(const std::string& name, Destination* destination)
{
	Ribbon* ribbon = GetOrCreateRibbon(name);
	if (ribbon->Destination != NULL)
	{
		Log::LogT("Ribbon %s already have destination", name.c_str());
		return;
	}

	ribbon->Destination = destination;
}

void SGMExporter::AddToRibbon(const std::string& name, Path* path)
{
	Ribbon* ribbon = GetOrCreateRibbon(name);
	if (ribbon->Path != NULL)
	{
		Log::LogT("Ribbon %s already have path", name.c_str());
		return;
	}

	ribbon->Path = path;
}

void SGMExporter::AddToRibbon(const std::string& name, StaticSource* source)
{
	Ribbon* ribbon = GetOrCreateRibbon(name);
	if (ribbon->StaticSource != NULL)
	{
		Log::LogT("Ribbon %s already have source", name.c_str());
		return;
	}

	ribbon->StaticSource = source;
}

void SGMExporter::AddToRibbon(const std::string& name, StaticDestination* destination)
{
	Ribbon* ribbon = GetOrCreateRibbon(name);
	if (ribbon->StaticDestination != NULL)
	{
		Log::LogT("Ribbon %s already have destination", name.c_str());
		return;
	}

	ribbon->StaticDestination = destination;
}

void SGMExporter::AddToGuys(Guy* guy)
{
	m_guys.push_back(guy);
}

void SGMExporter::AddToCustomSceneElements(CustomSceneElement* element)
{
	m_customSceneElements.push_back(element);
}

Source* SGMExporter::ProcessSource(IGameNode* node, const std::string& id)
{
	Source* source = new Source();
	source->MeshName = StringUtils::ToNarrow(node->GetName());
	source->MaterialName = GetMaterial(node);
	source->Destroy = false;
	source->Stay = false;

	bool destroy = false;
	GetPropertyBool(node, "destroy", destroy);
	source->Destroy = destroy;

	bool stay = false;
	GetPropertyBool(node, "stay", stay);
	source->Stay = stay;

	return source;
}

bool SGMExporter::IsMaterialCollected(const std::string& name)
{
	return m_materials.find(name) != m_materials.end();
}

void SGMExporter::CollectMaterial(Material* material)
{
	m_materials[material->Name] = material;
}

std::string SGMExporter::GetMaterial(IGameNode* node)
{
	IGameMaterial* igameMaterial = node->GetNodeMaterial();
	if (igameMaterial == NULL)
		return "";

	std::string name = StringUtils::ToNarrow(igameMaterial->GetMaterialName());

	if (IsMaterialCollected(name))
		return name;

	MaterialProcessor materialProcessor;
	Material* material = materialProcessor.Create(igameMaterial);

	/*
	IPropertyContainer* propertyContainer = igameMaterial->GetIPropertyContainer();
	if (propertyContainer != NULL)
	{
		IGameProperty* prop = NULL;

		prop = propertyContainer->QueryProperty(L"use_solid");
		if (prop != NULL && prop->GetPropertyValue(iValue))
			material->UseSolid = (iValue != 0);

		prop = propertyContainer->QueryProperty(L"use_wire");
		if (prop != NULL && prop->GetPropertyValue(iValue))
			material->UseWire = (iValue != 0);
		
		prop = propertyContainer->QueryProperty(L"solid_glow_power");
		if (prop != NULL && prop->GetPropertyValue(fValue))
			material->SolidGlowPower = fValue;

		prop = propertyContainer->QueryProperty(L"solid_glow_multiplier");
		if (prop != NULL && prop->GetPropertyValue(fValue))
			material->SolidGlowMultiplier = fValue;

		prop = propertyContainer->QueryProperty(L"wire_glow_power");
		if (prop != NULL && prop->GetPropertyValue(fValue))
			material->WireGlowPower = fValue;

		prop = propertyContainer->QueryProperty(L"wire_glow_multiplier");
		if (prop != NULL && prop->GetPropertyValue(fValue))
			material->WireGlowMultiplier = fValue;
	}
	*/

	CollectMaterial(material);

	return name;
}

Destination* SGMExporter::ProcessDestination(IGameNode* node, const std::string& id)
{
	Destination* destination = new Destination();
	destination->MeshName = StringUtils::ToNarrow(node->GetName());
	destination->MaterialName = GetMaterial(node);
	destination->Stay = false;

	bool stay = false;;
	GetPropertyBool(node, "stay", stay);

	destination->Stay = stay;

	return destination;
}

StaticSource* SGMExporter::ProcessStaticSource(IGameNode* node, const std::string& id)
{
	StaticSource* source = new StaticSource();
	source->MeshName = StringUtils::ToNarrow(node->GetName());
	source->MaterialName = GetMaterial(node);
	return source;
}

StaticDestination* SGMExporter::ProcessStaticDestination(IGameNode* node, const std::string& id)
{
	StaticDestination* destination = new StaticDestination();
	destination->MeshName = StringUtils::ToNarrow(node->GetName());
	destination->MaterialName = GetMaterial(node);
	return destination;
}

Path* SGMExporter::ProcessPath(IGameNode* node)
{
	Path* path = new Path();

	node->GetIGameObject()->InitializeData();

	IGameControl *gControl = node->GetIGameControl();
	if (gControl != NULL)
		ExtractKeys(gControl, path->Keys);

	node->ReleaseIGameObject();

	float spread = 1.0f;
	GetPropertyFloat(node, "radius", spread);

	float triangle_scale = 0.5f;
	GetPropertyFloat(node, "triangle_scale", triangle_scale);

	float delay = 4.0f;
	GetPropertyFloat(node, "delay", delay);

	bool dontRender = false;
	GetPropertyBool(node, "dont_render", dontRender);

	ProcessFloatProperty(node, "ribbon_weight", path->RibbonWeights);

	path->Spread = spread;
	path->TriangleScale = triangle_scale;
	path->Delay = delay;
	path->DontRender = dontRender;

	return path;
}

void SGMExporter::ProcessIntProperty(IGameNode* node, const std::string& name, std::vector<Key<int>*>& keys)
{
	IGameObject* obj = node->GetIGameObject();
	if (obj == NULL)
		return;

	IPropertyContainer* propsCointainer = obj->GetIPropertyContainer();
	if (propsCointainer == NULL)
		return;

	IGameProperty *property = propsCointainer->QueryProperty(StringUtils::ToWide(name).c_str());
	if (property == NULL)
		return;

	if (!property->IsPropAnimated())
		return;

	IGameControl *ctrl = property->GetIGameControl();
	if (ctrl == NULL)
		return;

	IGameKeyTab gKeys;
	if (ctrl->GetLinearKeys(gKeys, IGAME_FLOAT))
	{
		for (int i = 0; i < gKeys.Count(); i++)
		{
			Key<int>* key = new Key<int>();
			key->Time = TicksToSec(gKeys[i].t);
			key->Value = (int)gKeys[i].linearKey.fval;

			keys.push_back(key);
		}
	}
}

void SGMExporter::ProcessFloatProperty(IGameNode* node, const std::string& name, std::vector<Key<float>*>& keys)
{
	IGameObject* obj = node->GetIGameObject();
	if (obj == NULL)
		return;

	IPropertyContainer* propsCointainer = obj->GetIPropertyContainer();
	if (propsCointainer == NULL)
		return;

	IGameProperty *property = propsCointainer->QueryProperty(StringUtils::ToWide(name).c_str());
	if (property == NULL)
		return;

	if (!property->IsPropAnimated())
		return;

	IGameControl *ctrl = property->GetIGameControl();
	if (ctrl == NULL)
		return;

	IGameKeyTab gKeys;
	if (ctrl->GetTCBKeys(gKeys, IGAME_FLOAT))
	{
		for (int i = 0; i < gKeys.Count(); i++)
		{
			Key<float>* key = new Key<float>();
			key->Time = TicksToSec(gKeys[i].t);
			key->Value = gKeys[i].tcbKey.fval;

			keys.push_back(key);
		}
	}
}

Guy* SGMExporter::ProcessGuy(IGameNode* node, const std::string& guyId)
{
	Guy* guy = new Guy();

	guy->Id = guyId;
	guy->Path = ProcessPath(node);
	guy->MaterialName = GetMaterial(node);

	std::string ribbonName;
	GetPropertyString(node, "ribbon", ribbonName);
	guy->RibbonName = ribbonName;

	ProcessIntProperty(node, "anim_index", guy->AnimationIndex);

	return guy;
}

CustomSceneElement* SGMExporter::ProcessCustomSceneElement(IGameNode* node, const std::string& type, const std::string& id)
{
	CustomSceneElement* element = new CustomSceneElement();

	element->Type = type;
	element->Id = id;
	element->MeshName = StringUtils::ToNarrow(node->GetName());
	element->MaterialName = GetMaterial(node);

	//TODO: pobierz CustomAttributes i wsadz je do element->Parameters

	return element;
}

Ribbon* SGMExporter::GetRibbonByName(const std::string& name)
{
	RibbonsMap::iterator it = m_ribbons.find(name);
	if (it == m_ribbons.end())
		return NULL;

	return it->second;
}

Ribbon* SGMExporter::GetOrCreateRibbon(const std::string& name)
{
	Ribbon* ribbon = GetRibbonByName(name);
	if (ribbon == NULL)
	{
		ribbon = new Ribbon();
		ribbon->Source = NULL;
		ribbon->Destination = NULL;
		ribbon->Path = NULL;
		ribbon->StaticSource = NULL;
		ribbon->StaticDestination = NULL;

		m_ribbons[name] = ribbon;
	}

	return ribbon;
}

void SGMExporter::ExtractKeys(IGameControl *gControl, std::vector<TransformKey*>& keys)
{
	if (!gControl->IsAnimated(IGAME_POS))
	{
		Log::LogT("No position keys");
		return;
	}

	IGameControl::MaxControlType controlType =
		gControl->GetControlType(IGAME_POS);

	IGameKeyTab tcbKeys;
	if (controlType == IGameControl::IGAME_MAXSTD &&
		gControl->GetTCBKeys(tcbKeys, IGAME_POS))
	{
		for (int i = 0; i < tcbKeys.Count(); i++)
		{
			TransformKey* key = new TransformKey();

			key->Time = TicksToSec(tcbKeys[i].t);
			key->Position.Set(tcbKeys[i].tcbKey.pval.x, tcbKeys[i].tcbKey.pval.y, tcbKeys[i].tcbKey.pval.z);
			key->Rotation.Set(0.0f, 1.0f, 0.0f, 0.0f);
			key->Scale.Set(1.0f, 1.0f, 1.0f);

			keys.push_back(key);
		}
	}
	else
	{
		Log::LogT("No TCB Keys");
	}
}

void SGMExporter::WriteMaterials(XmlWriter& xml)
{
	xml.OpenElement("Materials");

	for (MaterialsMap::iterator it = m_materials.begin(); it != m_materials.end(); it++)
		WriteMaterial(xml, it->second);

	xml.CloseElement();
}

void SGMExporter::WritePath(XmlWriter& xml, Path* path)
{
	xml.OpenElement("Path");
	xml.WriteAttribute("spread", path->Spread);
	xml.WriteAttribute("triangle_scale", path->TriangleScale);
	xml.WriteAttribute("delay", path->Delay);
	if (path->DontRender)
		xml.WriteAttribute("dont_render", path->DontRender);

	if (path->RibbonWeights.size() > 0)
	{
		xml.OpenElement("RibbonWeights");
		WriteFloatKeys(xml, path->RibbonWeights);
		xml.CloseElement();
	}

	for (int j = 0; j < path->Keys.size(); j++)
	{
		TransformKey* key = path->Keys[j];

		char posTxt[128];
		char rotTxt[128];
		char scaleTxt[128];

		sprintf(posTxt, "%f;%f;%f", key->Position.x, key->Position.y, key->Position.z);
		sprintf(rotTxt, "%f;%f;%f,%f", key->Rotation.w, key->Rotation.x, key->Rotation.y, key->Rotation.z);
		sprintf(scaleTxt, "%f;%f;%f", key->Scale.x, key->Scale.y, key->Scale.z);

		xml.OpenElement("Key");
		xml.WriteAttribute<float>("time", path->Keys[j]->Time);
		xml.WriteAttribute<const char*>("position", posTxt);
		//xmlWriter.WriteAttribute<const char*>("rotation", rotTxt);
		//xmlWriter.WriteAttribute<const char*>("scale", scaleTxt);
		xml.CloseElement();
	}

	xml.CloseElement();
}

void SGMExporter::WriteIntKeys(XmlWriter& xml, std::vector<Key<int>*>& keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		xml.CreateElement("Key", "time", keys[i]->Time, "value", keys[i]->Value);
	}
}

void SGMExporter::WriteFloatKeys(XmlWriter& xml, std::vector<Key<float>*>& keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		xml.CreateElement("Key", "time", keys[i]->Time, "value", keys[i]->Value);
	}
}

void SGMExporter::WriteStaticNodes(XmlWriter& xml, const std::vector<IGameNode*>& staticNodes)
{
	xml.OpenElement("StaticMeshes");

	for (uint32_t i = 0; i < staticNodes.size(); i++)
	{
		IGameNode* node = staticNodes[i];

		IGameObject *gameObject = node->GetIGameObject();
		assert(gameObject != NULL);

		if (gameObject->GetIGameType() == IGameObject::IGAME_MESH)
		{
			xml.OpenElement("Static");
			xml.WriteAttribute("mesh_name", StringUtils::ToNarrow(node->GetName()));

			int order = 0;
			if (GetPropertyInt(node, "order", order))
				xml.WriteAttribute("order", order);

			std::string materialName = GetMaterial(node);
			WriteMaterialAttribIfExist(xml, materialName);

			xml.CloseElement();
		}

		node->ReleaseIGameObject();
	}

	xml.CloseElement();
}

void SGMExporter::WriteMaterial(XmlWriter& xml, Material* material)
{
	xml.OpenElement("Material");
	xml.WriteAttribute("name", material->Name);
	xml.WriteAttribute("shader", material->ShaderName);

	xml.OpenElement("Parameters");

	const Material::ParametersMap& parameters = material->GetParameters();
	Material::ParametersMap::const_iterator it;

	for (it = parameters.begin(); it != parameters.end(); ++it)
	{
		std::string serializedParameter = Serializers::CustomParameterSerializer::Serialize(*it->second);
		xml.CreateElementInline(serializedParameter);
	}

	xml.CloseElement(); // Parameters
	xml.CloseElement(); // Material
}

void SGMExporter::WriteGuys(XmlWriter& xml)
{
	xml.OpenElement("Guys");

	for (uint32_t i = 0; i < m_guys.size(); i++)
	{
		Guy* guy = m_guys[i];

		xml.OpenElement("Guy");
		xml.WriteAttribute("id", guy->Id.c_str());

		if (guy->RibbonName.size() > 0)
			xml.WriteAttribute("ribbon_name", guy->RibbonName.c_str());

		WriteMaterialAttribIfExist(xml, guy->MaterialName);

		if (guy->Path != NULL)
			WritePath(xml, guy->Path);

		xml.OpenElement("AnimationIndex");
		WriteIntKeys(xml, guy->AnimationIndex);
		xml.CloseElement();

		xml.CloseElement();
	}

	xml.CloseElement();
}

void SGMExporter::WriteCustomSceneElements(XmlWriter& xml)
{
	xml.OpenElement("Elements");

	for (uint32_t i = 0; i < m_customSceneElements.size(); i++)
	{
		CustomSceneElement* element = m_customSceneElements[i];

		xml.OpenElement(element->Type.c_str());
		xml.WriteAttribute("id", element->Id.c_str());
		xml.WriteAttribute("mesh_name", element->MeshName.c_str());

		WriteMaterialAttribIfExist(xml, element->MaterialName);

		xml.CloseElement();
	}

	xml.CloseElement();
}

void SGMExporter::WriteMaterialAttribIfExist(XmlWriter& xml, const std::string& materialName)
{
	if (materialName.size() > 0)
		xml.WriteAttribute<const char*>("material_name", materialName.c_str());
}

IGameProperty* SGMExporter::GetProperty(IGameNode* node, const std::string& name)
{
	IGameObject *gameObject = node->GetIGameObject();
	gameObject->InitializeData();
	if (gameObject == NULL)
		return NULL;

	if (gameObject->GetIGameType() != IGameObject::IGAME_MESH)
	{
		node->ReleaseIGameObject();
		return NULL;
	}

	IPropertyContainer* propCont = gameObject->GetIPropertyContainer();
	if (propCont == NULL)
	{
		node->ReleaseIGameObject();
		return NULL;
	}

	return propCont->QueryProperty(StringUtils::ToWide(name).c_str());
}

bool SGMExporter::GetPropertyFloat(IGameNode* node, const std::string& name, float& value)
{
	IGameProperty* prop = GetProperty(node, name);
	if (prop == NULL || !prop->GetPropertyValue(value))
	{
		node->ReleaseIGameObject();
		return false;
	}
	
	node->ReleaseIGameObject();

	return true;
}

bool SGMExporter::GetPropertyBool(IGameNode* node, const std::string& name, bool& value)
{
	int iValue = 0;

	IGameProperty* prop = GetProperty(node, name);
	if (prop == NULL || !prop->GetPropertyValue(iValue))
	{
		node->ReleaseIGameObject();
		return false;
	}

	value = iValue != 0;

	node->ReleaseIGameObject();

	return true;
}

bool SGMExporter::GetPropertyInt(IGameNode* node, const std::string& name, int& value)
{
	IGameProperty* prop = GetProperty(node, name);
	if (prop == NULL || !prop->GetPropertyValue(value))
	{
		node->ReleaseIGameObject();
		return false;
	}

	node->ReleaseIGameObject();

	return true;
}

bool SGMExporter::GetPropertyString(IGameNode* node, const std::string& name, std::string& value)
{
	wchar_t *sValue;

	IGameProperty* prop = GetProperty(node, name);
	if (prop == NULL || !prop->GetPropertyValue((const wchar_t*&)sValue))
	{
		node->ReleaseIGameObject();
		return false;
	}
	
	value = StringUtils::ToNarrow(sValue);

	node->ReleaseIGameObject();

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

bool SGMExporter::WriteSceneToFile()
{
	std::ofstream file(fileName);
	if (file.fail())
	{
		Log::LogT("error: couldn't open file");
		return false;
	}

	XmlWriter xmlWriter(&file, 0);

	xmlWriter.OpenElement("Scene");
	xmlWriter.WriteAttribute("name", sceneName.c_str());

	xmlWriter.OpenElement("GameObjects");
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		GameObject* gameObject = (*it);

		if (gameObject->IsEmpty())
			continue;
		
		xmlWriter.CreateElementInline(gameObject->Serialize());
	}
	xmlWriter.CloseElement(); // GameObjects

	//WriteMaterials(xmlWriter);

	xmlWriter.CloseElement(); // Scene

	file.close();

	return true;
}

