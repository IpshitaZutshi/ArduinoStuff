function thetaSilencer_fixedVoltage_frequency

%(1) Generate a square pulse 10, 20 and 50 Hz with duration 5s, followed by off for 25s on channel 3, for 15 mins. 

PulsePal('COM4');

voltage  = 3.5;
pulseDur = 5;
pulseFreq = [50 10 20];% 20]; % in Hz
ITI  = 25;
totalDur = 15*60; %Number of minutes * 60 (seconds)
Channel1 = 2;
Channel2 = 4;
ProgramPulsePalParam(Channel1, 14, 1); % Sets output channel 2 to use custom train 1
ProgramPulsePalParam(Channel2, 14, 1); % Sets output channel 4 to use custom train 1

for l = 1:3
    for i = 1:(totalDur/(ITI+pulseDur))

        SineWaveVoltages = voltage*square((2*pi/100)*(1:1000));

        ProgramPulsePalParam(Channel1, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
        ProgramPulsePalParam(Channel1, 16 , 1); % Set output channel 2 to loop until pulse duration
        
        ProgramPulsePalParam(Channel2, 10 , pulseDur); % Set maximum pulse duration for output channel to be 5 sec
        ProgramPulsePalParam(Channel2, 16 , 1); % Set output channel 2 to loop until pulse duration
        
        SamplePlay = (1/pulseFreq(l))*(1/100);
        SendCustomWaveform(1, SamplePlay, SineWaveVoltages); % Uploads waveform for intan channel. Samples are played at 2khz.
        
        TriggerPulsePal([Channel1 Channel2]); % Soft-triggers channel

        pause(ITI)
    end

    waitMin(30);
end


%% Set parameters for channel2&  4
voltage = 3.5;
numSecs = 5;
PulseTimesdelay = 30; %PulseTimesdelay
PulseTrainDuration = 900;
for i  = 1:(PulseTrainDuration/PulseTimesdelay)
    PulseTimes(i) = PulseTimesdelay*(i-1);
    Voltages(i) = voltage(1);
end
SendCustomPulseTrain(2, PulseTimes, Voltages); % Uploads our train in slot 2 of 2
%Upload to Ch2 (Rec sys 3)
ProgramPulsePalParam(2,'CustomTrainID', 2); % Sets output channel 4 to use custom train 2
ProgramPulsePalParam(2,'CustomTrainTarget', 0); % Sets output channel 4 to use custom pulses
ProgramPulsePalParam(2,'Phase1Duration', numSecs); % Sets output channel 4 to pulse for 25 ms.
ProgramPulsePalParam(2,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds

%Upload to Ch4 (Rec sys 3)
ProgramPulsePalParam(4,'CustomTrainID', 2); % Sets output channel 4 to use custom train 2
ProgramPulsePalParam(4,'CustomTrainTarget', 0); % Sets output channel 4 to use custom pulses
ProgramPulsePalParam(4,'Phase1Duration', numSecs); % Sets output channel 4 to pulse for 25 ms.
ProgramPulsePalParam(4,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds

% Now run the script
%% Pulses to CA1
%waitMin(8);
% 
% fprintf("    Starting 5s long pulses to CA1\n");
% TriggerPulsePal([1 3]);
% waitMin(30);

%Pulses to mEC
fprintf("    Starting 5s long pulses to mEC\n");
TriggerPulsePal([2 4]);
waitMin(30);
  
% Pulses to both CA1 and mEC
% fprintf("    Starting pulses to both CA1 and mEC\n");
% TriggerPulsePal([1 2 3 4]);
% waitMin(30);
% 
% fprintf("    Starting 5s long pulses to CA1\n");
% TriggerPulsePal([1 3]);
% waitMin(30);
% 
% fprintf("    Starting 5s long pulses to CA1\n");
% TriggerPulsePal([1 3]);
% waitMin(30);

%%
EndPulsePal;
end

