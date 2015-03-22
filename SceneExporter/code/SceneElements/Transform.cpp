#include "Transform.h"
#include <XML/XmlWriter.h>
#include <IGame/IGame.h>

Transform::Transform() :
	m_position(0, 0, 0),
	m_rotation(0, 1, 0, 0),
	m_scale(1, 1, 1)
{

}

void Transform::SetFromNode(IGameNode* node)
{
	if (node == NULL)
		return;

	GMatrix worldMatrix = node->GetObjectTM();

	Point3 igPosition = worldMatrix.Translation();
	AngAxis igRotation(worldMatrix.Rotation());
	Point3 igScale = worldMatrix.Scaling();

	m_position = sm::Vec3(igPosition.x, igPosition.y, igPosition.z);
	m_rotation = sm::Vec4(igRotation.angle, igRotation.axis.x, igRotation.axis.y, igRotation.axis.z);
	m_scale = sm::Vec3(igScale.x, igScale.y, igScale.z);
}

std::string Transform::Serialize()
{
	std::stringstream ss;

	XmlWriter xml(&ss, 0);
	xml.OpenElement("Transform");
	xml.WriteAttribute("position", StringUtils::ToString(m_position));
	xml.WriteAttribute("rotation", StringUtils::ToString(m_rotation));
	xml.WriteAttribute("scale", StringUtils::ToString(m_scale));
	xml.CloseElement();

	return ss.str();
}

