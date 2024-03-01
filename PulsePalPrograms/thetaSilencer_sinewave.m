function thetaSilencer_sinewave

%Pulse mEC with sine waves of a given frequency and variable power for 15 minutes, 5 seconds ON, 25 seconds OFF

PulsePal('COM4');
for sess = 1%:2
    voltage  = [4 3.5 3];
    pulseDur = 5;
    pulseFreq = [50 20 10];% 20]; % in Hz
    ITI  = 25;
    totalDur = 10*60; %Number of minutes * 60 (seconds)
    Channel1 = 1;
    Channel2 = 3;
    ProgramPulsePalParam(Channel1, 14, 1); % Sets output channel 2 to use custom train 1
    ProgramPulsePalParam(Channel2, 14, 1); % Sets output channel 4 to use custom train 1

    for l = 1:3
        for i = 1:(totalDur/(ITI+pulseDur))

            randVolt = randi(3);
            SineWaveVoltages = voltage(randVolt)*sin((2*pi/100)*(1:1000));

            ProgramPulsePalParam(Channel1, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
            ProgramPulsePalParam(Channel1, 16 , 1); % Set output channel 2 to loop until pulse duration
            ProgramPulsePalParam(Channel2, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
            ProgramPulsePalParam(Channel2, 16 , 1); % Set output channel 2 to loop until pulse duration

            SamplePlay = (1/pulseFreq(l))*(1/100);
            SendCustomWaveform(1, SamplePlay, SineWaveVoltages); % Uploads sine waveform for intan channel. Samples are played at 2khz.

            TriggerPulsePal([Channel1 Channel2]); % Soft-triggers channel
            %TriggerPulsePal([Channel1]); % Soft-triggers channel
            pause(ITI)
        end

        waitMin(10);
    end
end
