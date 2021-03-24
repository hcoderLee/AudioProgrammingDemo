#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "midi.h"

// frequency of note c0
static double c0;
static short C0_OFFSET = 12;
// semiton increament ration, c# = c * semiton_ration
static double semiton_ratio;

int main(int argc, char const *argv[])
{
    // semiton_ratio^12 = 2
    semiton_ratio = pow(2, 1 / 12.0);
    double a5 = 440;
    double c5 = a5 * pow(semiton_ratio, 3);
    // c5 = c0 * (2^5)
    c0 = c5 * pow(0.5, 5);
    if (argc < 2)
    {
        return 1;
    }
    // double freq = atof(argv[1]);
    // MIDI_NOTE note = freq2midi(freq);
    // printf("convert frequency %.2lf to note %d\n", freq, note);

    MIDI_NOTE note = atoi(argv[1]);
    double freq = midi2freq(note);
    char *name = note_name(note, 0);
    printf("convert midi note %d (named %s) to frequency %.2lf\n", note, name, freq);
    free(name);
    return 0;
}

double midi2freq(MIDI_NOTE note)
{
    return c0 * pow(semiton_ratio, note - C0_OFFSET);
}

MIDI_NOTE freq2midi(double freq)
{
    // freq = c0 * (semiton_ration^midi_note)
    // log_a(N) = log_b(N) / log_b(a)
    double note = log(freq / c0) / log(semiton_ratio);
    return round(note) + C0_OFFSET;
}

/// get next note in octave, e.g. G->A, B->C
static inline char next_note(char note)
{
    if (note < 'A' || note > 'G')
    {
        return 0;
    }
    return ++note > 'G' ? 'A' : note;
}

char *note_name(MIDI_NOTE note, int flag)
{
    if (note < 21)
        return (char *)0;
    // which octave the note is
    short level = (note - C0_OFFSET) / 12;
    if (level > 9)
    {
        fprintf(stderr, "invalid octave %d\n", level);
        exit(1);
    }
    // position of the note in current octave
    short p = note % 12;
    // name is the note name without octave number, e.g. C, D, E
    // mark is the flat or sharp symbol, which could be null
    char name = 'C', mark = 0;
    short n = 0;
    for (int i = 0; i < 7; i++)
    {
        if (name == 0)
        {
            fprintf(stderr, "invalid note name!");
            exit(1);
        }

        if (p == n)
        {
            break;
        }
        if (MAJOR_SCALE[i] == 0)
        {
            // semiton interval
            n++;
            name = next_note(name);
        }
        else if (MAJOR_SCALE[i] == 1)
        {
            // whole tone scale
            if (++n == p)
            {
                if (flag == 0)
                {
                    // sharp note
                    mark = '#';
                }
                else
                {
                    // flat note
                    name = next_note(name);
                    mark = 'b';
                }
                break;
            }
            n++;
            name = next_note(name);
        }
    }
    // remember to call free after return value is not used
    char *note_name = malloc(sizeof(char) * 4);
    // concact note name
    char *s = note_name;
    *s++ = name;
    if (mark != 0)
    {
        *s++ = mark;
    }
    *s++ = '0' + level;
    *s = 0;
    return note_name;
}