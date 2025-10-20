#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Audio.h"
#include "Console.h"
#include "VFS.h"
#include "../Game.h"

SoLoud::Soloud Audio::system;
std::vector<Audio*> Audio::playing;

bool Audio::Enabled;
float Audio::MusicVolume, Audio::SoundVolume;
#ifdef BECKETT_MOREVOLUME
float Audio::AmbientVolume, Audio::SpeechVolume;
#endif

static auto epsilon = glm::epsilon<float>();

void Audio::Initialize()
{
	Enabled = true;
	if (system.init() != 0)
	{
		conprint(1, "Could not initialize SoLoud. Sound disabled.");
		Enabled = false;
		return;
	}
}

void Audio::Update()
{
	static auto oldMusicVolume = MusicVolume;
	auto changed = false;

	if (std::fabs(oldMusicVolume - MusicVolume) > epsilon)
	{
		oldMusicVolume = MusicVolume;
		changed = true;
	}

	for (auto x : playing)
	{
		x->update();
		if (changed)
		{
			x->UpdateVolume();
		}
	}
}

Audio::Audio(const std::string& filename) : filename(filename)
{
	size_t size = 0;
	if (!Enabled)
	{
		status = Status::Invalid;
		return;
	}
	data = VFS::ReadData(filename, &size);
	if (!data)
	{
		conprint(1, "Could not open audio file {}.", filename);
		return;
	}
	if (filename.find("music/") != std::string::npos)
		type = Type::Music;
#ifdef BECKETT_MOREVOLUME
	else if (filename.find("ambient/") != std::string::npos)
		type = Type::Ambient;
	else if (filename.find("speech/") != std::string::npos)
		type = Type::Speech;
#endif
	else
		type = Type::Sound;

	if (theSound.loadMem((unsigned char*)data.get(), (unsigned int)size, true) != 0)
	{
		fmt::format("Could not create stream for audio file {}.", filename);
		return;
	}

	auto maybeTagFile = VFS::ChangeExtension(filename, "txt");
	auto maybeTags = VFS::ReadString(maybeTagFile);
	if (!maybeTags.empty())
	{
		//parse Audacity tag file
		ReplaceAll(maybeTags, "\r", "");
		auto lines = Split(maybeTags, '\n');
		for (auto& line : lines)
		{
			auto parts = Split(line, '\t');
			auto time = std::stof(parts[0]);
			auto text = parts[2];
			tags.push_back(std::make_tuple(time, text));
		}
		nextTag = std::get<0>(tags[0]);
	}
	
	status = Status::Stopped;
}

Audio::~Audio()
{
	Stop();
}

void Audio::Play(bool force)
{
	if (force && status != Status::Stopped)
		Stop();

	if (status == Status::Stopped)
	{
		if (Enabled)
		{
			theHandle = system.play(theSound);
			UpdateVolume();
		}
		playing.push_back(this);
	}
	else if (status == Status::Paused)
	{
		system.setPause(theHandle, false);
	}
	frequency = system.getSamplerate(theHandle);
	status = Status::Playing;
}

void Audio::Pause()
{
	if (Enabled)
		system.setPause(theHandle, true);
	status = Status::Paused;
}

void Audio::Stop()
{
	if (status != Status::Stopped)
	{
		if (Enabled)
			system.stop(theHandle);
	}
	status = Status::Stopped;
	playing.erase(std::remove(playing.begin(), playing.end(), this), playing.end());
}

void Audio::update()
{
	if (tags.empty())
		return;
	if (listeners.empty())
		return;
	if (currentTag >= tags.size())
		return;
	float fpos = (float)system.getStreamPosition(theHandle);
	
	//adjust a bit, not sure if my fault or FMOD's.
	//fpos -= 0.100f; if (fpos < 0.0f) fpos = 0.0f;

	while (fpos > nextTag)
	{
		for (auto listener : listeners)
		{
			listener->AudioEvent(lastTag, std::get<1>(tags[currentTag]));
		}
		currentTag++;
		if (currentTag >= tags.size())
			return;
		lastTag = nextTag;
		nextTag = std::get<0>(tags[currentTag]);
	}
}

void Audio::UpdateVolume()
{
	auto v = 0.0f;
	switch (type)
	{
	case Type::Music: v = MusicVolume; break;
	case Type::Sound: v = SoundVolume; break;
#ifdef BECKETT_MOREVOLUME
	case Type::Ambient: v = AmbientVolume; break;
	case Type::Speech: v = SpeechVolume; break;
#endif
	}
	Volume = glm::clamp(Volume, 0.0f, 1.0f);
	system.setVolume(theHandle, v * Volume);
}

void Audio::SetPitch(float ratio)
{
	assert(ratio != 0.0f);
	system.setRelativePlaySpeed(theHandle, ratio);
}

#ifdef BECKETT_3DAUDIO
void Audio::SetPosition(glm::vec3 pos)
{
	//Only generic sounds can be positioned.
	if (type != Type::Sound)
		return;
	//FMOD_VECTOR v = { pos.x, pos.y, pos.z };
	//theChannel->set3DAttributes(&v, nullptr);
}
#endif

void Audio::SetPan(float pos)
{
	system.setPan(theHandle, glm::clamp(pos, -1.0f, 1.0f));
}

void Audio::RegisterListener(const AudioEventListener* listener)
{
	if (std::find(listeners.cbegin(), listeners.cend(), listener) != listeners.cend())
		return;
	listeners.push_back(const_cast<AudioEventListener*>(listener));
}

void Audio::UnregisterLister(const AudioEventListener* listener)
{
	auto it = std::find_if(listeners.begin(), listeners.end(), [listener](auto e)
	{
		return e == listener;
	});
	if (it != listeners.end())
		listeners.erase(it);
}
