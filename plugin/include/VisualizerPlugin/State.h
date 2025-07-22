#ifndef STATE_H
#define STATE_H

#include <vector>
#include <juce_audio_processors/juce_audio_processors.h>

enum class Band
{
    Bass,
    Mid,
    Treble
};

class State {
public:
  static State& getInstance()
  {
    if (nullptr == instance) instance = new State;
    return *instance;
  }

  State(const State&) = delete; 
  State& operator=(const State&) = delete;
  static void destruct() {
    delete instance;
    instance = nullptr;
  }
  
  int getFrameCounter() const {return currentFrame;}
  float getMeanBandVolume(const Band& bandType) const {if (bandType == Band::Bass) {return meanBandVolumes[static_cast<int>(bandType)];}
                                                       else if (bandType == Band::Mid) {return meanBandVolumes[static_cast<int>(bandType)];}
                                                       else if (bandType == Band::Treble) {return meanBandVolumes[static_cast<int>(bandType)];}}

  void setCurrentFrame(const int& frame) {currentFrame = frame;}
  void setmeanBandVolume(const Band& bandType, const float& newValue) {if (bandType == Band::Bass) {meanBandVolumes[static_cast<int>(bandType)] = newValue;}
                                                                     else if (bandType == Band::Mid) {meanBandVolumes[static_cast<int>(bandType)] = newValue;}
                                                                     else if (bandType == Band::Treble) {meanBandVolumes[static_cast<int>(bandType)] = newValue;}}

  void setMeanBandVolume(const Band& bandType, const juce::AudioBuffer<float>& buffer)
  {
    float sum = 0;
    for (int c = 0; c < buffer.getNumChannels(); c++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            sum += abs(buffer.getSample(c, i));
        }
    }
    meanBandVolumes[static_cast<int>(bandType)] = sum/(256.0f * buffer.getNumChannels());
  }
    
  void incrementCurrentFrame() {currentFrame++;}
private:
  State() = default; 
  ~State() = default; 
  static State* instance; 
  int currentFrame = 0;

  std::vector<float> meanBandVolumes {0.0f, 0.0f, 0.0f}; //meanBassVolume, meanMidVolume, meanTrebleVolume
};

#endif