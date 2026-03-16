my reference implementation of bilinear / biquadratic digital filters in C, with bindings for Python and examples

the C code has no dependencies. Python binding requires numpy, `plot.py` example requires matplotlib and `play.py` example requires soundfile and sounddevice

# coefficient formula sources

- bilinear: https://dsp.stackexchange.com/a/93451
- biquadratic: https://shepazu.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

# sound sources

- `birds.ogg`: https://freesound.org/people/Vonora/sounds/269570/
- `radio.ogg`: https://freesound.org/people/qubodup/sounds/182817/
