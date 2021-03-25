#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/// generate exponential attack or decay breakpoints data
/// @param duration: how long time of attack or decay
/// @param npoints: how many points during attack or decay
/// @param startfreq: the frequency at the beginning of attack or decay
/// @param endfreq: the frequency at the end of attack or decay
void expbrk(double duration, int npoints, double startfreq, double endfreq)
{
    if (startfreq == endfreq)
    {
        fprintf(stderr, "cannot attack or decay to the same frequency");
        return;
    }

    // the number near to 0
    const double smallval = 1.0e-4;
    // standardized data, assume it's attack first
    // start and end is belong to interval [0, 1]
    double start = smallval, end = 1;
    double range = endfreq - startfreq, offset = startfreq;
    double startval = smallval;

    if (startfreq > end)
    {
        // decay
        start = 1;
        end = smallval;
        range = -range;
        offset = endfreq;
        startval = startfreq - offset;
    }

    const double step = duration / npoints;
    //the function of frequency at each point is:  frequency = offset + ragne * ratio, and ratio should belong to [0, 1].
    // and ratio is: ration = start * (k ^ point), k is a constant number.
    // when point is npoints, ration = end, end = start * (k ^ npoints), then: k = (end / start) ^ (1 / npoints)
    // then: frequncy = offset + range * start * (k ^ point)
    const double k = pow(end / start, 1.0 / npoints);
    double ratio = 1;
    // the time of each point during attack or decay
    double time = 0;
    // the frequency of each point during attack or decay
    double freq = startfreq;
    printf("%lf, %lf\n", time, freq);
    for (int i = 0; i < npoints; i++)
    {
        ratio = ratio * k;
        time += step;
        freq = offset + range * start * ratio;
        printf("%lf, %lf\n", time, freq);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "please input the correct params: [duration, npoints, startfreq, endfreq]\n");
        exit(1);
    }

    double duration = atof(argv[1]);
    int npoints = atoi(argv[2]);
    double startfreq = atof(argv[3]);
    double endfreq = atof(argv[4]);
    expbrk(duration, npoints, startfreq, endfreq);
    return 0;
}
