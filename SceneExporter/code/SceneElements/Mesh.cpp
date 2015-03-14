#include "Mesh.h"
#include <XML/XmlWriter.h>
#include <IGame/IGame.h>

namespace SceneElements
{
	Mesh::Mesh()
	{

	}

	Mesh* Mesh::ExtractFromNode(IGameScene* igScene, IGameNode* node)
	{
		if (node == NULL)
			return NULL;

		IGameObject* igObject = node->GetIGameObject();
		if (igObject == NULL)
			return NULL;

		if (igObject->GetIGameType() != IGameObject::ObjectTypes::IGAME_MESH)
			return NULL;

		IGameMesh* igMesh = dynamic_cast<IGameMesh*>(igObject);
		if (igMesh == NULL)
			return NULL;

		IGameMaterial* material = igScene->GetRootMaterial(node->GetMaterialIndex());

		SceneElements::Mesh* mesh = new SceneElements::Mesh();
		mesh->m_meshName = StringUtils::ToNarrow(node->GetName());
		if (material != NULL)
			mesh->m_materialName = StringUtils::ToNarrow(material->GetMaterialName());

		node->ReleaseIGameObject();

		return mesh;
	}

	std::string Mesh::Serialize()
	{
		std::stringstream ss;

		XmlWriter xml(&ss, 0);
		xml.OpenElement("Mesh");
		xml.WriteAttribute("name", m_meshName);
		if (!m_materialName.empty())
			xml.WriteAttribute("material", m_materialName);
		xml.CloseElement();

		return ss.str();
	}
}

