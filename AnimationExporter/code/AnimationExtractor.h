#pragma once

#include <string>
#include <IGame/IGame.h>

class AnimationClip;

class AnimationExtractor
{
public:
	AnimationExtractor(const std::string& name, IGameScene* igScene);

	void AddNode(IGameNode* igNode);
	void SaveToFile(const std::string& filename);

private:
	AnimationClip* m_animationClip;
	IGameScene* m_scene;

	IGameControl* GetIGameControl(IGameNode* gNode);

	void ExtractKeys(
		const std::string& nodeName,
		IGameControl *gControl,
		IGameControlType controlType,
		IGameKeyTab& keys);
};

