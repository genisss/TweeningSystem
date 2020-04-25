#include "AnimationSystem.h"
#include "extern.h";


//AnimationSystem::~AnimationSystem()
//{
//}

//void AnimationSystem::init()
//{
//}
//
//void AnimationSystem::lateInit()
//{
//}

void AnimationSystem::update(float dt)
{
	//access out animation components and do stuff with them
	auto& anims = ECS.getAllComponents<Animation>();

	for (auto& anim : anims) {
		Transform& transform = ECS.getComponentFromEntity<Transform>(anim.owner);
		anim.ms_counter += dt * 1000;
		if (anim.ms_counter >= anim.ms_frame) {
			anim.ms_counter = anim.ms_counter - anim.ms_frame;

			transform.set(anim.keyframes[anim.curr_frame]);

			anim.curr_frame++;

			if (anim.curr_frame == anim.num_frames)
				anim.curr_frame = 0;

		}
	}
}
