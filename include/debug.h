#ifndef DEBUG_H
#define DEBUG_H



#define DEBUG_SKIP_INTROS 	0			// set to skip straight to gameplay
#define DEBUG_MUTE_SONGS 	0			// set to mute audio tracks
#define DEBUG_MUTE_SFX 		0			// set to mute sound effects
#define DEBUG_MUTE_ALL		0			// set to mute everything

void debug_write_text(char *str);
void debug_write_int(int num);

#endif //DEBUG_H