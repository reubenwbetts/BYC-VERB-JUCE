MUST BE RUN AT A 48K SAMPLE RATE 

A JUCE Plugin showcasing a reverb algorithim developed for my DSP class in university. Originally implemeented on an STM32 as a direct copy of the algorithim detailed in Datorro's paper, this project 
exists as an easier to access demo of the algorithim, and expands upon my orginal implementation with the following features:

- Stereo rather than mono
- Static biquad bank for tone shaping on the output
- A basic decimation (sample every second sample) and upsampling algorithim (zero-stuffing) to bring the sample rate more inline with the papers spec.
- 50:50 Blend of early diffusion and late reflection stages on the output.

The UI in the projucer file is different to the UI in the builds, as the builds make use of closed source assets for the knobs and wheels.

Build UI:

<img width="475" alt="image" src="https://github.com/user-attachments/assets/1128fee1-15a5-4604-9202-354369c2b207" />
