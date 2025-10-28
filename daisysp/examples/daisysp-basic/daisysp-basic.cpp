#include "Umfeld.h"
#include "UmfeldDaisySP.h"
#include "daisysp.h"

using namespace umfeld;
using namespace daisysp;

Overdrive  drive;
Oscillator osc, lfo;

void settings() {
    size(1024, 768);
    audio(0, 2);
}

void setup() {
    colorMode(RGB, 1.0, 1.0, 1.0, 1.0);
    osc.Init(audio_sample_rate);
    lfo.Init(audio_sample_rate);
    lfo.SetAmp(.8f);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetFreq(.25f);
}

void draw() {
    background(0.85f);
}

void audioEvent() {
    float sample_buffer[audio_buffer_size];
    for (int i = 0; i < audio_buffer_size; i++) {
        drive.SetDrive(fabsf(lfo.Process()));
        const float sig  = drive.Process(osc.Process());
        sample_buffer[i] = sig;
    }
    merge_interleaved_stereo(sample_buffer, sample_buffer, audio_output_buffer, audio_buffer_size);
}
