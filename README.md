# Granulizer Thingy / Live Granular Processor

This was an experiment to try out a graunular processor with 1-6 voices that processes live input. 
The idea was to keep audio in a circular buffer in order to make grains play back from the live input. 
A grain will start playing from the record position. It will play for the duration of the grain size, set in miliseconds. 
Reptitions slider chooses how many times it will repeat this grain before selecting a new grain window at the record position. 

This way, with small windows you can granulize the live input without too much delay or choose to have grains stay for longer with the repitions. 

### Sliders
- **Vocies** is the number of grains/voices you want to play. Currently from 1-6
- **Grain Size** is the window size of the grain/voice
- **Random Range** is much random value should be added to the grain size.
- **Repetitions** is how many repetitions a grain window will play before selecting a new one.
- **Dry** is the volume of the dry, unprocessed signal
- **Wet** is the volume of the wet, processed signal, the grains.

 ![Granulizer](/githubImg/Granulizer.png)

