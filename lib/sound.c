#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include <stdlib.h>
#include <time.h>
#include <audio.h>
#include <unistd.h>
#include <mikmod.h>
#include <grafix.h>
#include <tools.h>
#include <sound.h>


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
void sound_init(char *list[25], int nSounds, int nVoices)
{ 
	soundNameList = list;
	tools_print("Sounds:");
	for(int i = 0; i < nSounds; i++)
	{
		tools_print(soundNameList[i]);
	}
	
	audio_init(11025,2);
    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();
        
    md_mode = DMODE_STEREO | DMODE_SURROUND | DMODE_16BITS | DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;
                                
    md_mixfreq = audio_get_frequency();
    if(MikMod_Init(""))
    { 
		char msg[256];
		snprintf(msg, 255, "IErr: %s", MikMod_strerror(MikMod_errno));
		tools_print(msg);	
	}	
    
	numSounds = nSounds;
	numVoices = nVoices;
	
	tools_print("Loading Samples");
	for(int i = 0; i < numSounds; i++)
	{
		sampleList[i] = Sample_Load(soundNameList[i]);
		if(!sampleList[i])
		{
			char msg[256];
			snprintf(msg, 255, "LErr: %s -> %s(%d)", MikMod_strerror(MikMod_errno), soundNameList[i], i);
			tools_print(msg);	
		}
	}
	tools_print("Done Loading Samples");
	
	MikMod_SetNumVoices(1, numVoices);
    MikMod_EnableOutput();
}

 /*
   Function: sound_update

   Only exists to wrap the oddly sounding mikmod command

   TODO:

   - maybe... get rid of it??

*/
void sound_update()
{
	MikMod_Update();
}


 /*
   Function: sound_playSample

   WOULD play a WAVE-sample if the audio-library supported it...

   Parameters:

   - int index - index of the sample

   TODO:

   - update / patch mikmod!
   - allow volume control

*/
void sound_playSample(int index)
{
	/*char msg0[256];
	snprintf(msg0, 255, "Playing: %s", soundNameList[index]);
	tools_print(msg0);	*/
	
	
	char msg2[256];
	snprintf(msg2, 255, "Audio: Freq: %d, Stereo: %d", md_mixfreq, md_mode);
	tools_print(msg2);	
	/*
	voices[index] = */
	int v1 = Sample_Play(sampleList[index], 0, 0);
	Voice_SetVolume(v1, 60);
	char msg[256];
	snprintf(msg, 255, "%d -> PSErr: %s", v1, MikMod_strerror(MikMod_errno));
	tools_print(msg);	
	
	/*char msg[256];
	snprintf(msg, 255, "Voice of %s: %d", soundNameList[index], voices[index]);
	tools_print(msg);	*/
}

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
void sound_playAlone(int index)
{
	/*for(int i = 0; i < numVoices; i++)
		Voice_Stop(voices[i]);
	sound_play(index);*/
	SAMPLE *sfx = Sample_Load("rom://pacman_intro.wav");
	if(sfx == NULL)
	{
		char msg[256];
		snprintf(msg, 255, "LErr: %s", MikMod_strerror(MikMod_errno));
		tools_print(msg);	
	}
	audio_write_silence();
	audio_write_silence();
	SBYTE v2 = Sample_Play(sfx, 0, 0);
	Voice_SetVolume(v2, 255);
}


 /*
   Function: sound_playModule

   Plays module

   Parameters:

   - int index - index of the module

   Details:

   - this actually works :O

*/
void sound_playModule(int index)
{
	MODULE *module;
	module = Player_Load(soundNameList[index], 12, 0);
	tools_print(soundNameList[index]);
	Player_Start(module);
}
