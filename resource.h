#define PLUG_MFR "BLumia"
#define PLUG_NAME "Pineapple Synth - Mini"

#define PLUG_CLASS_NAME Synthesis

#define BUNDLE_MFR "BLumia"
#define BUNDLE_NAME "Pineapple Synth - Mini"

#define PLUG_ENTRY Synthesis_Entry
#define PLUG_VIEW_ENTRY Synthesis_ViewEntry

#define PLUG_ENTRY_STR "Synthesis_Entry"
#define PLUG_VIEW_ENTRY_STR "Synthesis_ViewEntry"

#define VIEW_CLASS Synthesis_View
#define VIEW_CLASS_STR "Synthesis_View"

// Format        0xMAJR.MN.BG - in HEX! so version 10.1.5 would be 0x000A0105
#define PLUG_VER 0x00010000
#define VST3_VER_STR "1.0.0"

// http://service.steinberg.de/databases/plugin.nsf/plugIn?openForm
// 4 chars, single quotes. At least one capital letter
#define PLUG_UNIQUE_ID 'haBL'
// make sure this is not the same as BUNDLE_MFR
#define PLUG_MFR_ID 'BKT'

// ProTools stuff

#if (defined(AAX_API) || defined(RTAS_API)) && !defined(_PIDS_)
  #define _PIDS_
  const int PLUG_TYPE_IDS[2] = {'EFN1', 'EFN2'};
  const int PLUG_TYPE_IDS_AS[2] = {'EFA1', 'EFA2'}; // AudioSuite
#endif

#define PLUG_MFR_PT "BLumia\nBLumia\nBearKidsTeam"
#define PLUG_NAME_PT "Pineapple Synth - Mini\nhaBL"
#define PLUG_TYPE_PT "Effect"
#define PLUG_DOES_AUDIOSUITE 1

/* PLUG_TYPE_PT can be "None", "EQ", "Dynamics", "PitchShift", "Reverb", "Delay", "Modulation", 
"Harmonic" "NoiseReduction" "Dither" "SoundField" "Effect" 
instrument determined by PLUG _IS _INST
*/

// #define PLUG_CHANNEL_IO "1-1 2-2"
#if (defined(AAX_API) || defined(RTAS_API)) 
#define PLUG_CHANNEL_IO "1-1 2-2"
#else
// no audio input. mono or stereo output
#define PLUG_CHANNEL_IO "0-1 0-2"
#endif

#define PLUG_LATENCY 0
#define PLUG_IS_INST 1

// if this is 0 RTAS can't get tempo info
#define PLUG_DOES_MIDI 1

#define PLUG_DOES_STATE_CHUNKS 0

// Unique IDs for each image resource.
#define BG_ID         101
#define PIANO_KEY_ID  102
#define WAVEFORM_ID   103
#define GREEN_KNOB_ID 104
#define BLUE_KNOB_ID  105
#define FILTERMODE_ID   106
#define ORANGE_KNOB_ID  107
#define PURPLE_KNOB_ID  108
#define BLUE_KNOB_CENTER_ID  109
#define GREEN_KNOB_CENTER_ID 110
#define SWITCHER_LIGHT_ID    111
#define SWITCHER_ONOFF_ID    112

// Image resource locations for this plug.
#define BG_FN         "resources/img/bg.png"
#define PIANO_KEY_FN  "resources/img/key.png"
#define WAVEFORM_FN   "resources/img/waveform.png"
#define GREEN_KNOB_FN "resources/img/greenknob.png"
#define BLUE_KNOB_FN  "resources/img/blueknob.png"
#define FILTERMODE_FN "resources/img/filtermode.png"
#define ORANGE_KNOB_FN "resources/img/orangeknob.png"
#define PURPLE_KNOB_FN "resources/img/purpleknob.png"
#define GREEN_KNOB_CENTER_FN  "resources/img/greenknob-center0.png"
#define BLUE_KNOB_CENTER_FN  "resources/img/blueknob-center0.png"
#define SWITCHER_LIGHT_FN "resources/img/switcherlight.png"
#define SWITCHER_ONOFF_FN "resources/img/switcheronoff.png"

// GUI default dimensions
#define GUI_WIDTH 665
#define GUI_HEIGHT 324

// on MSVC, you must define SA_API in the resource editor preprocessor macros as well as the c++ ones
#if defined(SA_API) && !defined(OS_IOS)
#include "app_wrapper/app_resource.h"
#endif

// vst3 stuff
#define MFR_URL "www.olilarkin.co.uk"
#define MFR_EMAIL "spam@me.com"
#define EFFECT_TYPE_VST3 "Instrument|Synth"

/* "Fx|Analyzer"", "Fx|Delay", "Fx|Distortion", "Fx|Dynamics", "Fx|EQ", "Fx|Filter",
"Fx", "Fx|Instrument", "Fx|InstrumentExternal", "Fx|Spatial", "Fx|Generator",
"Fx|Mastering", "Fx|Modulation", "Fx|PitchShift", "Fx|Restoration", "Fx|Reverb",
"Fx|Surround", "Fx|Tools", "Instrument", "Instrument|Drum", "Instrument|Sampler",
"Instrument|Synth", "Instrument|Synth|Sampler", "Instrument|External", "Spatial",
"Spatial|Fx", "OnlyRT", "OnlyOfflineProcess", "Mono", "Stereo",
"Surround"
*/
