#include "AnimationClip.h"
#include "AnimationCurve.h"
#include <XML/XmlWriter.h>
#include <stdint.h>

AnimationClip::AnimationClip(const std::string &name)
	: m_name(name)
{
}

AnimationClip::~AnimationClip()
{
	for (uint32_t i = 0; i < m_animationCurves.size(); i++)
		delete m_animationCurves[i];
}

std::string AnimationClip::Serialize()
{
	XmlWriter xml;
	xml.OpenElement("AnimationClip");
	xml.WriteAttribute("name", m_name);

	for (uint32_t i = 0; i < m_animationCurves.size(); i++)
		xml.CreateElementInline(m_animationCurves[i]->Serialize());

	xml.CloseElement();
	return xml.GetAsText();
}

void AnimationClip::AddAnimationCurve(AnimationCurve* animationCurve)
{
	m_animationCurves.push_back(animationCurve);
}

