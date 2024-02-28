function thetaSilencer

%(1) First generate a square pulse with duration 5s, followed by off for 25s on channel 3, for 15 mins. 
%(2) Pause 30 mins.
%(3) Pulse 20 Hz (50% duty cycle) for 5s, followed by off for 25s on channel 4, for 15 mins.
%(4) Pause 30 mins.
%(5) Simultaneously perform (1) and (3).
%(6) Pause 30 mins.

PulsePal('COM4');

voltage = 3.3;
%% Set parameters for channel 3
ProgramPulsePalParam(3, 'Phase1Voltage', voltage); % Set output channel 3 to produce 4.8V pulses
ProgramPulsePalParam(3, 'Phase1Duration', 5); % Set output channel 3 to produce 5s pulses
ProgramPulsePalParam(3, 'InterPulseInterval', 25); %  Set pulse interval to 25s.
ProgramPulsePalParam(3, 'PulseTrainDuration', 900); % Set pulse train to last 900 seconds

%% Set parameters for channel 4
voltage = 3; 
Voltages(1) = voltage;
numSecs = 5;
Freq = 20; %Pulse at 20 Hz
PulseTimes(1) = 25; %PulseTimesdelay
for i  = 2:(numSecs*Freq)
    PulseTimes(i) = PulseTimes(i-1) + 0.05;
    Voltages(i) = voltage;
end
SendCustomPulseTrain(1, PulseTimes, Voltages); % Uploads our train in slot 1 of 2
ProgramPulsePalParam(4,'CustomTrainID', 1); % Sets output channel 4 to use custom train 1
ProgramPulsePalParam(4,'CustomTrainTarget', 0); % Sets output channel 4 to use custom pulses
ProgramPulsePalParam(4,'CustomTrainLoop', 1); % Sets output channel 4 to loop until PulseTrainDuration
ProgramPulsePalParam(4,'Phase1Duration', 0.025); % Sets output channel 4 to pulse for 25 ms.
ProgramPulsePalParam(4, 'PulseTrainDuration', 900); % Set pulse train to last 900 seconds

% Now run the script
%% Pulses to CA1
fprintf("    Starting 5s long pulses to CA1\n");
TriggerPulsePal(3);
waitMin(45);

%% Pulses to mEC
fprintf("    Starting pulses to mEC at 20 Hz\n");
TriggerPulsePal(4);
waitMin(45);

%% Pulses to both CA1 and mEC
fprintf("    Starting pulses to both CA1 and mEC\n");
TriggerPulsePal(4);
pause(25)
TriggerPulsePal(3);
waitMin(45);

%%
%EndPulsePal;
end

