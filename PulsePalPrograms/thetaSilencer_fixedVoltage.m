function thetaSilencer_fixedVoltage

%(1) First generate a square pulse with duration 5s, followed by off for 25s on channel 3, for 15 mins. 
%(2) Pause 30 mins.
%(3) Pulse 20 Hz (50% duty cycle) for 5s, followed by off for 25s on channel 4, for 15 mins.
%(4) Pause 30 mins.
%(5) Simultaneously perform (1) and (3).
%(6) Pause 30 mins.

PulsePal('COM4');

%% Set parameters for channel 1& 3
voltage = 4;
numSecs = 5;
PulseTimesdelay = 30; %PulseTimesdelay
PulseTrainDuration = 900;
for i  = 1:(PulseTrainDuration/PulseTimesdelay)
    PulseTimes(i) = PulseTimesdelay*(i-1);
    Voltages(i) = voltage(1);
end
    
SendCustomPulseTrain(1, PulseTimes, Voltages); % Uploads our train in slot 1 of 2
%Upload to Ch1 (Rec sys 3)
ProgramPulsePalParam(1,'CustomTrainID', 1); % Sets output channel 1 to use custom train 1
ProgramPulsePalParam(1,'CustomTrainTarget', 0); % Sets output channel 1 to use custom pulses
ProgramPulsePalParam(1,'Phase1Duration', numSecs); % Sets output channel 1 to pulse for 25 ms.
ProgramPulsePalParam(1,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds
%Upload to Ch3 (Rec sys 4)
ProgramPulsePalParam(3,'CustomTrainID', 1); % Sets output channel 3 to use custom train 1
ProgramPulsePalParam(3,'CustomTrainTarget', 0); % Sets output channel 3 to use custom pulses
ProgramPulsePalParam(3,'Phase1Duration', numSecs); % Sets output channel 3 to pulse for 25 ms.
ProgramPulsePalParam(3,'PulseTrainDuration', PulseTrainDuration); % Set pulse train to last 900 seconds

%% Set parameters for channel2&  4
voltage = 4;
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
%Pulses to mEC
%waitMin(30);
fprintf("    Starting 5s long pulses to mEC\n");
TriggerPulsePal([2 4]);
waitMin(30);
TriggerPulsePal([2 4]);
waitMin(30);
TriggerPulsePal([2 4]);
waitMin(30);

% % 
% %waitMin(15);
% %Pulses to CA1
% fprintf("    Starting 5s long pulses to CA1\n");
% %TriggerPulsePal([1 3]);
% TriggerPulsePal([1]);
% waitMin(15);

% %Pulses to both CA1 and mEC
% fprintf("    Starting pulses to both CA1 and mEC\n");
% TriggerPulsePal([1 2 3 4]);
% waitMin(15); 

%%
EndPulsePal;
end

