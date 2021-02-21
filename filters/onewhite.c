#include "MagickCore/studio.h"
#include "MagickCore/list.h"
#include "MagickCore/log.h"
#include "MagickCore/module.h"
#include "MagickCore/pixel-accessor.h"
#include "MagickCore/property.h"
#include "MagickCore/thread-private.h"


static MagickBooleanType
onewhite(Image *image, ExceptionInfo *exception)
{
    assert(image);
    assert(image->signature == MagickCoreSignature);
    if (image->debug)
        LogMagickEvent(TraceEvent, GetMagickModule(), "%s", image->filename);

    MagickBooleanType status = MagickTrue;

    CacheView *image_view = AcquireVirtualCacheView(image, exception);

    ssize_t  ry = image->rows;
    ssize_t *rx = malloc(image->rows * sizeof(rx));

    if (!rx)
        return MagickFalse;

#if defined(MAGICKCORE_OPENMP_SUPPORT)
    #pragma omp parallel for schedule(static) reduction(min:ry) \
        magick_number_threads(image, image, image->rows, 1)
#endif
    for (ssize_t y = 0; y < (ssize_t)image->rows; y++)
    {
        if (status == MagickFalse || ry <= y)
            continue;

        const Quantum *p = GetCacheViewVirtualPixels(
                image_view, 0, y, image->columns, 1, exception
        );

        if (!p) {
            status = MagickFalse;
            continue;
        }

        for (ssize_t x = 0; x < (ssize_t)image->columns; x++)
        {
            if (GetPixelRed  (image, p) >= QuantumRange - MagickEpsilon &&
                GetPixelGreen(image, p) >= QuantumRange - MagickEpsilon &&
                GetPixelBlue (image, p) >= QuantumRange - MagickEpsilon)
            {
                rx[y] = x;
                ry = y;
                break;
            }

            p += GetPixelChannels(image);
        }
    }

    if (status && ry < image->rows)
    {
        char text[MagickPathExtent];

        FormatLocaleString(
                text, MagickPathExtent, "(%ld,%ld)", rx[ry], ry
        );

        SetImageProperty(image, "filter:onewhite", text, exception);
    }
    else
        status = MagickFalse;

    free(rx);

    return status;
}


ModuleExport size_t
onewhiteImage(Image **images,
             const int argc, const char **argv,
             ExceptionInfo *exception)
{
    assert(images);

    (void)argc;
    (void)argv;

    Image *image = (*images);
    for (; image; image = GetNextImageInList(image))
        onewhite(image, exception);

    return MagickImageFilterSignature;
}
