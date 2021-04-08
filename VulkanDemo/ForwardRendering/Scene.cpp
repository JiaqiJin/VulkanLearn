#include "Scene.h"

TestSceneConfiguration& getGlobalTestSceneConfiguration()
{
	static TestSceneConfiguration sp;
	return sp;
}