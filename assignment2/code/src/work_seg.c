#include "work_seg.h"

#include <string.h>

unsigned long
work_seg(
    struct work_seg *range,
    struct work_seg *segments,
    int nsegments,
    unsigned long (*f)(unsigned long))
{
    if (!range || !segments || !f)
    {
        return 0;
    }

    /* initialize input segments */
    memset(segments, 0x00, sizeof(struct work_seg) * nsegments);

    /* check input arguments */
    if (range->a >= range->b)
    {
        return 0;
    }

    unsigned long total_work = f(range->b) - f(range->a);
    unsigned long work_size = total_work / nsegments;

    /* i don't even care */
    unsigned long cursor = range->a;

    int i = 0;
    for (i = 0; i < nsegments; i++)
    {
        segments[i].a = cursor;
        segments[i].b = cursor;

        /* last segment goes to end of range */
        if (i == nsegments - 1)
        {
            segments[i].b = range->b;
            break;
        }

        /* binary search to find end of segment, assumes f is always increasing */
        unsigned long search = range->b;
        unsigned long seg_size = search - cursor + 1;

        while(1)
        {
            unsigned long current_work_size = f(search) - f(cursor);
            seg_size = seg_size / 2;

            if(seg_size == 0)
            {
                break;
            }

            if (current_work_size > work_size)
            {
                search -= seg_size;
            }
            else if (current_work_size < work_size)
            {
                search += seg_size;
            }
            else
            {
                break;
            }
        }

        segments[i].b = search;
        cursor = segments[i].b + 1;
    }

    return work_size;
}
