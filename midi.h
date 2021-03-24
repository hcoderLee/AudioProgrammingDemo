/// the type of midi note number
typedef unsigned short MIDI_NOTE;

/// return the nearest midi note number of the frequency
MIDI_NOTE freq2midi(double freq);

/// retuen the frequency of a midi note number
double midi2freq(MIDI_NOTE note);

/// return note name of midi note number, e.g. return A4 for note number 69
/// @param flag: mark the note name is sharp or flat, 0 is sharp, 1 is flat
char *note_name(MIDI_NOTE note, int flag);

/// the array composed by 7 numbers which describe the scale's intervals
/// 0 means semiton, 1 means whole tone
typedef short SCALE[7];

/// the intervals relationship of natural major scale
SCALE MAJOR_SCALE = {1, 1, 0, 1, 1, 1, 0};