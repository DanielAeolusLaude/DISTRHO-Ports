/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#include "includes.h"
#include "TalComponent.h"
#include "ProgramChunk.h"

/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TalCore();
}

TalCore::TalCore()
{
	// init engine
	if (this->getSampleRate() > 0)
		sampleRate = (float)this->getSampleRate();
	else
		sampleRate = 44100.0f;

	engine = new ReverbEngine(sampleRate);
	params = engine->param;
	talPresets = new TalPreset*[NUMPROGRAMS];

	for (int i = 0; i < NUMPROGRAMS; i++) talPresets[i] = new TalPreset();
	curProgram = 0;

	// load factory presets
	ProgramChunk chunk;
	setStateInformationString(chunk.getXmlChunk());
	setCurrentProgram(curProgram);
}

TalCore::~TalCore()
{
	if (talPresets) delete[] talPresets;
	if (engine) delete engine;
}

const String TalCore::getName() const
{
    return "Tal-Reverb-II";
}

int TalCore::getNumParameters()
{
	return (int)NUMPARAM;
}

float TalCore::getParameter (int index)
{
	if (index < NUMPARAM)
		return talPresets[curProgram]->programData[index];
	else
		return 0;
}

void TalCore::setParameter (int index, float newValue)
{
	if (index < NUMPARAM)
	{
		params[index] = newValue;
		talPresets[curProgram]->programData[index] = newValue;

		switch(index)
		{
			case WET:
				engine->setWet(newValue);
				break;
			case DRY:
				engine->setDry(newValue);
				break;
			case DECAYTIME:
				engine->setDecayTime(newValue);
				break;
			case PREDELAY:
				engine->setPreDelay(newValue);
				break;
			case STEREO:
				engine->setStereoWidth(newValue);
				break;
			case REALSTEREOMODE:
				engine->setStereoMode(newValue);
				break;

			case LOWSHELFFREQUENCY:
				engine->setLowShelfFrequency(newValue);
				break;
			case HIGHSHELFFREQUENCY:
				engine->setHighShelfFrequency(newValue);
				break;
			case PEAKFREQUENCY:
				engine->setPeakFrequency(newValue);
				break;

			case LOWSHELFGAIN:
				engine->setLowShelfGain(newValue);
				break;
			case HIGHSHELFGAIN:
				engine->setHighShelfGain(newValue);
				break;
			case PEAKGAIN:
				engine->setPeakGain(newValue);
				break;
		}

		sendChangeMessage ();
	}
}

const String TalCore::getParameterName (int index)
{
	switch(index)
	{
		case WET: return T("wet");
		case DRY: return T("dry");
		case DECAYTIME: return T("room size");
		case PREDELAY: return T("pre delay");

		case LOWSHELFFREQUENCY: return T("low shelf frequency");
		case HIGHSHELFFREQUENCY: return T("high shelf frequency");
		case PEAKFREQUENCY: return T("peak frequency");

		case LOWSHELFGAIN: return T("low shelf gain");
		case HIGHSHELFGAIN: return T("high shelf gain");
		case PEAKGAIN: return T("peak gain");

		case STEREO: return T("stereo");
		case REALSTEREOMODE: return T("stereo input");

		case UNUSED: return "unused";
	}
    return String::empty;
}

const String TalCore::getParameterText (int index)
{
	if (index < NUMPARAM)
	{
		return String(talPresets[curProgram]->programData[index], 2);
	}
    return String::empty;
}

const String TalCore::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String TalCore::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool TalCore::isInputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::acceptsMidi() const
{
    return false;
}

bool TalCore::producesMidi() const
{
    return false;
}

void TalCore::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void TalCore::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TalCore::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	// Change sample rate if required
	if (sampleRate != this->getSampleRate())
	{
		sampleRate = this->getSampleRate();
		engine->setSampleRate(sampleRate);
		setCurrentProgram(curProgram);
	}
    const ScopedLock sl (this->getCallbackLock());

    // for each of our input channels, we'll attenuate its level by the
    // amount that our volume parameter is set to.
	int numberOfChannels = getNumInputChannels();
	//int bufferSize = buffer.getNumSamples();

	if (numberOfChannels == 2)
	{
		float *samples0 = buffer.getWritePointer(0, 0);
		float *samples1 = buffer.getWritePointer(1, 0);

		int samplePos = 0;
		int numSamples = buffer.getNumSamples();
		while (numSamples > 0)
		{
			engine->process(samples0++, samples1++);

			numSamples--;
			samplePos++;
		}
	}
	if (numberOfChannels == 1)
	{
		float *samples0 = buffer.getWritePointer(0, 0);
		float *samples1 = buffer.getWritePointer(0, 0);

		int samplePos = 0;
		int numSamples = buffer.getNumSamples();
		while (numSamples > 0)
		{
			engine->process(samples0++, samples1++);

			numSamples--;
			samplePos++;
		}
	}
    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

