#pragma once

#include <string>
#include <vector>

class AnimationCurve;

class AnimationClip
{
public:
	AnimationClip(const std::string &name);
	~AnimationClip();

	std::string Serialize();

	void AddAnimationCurve(AnimationCurve* animationCurve);

private:
	std::string m_name;
	std::vector<AnimationCurve*> m_animationCurves;
};

