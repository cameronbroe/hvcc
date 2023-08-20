{{copyright}}

#include <juce_audio_processors/juce_audio_processors.h>
#include "Heavy_{{name}}.h"
#include "Heavy_{{name}}.hpp"

class HeavyJuceParameterFloat: public juce::AudioParameterFloat {
public:
    HeavyJuceParameterFloat(
        const juce::ParameterID& parameterID,
        const juce::String& parameterName,
        float minValue,
        float maxValue,
        float defaultValue,
        HeavyContextInterface *heavyContext
    );
    virtual void valueChanged(float newValue);
private:
    HeavyContextInterface *_context;
};