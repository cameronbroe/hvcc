{{copyright}}

#include "HeavyJuceParameter.h"

HeavyJuceParameterFloat::HeavyJuceParameterFloat(
        const juce::ParameterID& parameterID,
        const juce::String& parameterName,
        float minValue,
        float maxValue,
        float defaultValue,
        HeavyContextInterface *heavyContext
) : juce::AudioParameterFloat(
    parameterID,
    parameterName,
    minValue,
    maxValue,
    defaultValue
), _context(heavyContext) {}


void HeavyJuceParameterFloat::valueChanged(float newValue) {
    {%- if receivers|length > 0 %}
    auto index = this->getParameterIndex();
    switch(index) {
        {% for k, v in receivers -%}
        case {{loop.index-1}}: {
            _context->sendFloatToReceiver(
                Heavy_{{name}}::Parameter::In::{{k|upper}},
                newValue
            );
            break;
        }
        {% endfor %}
        default: return;
    }
    {%- endif %}
}