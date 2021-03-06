#pragma once

#include <Egg/Math/Float4x4.h>
#include <Egg/Scene/PerObjectData.h>

using namespace Egg::Math;

__declspec(align(16)) struct PerObjectCb {
	Egg::Scene::PerObjectData objects[1024];
};


__declspec(align(16)) struct PerFrameCb {
	Float4x4 viewProjTransform;
	Float4x4 rayDir;
	Float4 cameraPos;
	Float4 lightPos;
	Float4 lightPowerDensity;
	Float4 billboardSize;
};

