#include "Synthesis.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "IKeyboardControl.h"
#include "resource.h"

const int kNumPrograms = 5;

enum EAdsr
{
	E_Att = 0,
	E_Dec,
	E_Sus,
	E_Rel,
};

enum EParams
{
	mOsc1Waveform = 0,
	mOsc1Coarse,
	mOsc1Fine,
	mOsc2Waveform,
	mOsc2Coarse,
	mOsc2Fine,
	mOscillatorMix,
	mPhaseStart,
	mBitCrusher,
	mTejiBooster,
	mAmpAmount,
	mAttack,
	mDecay,
	mSustain,
	mRelease,
	mFilterMode,
	mFilterCutoff,
	mFilterResonance,
	mFilterAttack,
	mFilterDecay,
	mFilterSustain,
	mFilterRelease,
	mFilterEnvelopeAmount,
	mVerbRoomSize,
	mVerbDamp,
	mVerbWidth,
	mVerbDry,
	mVerbWet,
	mVerbOn,
	kNumParams
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,
	kKeybX = 12,
	kKeybY = 297,
	kGreenRow = 10,
	kOrangeRow = 80,
	kBlueRow = 150,
	kPurpleRow = 220,
	kSwitcherTopPadding = 10,
};

Synthesis::Synthesis(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
	lastVirtualKeyboardNoteNumber(virtualKeyboardMinimumNoteNumber - 1) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	IBitmap pianoKeyImage = pGraphics->LoadIBitmap(PIANO_KEY_ID, PIANO_KEY_FN);

	//                            C#     D#          F#      G#      A#
	int keyCoordinates[12] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
	mVirtualKeyboard = new IKeyboardControl(this, kKeybX, kKeybY, virtualKeyboardMinimumNoteNumber, /* octaves: */ 10, &pianoKeyImage, &pianoKeyImage, keyCoordinates);

	pGraphics->AttachControl(mVirtualKeyboard);

	// Bitmaps
	IBitmap greenKnobBitmap = pGraphics->LoadIBitmap(GREEN_KNOB_ID, GREEN_KNOB_FN, 31);
	IBitmap greenKnobCenterBitmap = pGraphics->LoadIBitmap(GREEN_KNOB_CENTER_ID, GREEN_KNOB_CENTER_FN, 31);
	IBitmap blueKnobBitmap = pGraphics->LoadIBitmap(BLUE_KNOB_ID, BLUE_KNOB_FN, 31);
	IBitmap blueKnobCenterBitmap = pGraphics->LoadIBitmap(BLUE_KNOB_CENTER_ID, BLUE_KNOB_CENTER_FN, 31);
	IBitmap orangeKnobBitmap = pGraphics->LoadIBitmap(ORANGE_KNOB_ID, ORANGE_KNOB_FN, 31);
	IBitmap purpleKnobBitmap = pGraphics->LoadIBitmap(PURPLE_KNOB_ID, PURPLE_KNOB_FN, 31);
	IBitmap waveformBitmap = pGraphics->LoadIBitmap(WAVEFORM_ID, WAVEFORM_FN, 5);
	IBitmap filtermodeBitmap = pGraphics->LoadIBitmap(FILTERMODE_ID, FILTERMODE_FN, 3);
	IBitmap switcherLightBitmap = pGraphics->LoadIBitmap(SWITCHER_LIGHT_ID, SWITCHER_LIGHT_FN, 2);
	IBitmap switcherOnOffBitmap = pGraphics->LoadIBitmap(SWITCHER_ONOFF_ID, SWITCHER_ONOFF_FN, 2);

	// OSC1 Waveform switch
	GetParam(mOsc1Waveform)->InitEnum("Waveform1", Oscillator::OSCILLATOR_MODE_SINE, Oscillator::kNumOscillatorModes);
	GetParam(mOsc1Waveform)->SetDisplayText(0, "Sine"); 
	pGraphics->AttachControl(new ISwitchControl(this, 42, kGreenRow + kSwitcherTopPadding, mOsc1Waveform, &waveformBitmap));

	// OSC1 Coarse knob:
	GetParam(mOsc1Coarse)->InitInt("Coarse1", 0, -24, 24);
	GetParam(mOsc1Coarse)->SetShape(1);
	pGraphics->AttachControl(new IKnobMultiControl(this, 136, kGreenRow, mOsc1Coarse, &greenKnobCenterBitmap));

	// OSC1 Fine knob:
	GetParam(mOsc1Fine)->InitInt("Fine1", 0, -100, 100);
	GetParam(mOsc1Fine)->SetShape(1);
	pGraphics->AttachControl(new IKnobMultiControl(this, 195, kGreenRow, mOsc1Fine, &greenKnobCenterBitmap));

	// OSC2 Waveform switch
	GetParam(mOsc2Waveform)->InitEnum("Waveform2", Oscillator::OSCILLATOR_MODE_SINE, Oscillator::kNumOscillatorModes);
	GetParam(mOsc2Waveform)->SetDisplayText(0, "Sine"); 
	pGraphics->AttachControl(new ISwitchControl(this, 346, kGreenRow + kSwitcherTopPadding, mOsc2Waveform, &waveformBitmap));

	// OSC2 Coarse knob:
	GetParam(mOsc2Coarse)->InitInt("Coarse2", 0, -24, 24);
	GetParam(mOsc2Coarse)->SetShape(1);
	pGraphics->AttachControl(new IKnobMultiControl(this, 437, kGreenRow, mOsc2Coarse, &greenKnobCenterBitmap));

	// OSC2 Fine knob:
	GetParam(mOsc2Fine)->InitInt("Fine2", 0, -100, 100);
	GetParam(mOsc2Fine)->SetShape(1);
	pGraphics->AttachControl(new IKnobMultiControl(this, 491, kGreenRow, mOsc2Fine, &greenKnobCenterBitmap));
	
	// mOscillatorMix
	GetParam(mOscillatorMix)->InitDouble("Osc Mix", 0.0, 0.0, 1.0, 0.001);
	GetParam(mOscillatorMix)->SetShape(1);
	pGraphics->AttachControl(new IKnobMultiControl(this, 275, kGreenRow, mOscillatorMix, &greenKnobCenterBitmap));

	// OSC Phase Start switch
	GetParam(mPhaseStart)->InitEnum("Start", 1, 2);
	GetParam(mPhaseStart)->SetDisplayText(0, "Start");
	pGraphics->AttachControl(new ISwitchControl(this, 564, kGreenRow + 7, mPhaseStart, &switcherLightBitmap));

	// OSC1 Waveform switch
	GetParam(mBitCrusher)->InitEnum("Bit Crusher", 0, 2);
	GetParam(mBitCrusher)->SetDisplayText(0, "Bit Crusher");
	pGraphics->AttachControl(new ISwitchControl(this, 564, kGreenRow + 25, mBitCrusher, &switcherLightBitmap));

	// OSC1 Waveform switch
	GetParam(mTejiBooster)->InitEnum("Mono/Poly", 0, 2);
	GetParam(mTejiBooster)->SetDisplayText(0, "Teji Booster");
	pGraphics->AttachControl(new ISwitchControl(this, 564, kGreenRow + 43 + 1, mTejiBooster, &switcherLightBitmap));

	// Amp amount knob:
	GetParam(mAmpAmount)->InitDouble("Amp Amount", 50., 0., 100.0, 0.01, "%");
	GetParam(mAmpAmount)->SetShape(2);
	pGraphics->AttachControl(new IKnobMultiControl(this, 255, kOrangeRow, mAmpAmount, &orangeKnobBitmap));

	// Attack knob:
	ampAdsrKnobs[E_Att] = new IKnobMultiControl(this, 329, kOrangeRow, mAttack, &orangeKnobBitmap);
	ampAdsrKnobs[E_Att]->SetTooltip("Amp Attach Envelope"); // Seems doesn't work. tested in savihost, fl studio and ableton live 9.
	GetParam(mAttack)->InitDouble("Attack", 0.01, 0.01, 10.0, 0.001);
	GetParam(mAttack)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Att]);
	// Decay knob:
	ampAdsrKnobs[E_Dec] = new IKnobMultiControl(this, 383, kOrangeRow, mDecay, &orangeKnobBitmap);
	GetParam(mDecay)->InitDouble("Decay", 0.5, 0.01, 15.0, 0.001);
	GetParam(mDecay)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Dec]);
	// Sustain knob:
	ampAdsrKnobs[E_Sus] = new IKnobMultiControl(this, 437, kOrangeRow, mSustain, &orangeKnobBitmap);
	GetParam(mSustain)->InitDouble("Sustain", 0.1, 0.001, 1.0, 0.001);
	GetParam(mSustain)->SetShape(2);
	pGraphics->AttachControl(ampAdsrKnobs[E_Sus]);
	// Release knob:
	ampAdsrKnobs[E_Rel] = new IKnobMultiControl(this, 491, kOrangeRow, mRelease, &orangeKnobBitmap);
	GetParam(mRelease)->InitDouble("Release", 1.0, 0.001, 15.0, 0.001);
	GetParam(mRelease)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Rel]);

	// ADSR Visualization
	ampAdsrVisualization = new ADSRVisualizationControl(this, IRECT(546, kOrangeRow + 7, 648, kOrangeRow + 55));
	ampAdsrVisualization->setColor(IColor(100, 235, 124, 6));
	pGraphics->AttachControl(ampAdsrVisualization);

	// Filter switch
	GetParam(mFilterMode)->InitEnum("Filter Mode", Filter::FILTER_MODE_LOWPASS, Filter::kNumFilterModes);
	GetParam(mFilterMode)->SetDisplayText(0, "LP"); // Needed for VST3, thanks plunntic
	pGraphics->AttachControl(new ISwitchControl(this, 42, kBlueRow + kSwitcherTopPadding, mFilterMode, &filtermodeBitmap));

	// Filter options
	// Cutoff knob:
	GetParam(mFilterCutoff)->InitDouble("Cutoff", 0.99, 0.01, 0.99, 0.001);
	GetParam(mFilterCutoff)->SetShape(2);
	pGraphics->AttachControl(new IKnobMultiControl(this, 137, kBlueRow, mFilterCutoff, &blueKnobBitmap));
	// Resonance knob:
	GetParam(mFilterResonance)->InitDouble("Resonance", 0.01, 0.01, 1.0, 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 195, kBlueRow, mFilterResonance, &blueKnobBitmap));
	// Filter envelope amount knob:
	GetParam(mFilterEnvelopeAmount)->InitDouble("Filter Env Amount", 0.0, -1.0, 1.0, 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 255, kBlueRow, mFilterEnvelopeAmount, &blueKnobCenterBitmap));

	// Attack knob:
	filterAdsrKnobs[E_Att] = new IKnobMultiControl(this, 329, kBlueRow, mFilterAttack, &blueKnobBitmap);
	GetParam(mFilterAttack)->InitDouble("Filter Env Attack", 0.01, 0.01, 10.0, 0.001);
	GetParam(mFilterAttack)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Att]);
	// Decay knob:
	filterAdsrKnobs[E_Dec] = new IKnobMultiControl(this, 383, kBlueRow, mFilterDecay, &blueKnobBitmap);
	GetParam(mFilterDecay)->InitDouble("Filter Env Decay", 0.5, 0.01, 15.0, 0.001);
	GetParam(mFilterDecay)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Dec]);
	// Sustain knob:
	filterAdsrKnobs[E_Sus] = new IKnobMultiControl(this, 437, kBlueRow, mFilterSustain, &blueKnobBitmap);
	GetParam(mFilterSustain)->InitDouble("Filter Env Sustain", 0.1, 0.001, 1.0, 0.001);
	GetParam(mFilterSustain)->SetShape(2);
	pGraphics->AttachControl(filterAdsrKnobs[E_Sus]);
	// Release knob:
	filterAdsrKnobs[E_Rel] = new IKnobMultiControl(this, 491, kBlueRow, mFilterRelease, &blueKnobBitmap);
	GetParam(mFilterRelease)->InitDouble("Filter Env Release", 1.0, 0.001, 15.0, 0.001);
	GetParam(mFilterRelease)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Rel]);

	// Filter ADSR Visualization
	filterEnvAdsrVisualization = new ADSRVisualizationControl(this, IRECT(546, kBlueRow + 7, 648, kBlueRow + 55));
	filterEnvAdsrVisualization->setColor(IColor(100, 25, 121, 173));
	pGraphics->AttachControl(filterEnvAdsrVisualization);

	// Reverb
	// Reverb switch
	GetParam(mVerbOn)->InitEnum("Reverb On/Off", 0, 2);
	GetParam(mVerbOn)->SetDisplayText(0, "On"); 
	pGraphics->AttachControl(new ISwitchControl(this, 42, kPurpleRow + kSwitcherTopPadding, mVerbOn, &switcherOnOffBitmap));

	GetParam(mVerbRoomSize)->InitDouble("Room Size", 0.5, 0.3, 0.99, 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 137, kPurpleRow, mVerbRoomSize, &purpleKnobBitmap));
	GetParam(mVerbDamp)->InitDouble("Dampening", 0.5, 0., 1., 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 195, kPurpleRow, mVerbDamp, &purpleKnobBitmap));
	GetParam(mVerbWidth)->InitDouble("Width", 1., -1., 1., 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 255, kPurpleRow, mVerbWidth, &purpleKnobBitmap));
	GetParam(mVerbDry)->InitDouble("Dry", 1., 0., 1., 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 329, kPurpleRow, mVerbDry, &purpleKnobBitmap));
	GetParam(mVerbWet)->InitDouble("Wet", 0.5, 0., 1., 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 383, kPurpleRow, mVerbWet, &purpleKnobBitmap));

	AttachGraphics(pGraphics);

	CreatePresets();

	mMIDIReceiver.noteOn.Connect(&voiceManager, &VoiceManager::onNoteOn);
	mMIDIReceiver.noteOff.Connect(&voiceManager, &VoiceManager::onNoteOff);
	mMIDIReceiver.pbChanged.Connect(&voiceManager, &VoiceManager::onPbChanged);

	// Some host will not auto-reset the params so it will not set the adsr value, do it here.
	ampAdsrVisualization->setADSR(ampAdsrKnobs[E_Att]->GetValue(), ampAdsrKnobs[E_Dec]->GetValue(),
		ampAdsrKnobs[E_Sus]->GetValue(), ampAdsrKnobs[E_Rel]->GetValue());
	filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
		filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());

}

