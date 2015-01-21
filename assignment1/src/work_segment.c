#include "work_segment.h"

struct work_segment
work_segment(
    int nwork,
    int nprocesses,
    int index)
{
    struct work_segment seg =
    {
        .count = 0,
        .start = 0,
        .end = -1
    };

    /* excess processes don't receive any work */
    if (index >= nwork)
    {
        return seg;
    }

    /* calculate the amount of rows for this process */
    int work_per_process = nwork / nprocesses;
    int rem_work = nwork - (work_per_process * nprocesses);

    seg.count = work_per_process;

    if (index < rem_work)
    {
        ++seg.count;
    }

    /* calculate start and end */
    if (index < rem_work)
    {
        seg.start = seg.count * index;
    }
    else
    {
        seg.start = (work_per_process + 1) * rem_work
                  + seg.count * (index - rem_work);
    }
    seg.end = seg.start + seg.count - 1;

    return seg;
}
