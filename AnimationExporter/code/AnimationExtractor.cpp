#pragma once

#include "AnimationExtractor.h"
#include "AnimationData/AnimationClip.h"
#include "AnimationData/AnimationCurve.h"
#include "AnimationData/KeyFrameVec3.h"
#include "AnimationData/KeyFrameQuat.h"
#include <IO/Path.h>
#include <Utils/Log.h>
#include <Utils/StringUtils.h>

AnimationExtractor::AnimationExtractor(const std::string& name, IGameScene* igScene)
	: m_scene(igScene)
{
	m_animationClip = new AnimationClip(name);
}

void AnimationExtractor::AddNode(IGameNode* gNode)
{
	std::string nodeName = StringUtils::ToNarrow(gNode->GetName());
	Log::LogT("Exporting node: %s", nodeName.c_str());

	if (gNode == NULL)
		return;

	IGameControl* gControl = GetIGameControl(gNode);
	if (gControl == NULL)
	{
		gNode->ReleaseIGameObject();
		return;
	}

	IGameKeyTab positionKeys;
	IGameKeyTab rotationKeys;
	IGameKeyTab scaleKeys;

	ExtractKeys(nodeName, gControl, IGAME_POS, positionKeys);
	ExtractKeys(nodeName, gControl, IGAME_ROT, rotationKeys);
	ExtractKeys(nodeName, gControl, IGAME_SCALE, scaleKeys);

	if (positionKeys.Count() == 0 &&
		rotationKeys.Count() == 0 &&
		scaleKeys.Count() == 0)
	{
		gNode->ReleaseIGameObject();
		return;
	}

	if (positionKeys.Count() > 0)
	{
		AnimationCurve* animationCurve = new AnimationCurve(PropertyType_Vec3, nodeName, "Transform", "Position");
		for (int i = 0; i < positionKeys.Count(); i++)
		{
			KeyFrameVec3* key = new KeyFrameVec3(
				TicksToSec(positionKeys[i].t),
				positionKeys[i].tcbKey.pval.x,
				positionKeys[i].tcbKey.pval.y,
				positionKeys[i].tcbKey.pval.z);

			animationCurve->AddKeyframe(key);
		}

		m_animationClip->AddAnimationCurve(animationCurve);
	}

	if (rotationKeys.Count() > 0)
	{
		Quat netQuat;
		netQuat.Identity();

		AnimationCurve* animationCurve = new AnimationCurve(PropertyType_Quat, nodeName, "Transform", "Rotation");
		for (int i = 0; i < rotationKeys.Count(); i++)
		{
			Quat quat;
			quat.Set(rotationKeys[i].tcbKey.aval);

			netQuat = netQuat * quat;
			netQuat.Normalize();

			AngAxis angAxis;
			angAxis.Set(netQuat);

			KeyFrameQuat* key = new KeyFrameQuat(
				TicksToSec(rotationKeys[i].t),
				angAxis.angle,
				angAxis.axis.x,
				angAxis.axis.y,
				angAxis.axis.z);

			animationCurve->AddKeyframe(key);
		}

		m_animationClip->AddAnimationCurve(animationCurve);
	}

	if (scaleKeys.Count() > 0)
	{
		AnimationCurve* animationCurve = new AnimationCurve(PropertyType_Vec3, nodeName, "Transform", "Scale");
		for (int i = 0; i < scaleKeys.Count(); i++)
		{
			KeyFrameVec3* key = new KeyFrameVec3(
				TicksToSec(scaleKeys[i].t),
				scaleKeys[i].tcbKey.sval.s.x,
				scaleKeys[i].tcbKey.sval.s.y,
				scaleKeys[i].tcbKey.sval.s.z);

			animationCurve->AddKeyframe(key);
		}

		m_animationClip->AddAnimationCurve(animationCurve);
	}

	gNode->ReleaseIGameObject();
}

void AnimationExtractor::SaveToFile(const std::string& filename)
{
	if (m_animationClip == NULL)
		return;

	std::string xml = m_animationClip->Serialize();
	Path::WriteTextFile(filename.c_str(), xml);
}

IGameControl* AnimationExtractor::GetIGameControl(IGameNode* gNode)
{
	IGameObject *gObject = gNode->GetIGameObject();
	if (gObject == NULL)
		return NULL;

	gObject->InitializeData();

	return gNode->GetIGameControl();
}

void AnimationExtractor::ExtractKeys(
	const std::string& nodeName,
	IGameControl *gControl,
	IGameControlType controlType,
	IGameKeyTab& keys)
{
	if (gControl == NULL || !gControl->IsAnimated(controlType))
		return;

	IGameControl::MaxControlType maxControlType = gControl->GetControlType(controlType);

	if (maxControlType != IGameControl::IGAME_MAXSTD ||
		!gControl->GetTCBKeys(keys, controlType))
	{
		Log::LogT("warning: node '%s' doesn't have TCB controller", nodeName.c_str());
	}
}
