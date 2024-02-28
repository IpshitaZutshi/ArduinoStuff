% Stimulation protocol

basepath = 'C:\Users\BuzLab\Documents\Roman\EPE15_2F2_190906_114408';

PulsePal('COM4');

% Pulses features
nBlocks = 250;
voltages = [ 3.5, 3.55, 3.6, 3.65, 3.7 ]+1; % in Volts
durations = [ 100, 100, 100, 100, 100 ];  % in ms
interval = [ 1.5, 1.5, 1.5, 1.5, 1.5 ]; % Average interval
randomness = [1, 1, 1, 1, 1];  % Jitter length of interval (plus minus half the value)

disp('First round of pulses')
voltages = voltages/1.5;

[pulseTimes, pulseVoltages] = runPulses_rh(nBlocks, voltages,durations,interval,randomness);

outpath = fullfile(basepath, 'stim_parameters.mat');
save(outpath, 'pulseTimes', 'pulseVoltages', 'durations')

