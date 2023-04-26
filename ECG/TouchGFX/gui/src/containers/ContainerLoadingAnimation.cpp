#include <gui/containers/ContainerLoadingAnimation.hpp>

ContainerLoadingAnimation::ContainerLoadingAnimation()
{

}

void ContainerLoadingAnimation::initialize()
{
    ContainerLoadingAnimationBase::initialize();
}

void ContainerLoadingAnimation::StartLoadingAnim()
{
    animatedImage1.startAnimation(false, true, true);
}

void ContainerLoadingAnimation::StopLoadingAnim()
{
	animatedImage1.stopAnimation();
}
