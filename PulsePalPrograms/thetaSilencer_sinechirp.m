function thetaSilencer_sinechirp

%Pulse mEC with sine waves of a given frequency and variable power for 15 minutes, 5 seconds ON, 25 seconds OFF


PulsePal('COM4');

voltage  = 3.5;%[2.5 2 1.5];
pulseDur = 5;
pulseFreq = (1:1:50); % in Hz
ITI  = 10;
totalDur = 25*60; %Number of minutes * 60 (seconds)
Channel1 = 2;
Channel2 = 4;
ProgramPulsePalParam(Channel1, 14, 1); % Sets output channel 2 to use custom train 1
ProgramPulsePalParam(Channel2, 14, 1); % Sets output channel 4 to use custom train 1

for l = 1:3
    for i = 1:(totalDur/(ITI+pulseDur))

        SineWaveVoltages = voltage*sin((2*pi/1000)*(1:1000)*pulseFreq(i));

        ProgramPulsePalParam(Channel1, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
        ProgramPulsePalParam(Channel1, 16 , 1); % Set output channel 2 to loop until pulse duration
        ProgramPulsePalParam(Channel2, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
        ProgramPulsePalParam(Channel2, 16 , 1); % Set output channel 2 to loop until pulse duration

        SamplePlay = (1/1000);
        SendCustomWaveform(1, SamplePlay, SineWaveVoltages); % Uploads sine waveform for intan channel. Samples are played at 1khz.
        TriggerPulsePal([Channel1 Channel2]); % Soft-triggers channel

        pause(ITI)
    end

    waitMin(20);
end
