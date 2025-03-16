function DAStim

% Generate a square pulse on channel 3,4 with duration 0.5s, followed by off for 24.5s on channel 3, for 30 mins. 

PulsePal('COM6');

%% Set parameters for channel 3
voltage = 8;
numSecs = 0.5;
PulseTimesdelay = 20; %PulseTimesdelay
PulseTrainDuration = 900;
for i  = 1:(PulseTrainDuration/PulseTimesdelay)
    PulseTimes(i) = PulseTimesdelay*(i-1);
    Voltages(i) = voltage(1);
end
    
%% Set parameters for channel 4
voltage = 3;
numSecs = 0.5;
PulseTimesdelay = 20; %PulseTimesdelay
PulseTrainDuration = 900;
for i  = 1:(PulseTrainDuration/PulseTimesdelay)
    PulseTimes(i) = PulseTimesdelay*(i-1);
    Voltages(i) = voltage(1);
end

SendCustomPulseTrain(1, PulseTimes, Voltages); % Uploads our train in slot 1 of 2

%Upload to Ch3
ProgramPulsePalParam(3,'CustomTrainID', 1); % Sets output channel 3 to use custom train 1
ProgramPulsePalParam(3,'CustomTrainTarget', 0); % Sets output channel 3 to use custom pulses
ProgramPulsePalParam(3,'Phase1Duration', numSecs); % Sets output channel 3 to pulse for 25 ms.
ProgramPulsePalParam(3,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds

%Upload to Ch4
ProgramPulsePalParam(4,'CustomTrainID', 1); % Sets output channel 3 to use custom train 1
ProgramPulsePalParam(4,'CustomTrainTarget', 0); % Sets output channel 3 to use custom pulses
ProgramPulsePalParam(4,'Phase1Duration', numSecs); % Sets output channel 3 to pulse for 25 ms.
ProgramPulsePalParam(4,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds

% Now run the script
fprintf("    Starting 0.5s long pulses\n");
TriggerPulsePal([3 4]);
waitMin(30);

%%
EndPulsePal;
end