Synthesis::~Synthesis() {
	if (ori_l != nullptr) free(ori_l);
	if (ori_r != nullptr) free(ori_r);
}

void Synthesis::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	/*
	if (mDry == 0. && mWet == 1.)
	{
		// Process the entire sample block at once (more efficient?).
		// Inputs should from voiceManager. **inputs is NOT writeable for such a vsti.
		mVerbEngine.ProcessSampleBlock(inputs[0], inputs[1], outputs[0], outputs[1], nFrames);
		return; // MIDIReceiver.
	}
	*/
	
	// don't alloc new mem space every time we call ProcessDoubleReplacing. Will faster?
	if (ori_l == nullptr || sizeof(*ori_l) < nFrames * sizeof(double)) {
		if (ori_l) {free(ori_l); ori_l = nullptr;}
		ori_l = (double*)malloc(nFrames * sizeof(double));
	}
	if (ori_r == nullptr || sizeof(*ori_r) < nFrames * sizeof(double)) {
		if(ori_r) { free(ori_r); ori_r = nullptr; }
		ori_r = (double*)malloc(nFrames * sizeof(double));
	}

	double *leftOutput = outputs[0];
	double *rightOutput = outputs[1];
	bool verbFxOn = GetParam(mVerbOn)->Int() == 0;

	processVirtualKeyboard();
	for (int i = 0; i < nFrames; ++i) {
		mMIDIReceiver.advance();
		
		if (!verbFxOn) {
			//Off
			leftOutput[i] = rightOutput[i] = voiceManager.nextSample();
		} else {
			ori_l[i] = ori_r[i] = leftOutput[i] = rightOutput[i] = voiceManager.nextSample();

			// Reverb
			mVerbEngine.ProcessSample(leftOutput + i, rightOutput + i);
			// Mix dry/wet
			leftOutput[i] = mDry * ori_l[i] + mWet * leftOutput[i];
			rightOutput[i] = mDry * ori_r[i] + mWet * rightOutput[i];
		}
	}

	mMIDIReceiver.Flush(nFrames);
}

