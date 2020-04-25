#include "TweeningSystem.h"
#include "extern.h";

void TweeningSystem::Init()
{

}

void TweeningSystem::setEasingMethod(EasingMethod easing)
{

}



float LinearEasing(float time) {
	return time;
}
float SineEasing(float x) {
	return -(cos(3.14159f * x) - 1) / 2;
}
float QuadEasing(float x) {
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}
float CubicEasing(float x) {
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}
float QuartEasing(float x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}
float QuintEasing(float x) {
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}
float ExpoEasing(float x) {
	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5 ? pow(2, 20 * x - 10) / 2
		: (2 - pow(2, -20 * x + 10)) / 2;
}
float CircEasing(float x) {
	return x < 0.5
		? (1 - sqrt(1 - pow(2 * x, 2))) / 2
		: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}
float BackEasing(float x) {
	float c1 = 1.70158f;
	float c2 = c1 * 1.525f;

	return x < 0.5
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}


float ElasticEasing(float x) {
	float c5 = (2 * 3.14159f) / 4.5f;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}
float easeOutBounce(float x) {
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}
float BounceEasing(float x) {
	return x < 0.5
		? (1 - easeOutBounce(1 - 2 * x)) / 2
		: (1 + easeOutBounce(2 * x - 1)) / 2;
}

void TweeningSystem::update(float dt)
{

	auto& easings = ECS.getAllComponents<Easing>();

	for (auto& easing : easings) {
		easing.time_progress += dt;
		if (easing.time_progress > easing.duration)
			easing.time_progress -= easing.duration;

		float t = easing.time_progress / easing.duration;
		float progress = 0;
		switch (easing.actualEasing)
		{
		case Linear:
			progress = LinearEasing(t);
			break;
		case EaseInOutSine:
			progress = SineEasing(t);

			break;
		case EaseInOutQuad:
			progress = QuadEasing(t);
			break;
		case EaseInOutCubic:
			progress = CubicEasing(t);
			break;
		case EaseInOutQuart:
			progress = QuartEasing(t);
			break;
		case EaseInOutQuint:
			progress = QuintEasing(t);
			break;
		case EaseInOutExpo:
			progress = ExpoEasing(t);
			break;
		case EaseInOutCirc:
			progress = CircEasing(t);
			break;
		case EaseInOutBack:
			progress = BackEasing(t);
			break;
		case EaseInOutElastic:
			progress = ElasticEasing(t);
			break;
		case EaseInOutBounce:
			progress = BounceEasing(t);
			break;
		default:
			break;
		}

		Transform& transform = ECS.getComponentFromEntity<Transform>(easing.owner);
		//transform.position(easing.from_pos * (1 - progress) + easing.to_pos * progress);



		lm::mat4 new_frame;
		//translation matrix
		lm::mat4 translation;
		if (easing.ease_pos)
			translation.makeTranslationMatrix(easing.from_pos * (1 - progress) + easing.to_pos * progress);

		//rotation matrix
		lm::vec3 rot = easing.from_rot * (1 - progress) + easing.to_rot * progress;
		lm::quat qrot(rot.x, rot.y, rot.z);
		lm::mat4 rotation;
		if (easing.ease_rot)

			rotation.makeRotationMatrix(qrot);
		//scale Matrix
		lm::mat4 scale;
		if (easing.ease_scale)

			scale.makeScaleMatrix(easing.from_scale * (1 - progress) + easing.to_scale * progress);


		new_frame = translation * rotation * scale * new_frame;

		transform.set(new_frame);

	}
}

