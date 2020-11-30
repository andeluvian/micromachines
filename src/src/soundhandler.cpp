#include "soundhandler.hpp"

SoundHandler::SoundHandler()
{
	// Immediately load everything. This might take a while on slower machines...
	initSound(SoundType::ENGINE, "Dismal_racket.wav");
	initSound(SoundType::TURBO, "Turbo.wav");
	initSound(SoundType::GUNSHOT, "GunShot.wav");
	initSound(SoundType::ROCKET, "Rocket.wav");
	initSound(SoundType::COLLISION, "Bump.wav");
	initSound(SoundType::EXPLOSION, "Explosion.wav");
	initSound(SoundType::PICKUP, "Pickup.wav");
	initSound(SoundType::BUTTON, "Button.wav");

	getSound(SoundType::COLLISION).setVolume(35.0f); //this sounds very loud
}

void SoundHandler::initSound(SoundType ID, const std::string& filename)
{
	std::unique_ptr < sf::SoundBuffer> buffer(new sf::SoundBuffer());
	if (!buffer->loadFromFile("../sound/" + filename))
	{
		if (!buffer->loadFromFile("../../sound/" + filename))
			std::cout << "SoundHandler::initSound - Couldn't find " << filename << std::endl;
	}
	soundBuffers.insert(std::make_pair( ID, std::move(buffer) ));

	sf::Sound sound;
	sound.setBuffer(*soundBuffers[ID]);
	sounds.insert(std::make_pair(ID, sound));
}

///Returns false if sound hasn't been added.
bool SoundHandler::isSound(SoundType ID)
{
	auto found = sounds.find(ID);
	return (found != sounds.end());

}

sf::Sound& SoundHandler::getSound(SoundType ID)
{
	return sounds[ID];
}
	
void SoundHandler::playSound(SoundType ID)
{
	sounds[ID].play();
}

void SoundHandler::playRandomPitch(SoundType ID, float modifier)
{
	//Getting truly numbers here isn't critical, so rand() will suffice.
	//The formula below will give a random float between 0 and 1.
	sounds[ID].setPitch(1 + modifier * ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)))); 
	sounds[ID].play();
}

void SoundHandler::playAsLoop(SoundType ID)
{
	sounds[ID].setLoop(true);
	sounds[ID].play();
}

void SoundHandler::stopSound(SoundType ID)
{
	sounds[ID].stop();
}

void SoundHandler::stopAllSound(SoundType ID)
{
	for (auto i : sounds)
		i.second.stop();
}

void SoundHandler::pauseAllSound(SoundType ID)
{
	for (auto i : sounds)
		i.second.pause();
}

bool SoundHandler::isPlaying(SoundType ID)
{
	return (sounds[ID].getStatus() == sf::Sound::Status::Playing);
}

void SoundHandler::revvingSound(SoundType ID, float pitch)
{
	while (pitch > 1)
	{
		pitch -= 0.6f;
	}
	sounds[ID].setPitch(1 + pitch);
}

