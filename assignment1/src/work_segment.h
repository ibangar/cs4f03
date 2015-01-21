#ifndef __WORK_SEGMENT_H__
#define __WORK_SEGMENT_H__

/*
 * This file generalizes the idea of dividing n amount of work to m processes.
 * It can be used to retrieve the work segment for a given process index.
 */

struct work_segment {
    int count;
    int start;
    int end;
};

struct work_segment work_segment(int nwork, int nprocesses, int index);

#endif
