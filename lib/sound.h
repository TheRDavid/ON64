/**
 * @file sound.h
 * @brief Music Stuff
 * @ingroup misc
 */
#ifndef __SOUND_H
#define __SOUND_H

struct SAMPLE *sampleList[64]; // pointer to soundList array
char **soundNameList;
int numSounds, numVoices;
SBYTE* voices;
int lastVoice;

void sound_init(char *list[25], int nSounds, int nVoices);

void sound_update();

void sound_playSample(int index);

void sound_playAlone(int index);

void sound_playModule(int index);

#endif
