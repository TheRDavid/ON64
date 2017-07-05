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

 /*
   Function: sound_init

   Initializes the sound-subsystem of libdragon

   Parameters:

   char* list[25] - names of the sound files
   int nSounds - number of sounds
   int nSounds - number of voices

   Returns:

   A pointer to a sprite-instance that contains the image data of the file

   TODO:

   - make it less ugly

*/
void sound_init(char *list[25], int nSounds, int nVoices);

 /*
   Function: sound_update

   Only exists to wrap the oddly sounding mikmod command

   TODO:

   - maybe... get rid of it??

*/
void sound_update();


 /*
   Function: sound_playSample

   WOULD play a WAVE-sample if the audio-library supported it...

   Parameters:

   - int index - index of the sample

   TODO:

   - update / patch mikmod!
   - allow volume control

*/
void sound_playSample(int index);

 /*
   Function: sound_playAlone

   WOULD play a WAVE-sample if the audio-library supported it...
   Also mutes all other samples

   Parameters:

   - int index - index of the sample

   TODO:

   - update / patch mikmod!
   - allow volume control

*/
void sound_playAlone(int index);


 /*
   Function: sound_playModule

   Plays module

   Parameters:

   - int index - index of the module

*/
void sound_playModule(int index);

#endif
