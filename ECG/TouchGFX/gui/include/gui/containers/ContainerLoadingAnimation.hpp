#ifndef CONTAINERLOADINGANIMATION_HPP
#define CONTAINERLOADINGANIMATION_HPP

#include <gui_generated/containers/ContainerLoadingAnimationBase.hpp>

class ContainerLoadingAnimation : public ContainerLoadingAnimationBase
{
public:
    ContainerLoadingAnimation();
    virtual ~ContainerLoadingAnimation() {}

    virtual void initialize();

    virtual void StartLoadingAnim();
    virtual void StopLoadingAnim();
protected:
};

#endif // CONTAINERLOADINGANIMATION_HPP
