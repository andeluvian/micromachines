#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <utility>
#include <memory>

/*
Author: Vili Karilas
	Class implementation is very similar to mainmenu:

	This class contains 2 maps. One with all sounds loaded into memory (soundbuffers) and another to play them.
	initSound loads a sound in the ../sound/ directory into memory, making it ready to play at any time.
	
*/

// an enum class for different types of sound, making it simpler to manage them
enum class SoundType
{
	ENGINE,		
	TURBO,	
	GUNSHOT,
	ROCKET,
	COLLISION,
	EXPLOSION,
	PICKUP,
	BUTTON,
};

class SoundHandler
{
public:
	SoundHandler();

	void initSound(SoundType, const std::string&); //add a single sound and allocate it to memory

	/// Returns true if the sound exists and is loaded. This could be used for better error handling.
	bool isSound(SoundType);

	/// Returns the sound specified by its filename. (Type: sf::Sound)
	/// The sound can be modified. Use this to set looping sounds (engine, etc.)
	sf::Sound& getSound(SoundType);

	/// Plays the given sound.
	void playSound(SoundType);

	/// Plays the given sound with a varying pitch.
	/// Use small numbers (~0.1) when you want a subtle change.
	/// Use large numbers (1-4) when you want a substantial change.
	void playRandomPitch(SoundType, float);

	/// PLays the given sound as a loop, if it isn't already playing.
	void playAsLoop(SoundType);

	/// Stops a sound.
	void stopSound(SoundType);

	/// Stops all sounds.
	void stopAllSound(SoundType);

	/// Pauses all sounds (doesn't reset the playing position).
	void pauseAllSound(SoundType);

	/// See if the sound is currently being played.
	/// Use this if you have the possibility of multiples of the same sound triggering at the same time.
	bool isPlaying(SoundType);

	/// Simulates the engine revving. Takes a pitch parameter which is relative to current speed of the vehicle.
	/// Lower values of pitch (speed/700) indicate longer gearing, higher values (speed/300) indicate short gearing.
	void revvingSound(SoundType, float);

private:
	std::map< SoundType, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
	std::map< SoundType, sf::Sound> sounds;
};

//NOTE: Since we're only using a few sounds, we don't really need to remove sf::Sound instances when they're not being played.


#endif