void Synthesis::CreatePresets()
{
}

void Synthesis::Reset()
{
	TRACE;
	IMutexLock lock(this);
	voiceManager.setSampleRate(GetSampleRate());
	mVerbEngine.SetSampleRate(GetSampleRate());
}

void Synthesis::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
	switch (paramIdx) {
	case mOsc1Waveform:
		voiceManager.setOscillatorModeForEachVoice(1, static_cast<Oscillator::OscillatorMode>(GetParam(mOsc1Waveform)->Int()));
		break;
	case mOsc1Coarse:
		voiceManager.setSemiOffsetForEachVoice(1, GetParam(paramIdx)->Value());
		break;
	case mOsc1Fine:
		voiceManager.setCentOffsetForEachVoice(1, GetParam(paramIdx)->Value());
		break;
	case mOsc2Waveform:
		voiceManager.setOscillatorModeForEachVoice(2, static_cast<Oscillator::OscillatorMode>(GetParam(mOsc2Waveform)->Int()));
		break;
	case mOsc2Coarse:
		voiceManager.setSemiOffsetForEachVoice(2, GetParam(paramIdx)->Value());
		break;
	case mOsc2Fine:
		voiceManager.setCentOffsetForEachVoice(2, GetParam(paramIdx)->Value());
		break;
	case mOscillatorMix:
		voiceManager.setOscillatorMixForEachVoice(GetParam(paramIdx)->Value());
		break;
	case mPhaseStart:
		voiceManager.setPhaseStartForEachVoice(static_cast<bool>(GetParam(paramIdx)->Int()));
		break;
	case mBitCrusher:
		voiceManager.setBitCrusherEnabledForEachVoice(static_cast<bool>(GetParam(paramIdx)->Int()));
		break;
	case mTejiBooster:
		voiceManager.setNumberOfVoices(GetParam(paramIdx)->Int() == 0 ? 64 : 32);
		break;
	case mAmpAmount:
		voiceManager.setAmpAmountForEachVoice(GetParam(paramIdx)->Value());
		break;
	case mAttack:
	case mDecay:
	case mSustain:
	case mRelease:
		voiceManager.setAmpEnvStageValueForEachVoice(static_cast<EnvelopeGenerator::EnvelopeStage>(paramIdx - mAttack + 1), GetParam(paramIdx)->Value());
		ampAdsrVisualization->setADSR(ampAdsrKnobs[E_Att]->GetValue(), ampAdsrKnobs[E_Dec]->GetValue(),
			ampAdsrKnobs[E_Sus]->GetValue(), ampAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterCutoff:
		voiceManager.setFilterCutoffForEachVoice(GetParam(paramIdx)->Value());
		break;
	case mFilterResonance:
		voiceManager.setFilterResonanceForEachVoice(GetParam(paramIdx)->Value());
		break;
	case mFilterMode:
		voiceManager.setFilterModeForEachVoice(static_cast<Filter::FilterMode>(GetParam(paramIdx)->Int()));
		break;
	case mFilterAttack:
	case mFilterDecay:
	case mFilterSustain:
	case mFilterRelease:
		voiceManager.setFilterEnvStageValueForEachVoice(static_cast<EnvelopeGenerator::EnvelopeStage>(paramIdx - mFilterAttack + 1), GetParam(paramIdx)->Value());
		filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
			filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterEnvelopeAmount:
		voiceManager.setFilterAmountForEachVoice(GetParam(paramIdx)->Value());
		break;

	case mVerbRoomSize:
		mVerbEngine.SetRoomSize(GetParam(paramIdx)->Value());
		mVerbEngine.Reset();
		break;
	case mVerbDamp:
		mVerbEngine.SetDampening(GetParam(paramIdx)->Value());
		mVerbEngine.Reset();
		break;
	case mVerbWidth:
		mVerbEngine.SetWidth(GetParam(paramIdx)->Value());
		break;
	case mVerbDry:
		mDry = GetParam(paramIdx)->Value();
		break;
	case mVerbWet:
		mWet = GetParam(paramIdx)->Value();
		break;
	}
}

void Synthesis::ProcessMidiMsg(IMidiMsg* pMsg) {
	mMIDIReceiver.onMessageReceived(pMsg);
	mVirtualKeyboard->SetDirty();
}

void Synthesis::processVirtualKeyboard() {
	IKeyboardControl* virtualKeyboard = (IKeyboardControl*)mVirtualKeyboard;
	int virtualKeyboardNoteNumber = virtualKeyboard->GetKey() + virtualKeyboardMinimumNoteNumber;

	if (lastVirtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
		// The note number has changed from a valid key to something else (valid key or nothing). Release the valid key:
		IMidiMsg midiMessage;
		midiMessage.MakeNoteOffMsg(lastVirtualKeyboardNoteNumber, 0);
		mMIDIReceiver.onMessageReceived(&midiMessage);
	}

	if (virtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
		// A valid key is pressed that wasn't pressed the previous call. Send a "note on" message to the MIDI receiver:
		IMidiMsg midiMessage;
		midiMessage.MakeNoteOnMsg(virtualKeyboardNoteNumber, virtualKeyboard->GetVelocity(), 0);
		mMIDIReceiver.onMessageReceived(&midiMessage);
	}

	lastVirtualKeyboardNoteNumber = virtualKeyboardNoteNumber;
}