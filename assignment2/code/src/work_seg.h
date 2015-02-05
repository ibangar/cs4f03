#ifndef __WORK_SEG_H__
#define __WORK_SEG_H__

/*
 * this module calculates a work segment given a weight function, range,
 * interval count and index.
 */

struct work_seg {
    unsigned long a;
    unsigned long b;
};

/*
 * range        - the range (scope) of the work to be done.
 * segments     - the segments for each of the n intervals.
 * nintervals   - the number of intervals to split the range (scope) into.
 * f            - the weight function.
 *
 * return       - the approximate size of each segment.
 */
unsigned long work_seg(struct work_seg *range, struct work_seg *segments,
                       int nintervals, unsigned long (*f)(unsigned long));

#endif
