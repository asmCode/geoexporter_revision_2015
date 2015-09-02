#include "SGMExporter.h"

#include "scene3d/Model.h"
#include "scene3d/Mesh.h"

#include <Utils/StringUtils.h>
#include <Utils/Log.h>
#include <Math/Matrix.h>

#include <IGame/IGame.h>

SGMExporter::SGMExporter() :
	m_scene(NULL)
{
}

SGMExporter::~SGMExporter()
{
}

void SGMExporter::ExtractVertices(IGameNode* gNode, IGameMesh* gMesh, FuturisEngine::Export::Mesh* mesh)
{
	assert(gMesh != NULL);
	assert(mesh != NULL);

	gMesh->InitializeBinormalData();

	for (int i = 0; i < gMesh->GetNumberOfFaces(); i++)
	{
		FaceEx* face = gMesh->GetFace(i);
		if (face == NULL)
			continue;

		if (gMesh->GetNumberOfVerts() > 0)
			mesh->AddVertexTriangle((uint16_t)face->vert[0], (uint16_t)face->vert[1], (uint16_t)face->vert[2]);

		if (gMesh->GetNumberOfNormals() > 0)
			mesh->AddNormalTriangle((uint16_t)face->norm[0], (uint16_t)face->norm[1], (uint16_t)face->norm[2]);

		if (gMesh->GetNumberOfTangents() > 0)
		{
			mesh->AddTangentTriangle(
				(uint16_t)gMesh->GetFaceVertexTangentBinormal(i, 0),
				(uint16_t)gMesh->GetFaceVertexTangentBinormal(i, 1),
				(uint16_t)gMesh->GetFaceVertexTangentBinormal(i, 2));
		}

		if (gMesh->GetNumberOfMapVerts(1) > 0)
		{
			mesh->AddCoord1Triangle(
				(uint16_t)gMesh->GetFaceTextureVertex(i, 0, 1),
				(uint16_t)gMesh->GetFaceTextureVertex(i, 1, 1),
				(uint16_t)gMesh->GetFaceTextureVertex(i, 2, 1));
		}
	}

	GMatrix worldMatrixInv = gNode->GetWorldTM().Inverse();

	for (int i = 0; i < gMesh->GetNumberOfVerts(); i++)
	{
		Point3 position = gMesh->GetVertex(i) * worldMatrixInv;
		mesh->AddVertex(position.x, position.y, position.z);
	}

	for (int i = 0; i < gMesh->GetNumberOfNormals(); i++)
	{
		Point3 normal = worldMatrixInv.ExtractMatrix3().VectorTransform(gMesh->GetNormal(i));
		mesh->AddNormal(normal.x, normal.y, normal.z);
	}

	for (int i = 0; i < gMesh->GetNumberOfTangents(); i++)
	{
		Point3 tangent = worldMatrixInv.ExtractMatrix3().VectorTransform(gMesh->GetTangent(i));
		mesh->AddTangent(tangent.x, tangent.y, tangent.z);
	}

	for (int i = 0; i < gMesh->GetNumberOfMapVerts(1); i++)
	{
		Point3 coord = gMesh->GetMapVertex(1, i);
		mesh->AddCoord1(coord.x, coord.y);
	}
}

void SGMExporter::ExtractMesh(IGameNode* gNode, FuturisEngine::Export::Model* model)
{
	if (gNode == NULL)
		return;

	IGameObject* igObject = gNode->GetIGameObject();
	if (igObject == NULL)
		return;

	if (!igObject->InitializeData())
		return;

	if (igObject->GetIGameType() != IGameObject::ObjectTypes::IGAME_MESH)
		return;

	IGameMesh* igMesh = dynamic_cast<IGameMesh*>(igObject);
	if (igMesh == NULL)
		return;

	std::string name = StringUtils::ToNarrow(gNode->GetName());
	FuturisEngine::Export::Mesh* mesh = new FuturisEngine::Export::Mesh(name);
	ExtractVertices(gNode, igMesh, mesh);

	if (mesh->Validate())
		model->AddMesh(mesh);
	else
		delete mesh;

	gNode->ReleaseIGameObject();
}

bool SGMExporter::DoExport(const wchar_t *name, ExpInterface *ei, Interface *max_interface)
{
	m_filename = StringUtils::ToNarrow(name);

	Log::StartLog(true, false, false);
	Log::LogT("=== exporting geometry to file '%s'", m_filename.c_str());

	if (!InitializeScene())
		return false;

	std::vector<IGameNode*> gNodes;
	for (int i = 0; i < m_scene->GetTopLevelNodeCount(); i++)
		FlattenNodes(m_scene->GetTopLevelNode(i), gNodes);

	FuturisEngine::Export::Model* model = new FuturisEngine::Export::Model();

	for (uint32_t i = 0; i < gNodes.size(); i++)
		ExtractMesh(gNodes[i], model);

	m_scene->ReleaseIGame();

	model->Save(m_filename);
	delete model;

	return true;
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

bool SGMExporter::InitializeScene()
{
	m_scene = GetIGameInterface();
	assert(m_scene != NULL);

	IGameConversionManager *cm = GetConversionManager();
	assert(cm != NULL);
	cm->SetCoordSystem(IGameConversionManager::IGAME_OGL);

	if (!m_scene->InitialiseIGame(false))
	{
		Log::LogT("error: couldnt initialize scene");
		return false;
	}

	m_scene->SetStaticFrame(0);

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