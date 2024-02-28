% Stimulation protocol
PulsePal('COM4');

% Pulses features
voltages = [4.8 4.8 4.8, 4.85, 4.85, 4.85, 4.9 4.9 4.9, 4.9 , 4.9]; % in Volts, for fNos6
durations = [100, 50, 20, 100, 50, 20, 100, 50, 20, 200, 500]; % in ms
nPulses = [250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250]; % Number of pulses
interval = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]; % Average interval
randomness = [.5, .5 .5, .5, .5 .5, .5, .5 .5, .5, .5]; % Seconds

disp('First round of pulses')
% voltages = voltages/1.5;

%voltages = voltages/5;
% voltages = [3, 3 3, 4, 4, 4, 5 5 5, 5, 5]; % in Volts, for fNos6
% voltages = voltages - 0.05; % for Nos6 in rig1
% voltages = voltages + 1.45; % I tried this with Nkx2...
% voltages = voltages + 4; % for Nos8 in rig1
% voltages = voltages + 2; % then I tried this with Nkx2
runPulses(voltages,durations,nPulses,interval,randomness);

waitMin(60);
    
disp('Second round of pulses')
runPulses(voltages,durations,nPulses,interval,randomness);


% DSA manipulations
voltages = [4 4]; % in Volts, for fNos6
durations = [100 100 ]; % in ms
nPulses = [1000 1000]; % Number of pulses
interval = [.5 .5]; % Average interval
randomness = [.3 .3]; % Seconds
runPulses(voltages,durations,nPulses,interval,randomness);

