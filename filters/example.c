#include "MagickCore/MagickCore.h"

ModuleExport size_t
exampleImage(Image **images,
             const int argc, const char **argv,
             ExceptionInfo *exception)
{
    return MagickImageFilterSignature;
}
