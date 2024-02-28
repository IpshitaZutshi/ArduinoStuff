function runSineWave

% run PulsePal to generate a 2 Hz sine wave on channel 1, triggered by a TTL from the
% arduino to trigger channel 1
% Peak to peak amplitude for the sine wave is 20V, spanning Pulse Pal's entire -10V - +10V output range.
% IZ 2019
%PulsePal;

SineWaveVoltageStimGen = 10*sin((2*pi/1000)*(1:1000)); %Range 20V to stimulus generator
SineWaveVoltageIntan = 2*sin((2*pi/1000)*(1:1000)); % Range 4V to intan

ProgramPulsePalParam(1, 14, 1); % Sets output channel 1 to use custom train 1
ProgramPulsePalParam(2, 14, 2); % Sets output channel 2 to use custom train 2

ProgramPulsePalParam(1, 128, 2); % Set trigger channel 1 to toggle mode

ProgramPulsePalParam(1, 12 , 1); % Set output channel 1 to be triggered by trigger channel 1
ProgramPulsePalParam(2, 12 , 1); % Set output channel 2 to be triggered by trigger channel 1

ProgramPulsePalParam(1, 10 , 30); % Set maximum pulse duration for output channel 1 to be 30 sec
ProgramPulsePalParam(2, 10 , 30); % Set maximum pulse duration for output channel 2 to be 30 sec

ProgramPulsePalParam(1, 16 , 1); % Set output channel 1 to loop until pulse duration
ProgramPulsePalParam(2, 16 , 1); % Set output channel 2 to loop until pulse duration

SendCustomWaveform(1, 0.0005, SineWaveVoltageStimGen); % Uploads sine waveform for stim gen channel. Samples are played at 2khz.
SendCustomWaveform(2, 0.0005, SineWaveVoltageIntan); % Uploads sine waveform for intan channel. Samples are played at 2khz.

TriggerPulsePal([1 2]); % Soft-triggers channels 1

end