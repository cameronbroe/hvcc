{{copyright}}
#include "HeavyJuceProcessor_{{name}}.h"

//==============================================================================
{{class_name}}::{{class_name}}()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    _context = hv_{{name}}_new_with_options(44100.0, {{pool_sizes_kb.internal}}, {{pool_sizes_kb.inputQueue}}, {{pool_sizes_kb.outputQueue}});
    _context->setUserData(this);

{%- for k, v in receivers %}
    addParameter({{v.display}} = new HeavyJuceParameterFloat("{{v.display}}_{{v.ids[0]}}",
                                                             "{{v.display}}",
                                                             {{v.attributes.min}}f,
                                                             {{v.attributes.max}}f,
                                                             {{v.attributes.default}}f,
                                                             _context));
    {{v.display}}->valueChanged({{v.attributes.default}}f);
{% endfor -%}
}

{{class_name}}::~{{class_name}}()
{
}

//==============================================================================
const juce::String {{class_name}}::getName() const
{
    return "{{name}}";
}

bool {{class_name}}::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool {{class_name}}::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool {{class_name}}::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double {{class_name}}::getTailLengthSeconds() const
{
    return 0.0;
}

int {{class_name}}::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int {{class_name}}::getCurrentProgram()
{
    return 0;
}

void {{class_name}}::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String {{class_name}}::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void {{class_name}}::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void {{class_name}}::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void {{class_name}}::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool {{class_name}}::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void {{class_name}}::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    if(lastSampleRate != getSampleRate()) {
        handleChangedSampleRate(getSampleRate());
    }

    /*
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ..do something to the data...
    }
    */
    _context->process(
        (float**)buffer.getArrayOfReadPointers(), 
        (float**)buffer.getArrayOfWritePointers(),
        buffer.getNumSamples());
}

//==============================================================================
bool {{class_name}}::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* {{class_name}}::createEditor()
{
    return nullptr;
}

//==============================================================================
void {{class_name}}::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void {{class_name}}::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

void {{class_name}}::handleChangedSampleRate (double newSampleRate)
{

    hv_{{name}}_free(_context);

    _context = hv_{{name}}_new_with_options(getSampleRate(), {{pool_sizes_kb.internal}}, {{pool_sizes_kb.inputQueue}}, {{pool_sizes_kb.outputQueue}});
    _context->setUserData(this);

    {% if receivers|length > 0 -%}
    {% for k, v in receivers -%}
    {{v.display}}->valueChanged({{v.display}}->get());
    {% endfor -%}
    {%- endif %}

    lastSampleRate = newSampleRate;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new {{class_name}}();
}