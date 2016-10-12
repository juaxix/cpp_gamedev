#include <Foundation/Foundation.h>

#include "FileHelper.h"

std::string FileHelper::resourcesDirectory()
{
    @autoreleasepool {
        std::string s([[[NSBundle mainBundle] resourcePath] UTF8String]);
        return s+"/";
    }
}