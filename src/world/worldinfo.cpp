#include "worldinfo.h"

WorldInfo::WorldInfo(): IntroImage(NULL),VictoryImage(NULL){}

WorldInfo::~WorldInfo()
{
    if(IntroImage)
        delete IntroImage;
    if(VictoryImage)
        delete VictoryImage;
}