AudioProcessorEditor* TalCore::createEditor()
{
    return new TalComponent (this);
}

void TalCore::getStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

	// header
	XmlElement tal("tal");
	tal.setAttribute (T("curprogram"), curProgram);
	tal.setAttribute (T("version"), 1);

	// programs
    XmlElement *programList = new XmlElement ("programs");
	for (int i = 0; i < NUMPROGRAMS; i++)
	{
		XmlElement* program = new XmlElement ("program");
		program->setAttribute (T("programname"), talPresets[i]->name);
		program->setAttribute (T("dry"), talPresets[i]->programData[DRY]);
		program->setAttribute (T("wet"), talPresets[i]->programData[WET]);
		program->setAttribute (T("roomsize"), talPresets[i]->programData[DECAYTIME]);
		program->setAttribute (T("predelay"), talPresets[i]->programData[PREDELAY]);

		program->setAttribute (T("lowshelffrequency"), talPresets[i]->programData[LOWSHELFFREQUENCY]);
		program->setAttribute (T("highshelffrequency"), talPresets[i]->programData[HIGHSHELFFREQUENCY]);
		program->setAttribute (T("peakfrequency"), talPresets[i]->programData[PEAKFREQUENCY]);

		program->setAttribute (T("lowshelfgain"), talPresets[i]->programData[LOWSHELFGAIN]);
		program->setAttribute (T("highshelfgain"), talPresets[i]->programData[HIGHSHELFGAIN]);
		program->setAttribute (T("peakgain"), talPresets[i]->programData[PEAKGAIN]);

		program->setAttribute (T("stereowidth"), talPresets[i]->programData[STEREO]);
		program->setAttribute (T("realstereomode"), talPresets[i]->programData[REALSTEREOMODE]);
		programList->addChildElement(program);
	}
	tal.addChildElement(programList);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (tal, destData);
}

void TalCore::setStateInformation (const void* data, int sizeInBytes)
{
	// use this helper function to get the XML from this binary blob..
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

	curProgram = 0;
	if (xmlState != 0 && xmlState->hasTagName(T("tal")))
	{
		curProgram = (int)xmlState->getIntAttribute (T("curprogram"), 1);
		XmlElement* programs = xmlState->getFirstChildElement();
		if (programs->hasTagName(T("programs")))
		{
			int i = 0;
			forEachXmlChildElement (*programs, e)
			{
				if (e->hasTagName(T("program")) && i < NUMPROGRAMS)
				{
					talPresets[i]->name = e->getStringAttribute (T("programname"), T("Not Saved"));
					talPresets[i]->programData[DRY] = (float) e->getDoubleAttribute (T("dry"), 0.8f);
					talPresets[i]->programData[WET] = (float) e->getDoubleAttribute (T("wet"), 0.8f);
					talPresets[i]->programData[DECAYTIME] = (float) e->getDoubleAttribute (T("roomsize"), 0.8f);
					talPresets[i]->programData[PREDELAY] = (float) e->getDoubleAttribute (T("predelay"), 0.0f);

					talPresets[i]->programData[LOWSHELFFREQUENCY] = (float) e->getDoubleAttribute (T("lowshelffrequency"), 0.5f);
					talPresets[i]->programData[HIGHSHELFFREQUENCY] = (float) e->getDoubleAttribute (T("highshelffrequency"), 0.5f);
					talPresets[i]->programData[PEAKFREQUENCY] = (float) e->getDoubleAttribute (T("peakfrequency"), 0.5f);

					talPresets[i]->programData[LOWSHELFGAIN] = (float) e->getDoubleAttribute (T("lowshelfgain"), 1.0f);
					talPresets[i]->programData[HIGHSHELFGAIN] = (float) e->getDoubleAttribute (T("highshelfgain"), 1.0f);
					talPresets[i]->programData[PEAKGAIN] = (float) e->getDoubleAttribute (T("peakgain"), 1.0f);

					talPresets[i]->programData[STEREO] = (float) e->getDoubleAttribute (T("stereowidth"), 1.0f);
					talPresets[i]->programData[REALSTEREOMODE] = (float) e->getDoubleAttribute (T("realstereomode"), 0.0f);
					i++;
				}
			}
		}
		delete xmlState;
		setCurrentProgram(curProgram);
		sendChangeMessage ();
	}
}

