#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"




class TweeningSystem
{
private:
	
public:
	
	void Init();
	void setEasingMethod(EasingMethod easing);
	void update(float dt);
};