void TalCore::setStateInformationString (const String& data)
{
    XmlElement* const xmlState = XmlDocument::parse(data);

    curProgram = 0;
    if (xmlState != 0 && xmlState->hasTagName(T("tal")))
    {
            curProgram = (int)xmlState->getIntAttribute (T("curprogram"), 1);
            XmlElement* programs = xmlState->getFirstChildElement();
            if (programs->hasTagName(T("programs")))
            {
                    int i = 0;
                    forEachXmlChildElement (*programs, e)
                    {
                            if (e->hasTagName(T("program")) && i < NUMPROGRAMS)
                            {
                                    talPresets[i]->name = e->getStringAttribute (T("programname"), T("Not Saved"));
                                    talPresets[i]->programData[DRY] = (float) e->getDoubleAttribute (T("dry"), 0.8f);
                                    talPresets[i]->programData[WET] = (float) e->getDoubleAttribute (T("wet"), 0.8f);
                                    talPresets[i]->programData[DECAYTIME] = (float) e->getDoubleAttribute (T("roomsize"), 0.8f);
                                    talPresets[i]->programData[PREDELAY] = (float) e->getDoubleAttribute (T("predelay"), 0.0f);

                                    talPresets[i]->programData[LOWSHELFFREQUENCY] = (float) e->getDoubleAttribute (T("lowshelffrequency"), 0.5f);
                                    talPresets[i]->programData[HIGHSHELFFREQUENCY] = (float) e->getDoubleAttribute (T("highshelffrequency"), 0.5f);
                                    talPresets[i]->programData[PEAKFREQUENCY] = (float) e->getDoubleAttribute (T("peakfrequency"), 0.5f);

                                    talPresets[i]->programData[LOWSHELFGAIN] = (float) e->getDoubleAttribute (T("lowshelfgain"), 1.0f);
                                    talPresets[i]->programData[HIGHSHELFGAIN] = (float) e->getDoubleAttribute (T("highshelfgain"), 1.0f);
                                    talPresets[i]->programData[PEAKGAIN] = (float) e->getDoubleAttribute (T("peakgain"), 1.0f);

                                    talPresets[i]->programData[STEREO] = (float) e->getDoubleAttribute (T("stereowidth"), 1.0f);
                                    talPresets[i]->programData[REALSTEREOMODE] = (float) e->getDoubleAttribute (T("realstereomode"), 0.0f);
                                    i++;
                            }
                    }
            }
            delete xmlState;
            setCurrentProgram(curProgram);
            sendChangeMessage ();
    }
}

String TalCore::getStateInformationString ()
{
    // header
    XmlElement tal("tal");
    tal.setAttribute (T("curprogram"), curProgram);
    tal.setAttribute (T("version"), 1);

    // programs
    XmlElement *programList = new XmlElement ("programs");
    for (int i = 0; i < NUMPROGRAMS; i++)
    {
            XmlElement* program = new XmlElement ("program");
            program->setAttribute (T("programname"), talPresets[i]->name);
            program->setAttribute (T("dry"), talPresets[i]->programData[DRY]);
            program->setAttribute (T("wet"), talPresets[i]->programData[WET]);
            program->setAttribute (T("roomsize"), talPresets[i]->programData[DECAYTIME]);
            program->setAttribute (T("predelay"), talPresets[i]->programData[PREDELAY]);

            program->setAttribute (T("lowshelffrequency"), talPresets[i]->programData[LOWSHELFFREQUENCY]);
            program->setAttribute (T("highshelffrequency"), talPresets[i]->programData[HIGHSHELFFREQUENCY]);
            program->setAttribute (T("peakfrequency"), talPresets[i]->programData[PEAKFREQUENCY]);

            program->setAttribute (T("lowshelfgain"), talPresets[i]->programData[LOWSHELFGAIN]);
            program->setAttribute (T("highshelfgain"), talPresets[i]->programData[HIGHSHELFGAIN]);
            program->setAttribute (T("peakgain"), talPresets[i]->programData[PEAKGAIN]);

            program->setAttribute (T("stereowidth"), talPresets[i]->programData[STEREO]);
            program->setAttribute (T("realstereomode"), talPresets[i]->programData[REALSTEREOMODE]);
            programList->addChildElement(program);
    }
    tal.addChildElement(programList);

    return tal.createDocument (String::empty);
}

int TalCore::getNumPrograms ()
{
	return NUMPROGRAMS;
}

int TalCore::getCurrentProgram ()
{
	return curProgram;
}

void TalCore::setCurrentProgram (int index)
{
	if (index < NUMPROGRAMS)
	{
		curProgram = index;
		for (int i = 0; i < NUMPARAM; i++)
		{
			setParameter(i, talPresets[index]->programData[i]);
		}
		sendChangeMessage ();
	}
}

const String TalCore::getProgramName (int index)
{
	if (index < NUMPROGRAMS)
		return talPresets[index]->name;
	else
		return T("Invalid");
}

void TalCore::changeProgramName (int index, const String& newName)
{
	if (index < NUMPROGRAMS)
		talPresets[index]->name = newName;
}
