function [pulseTimes, pulseVoltages] = runPulses_rh(nBlocks, voltages,durations,interval,randomness)

% run PulsePal pulses specified by inputs. Accepts vector with pulses
% combination, leaving a time delay between diferent programs of 1 second.
% MV 2019

nPulsesPerBlock = length(voltages);
timeOffset = 1; % time between protocols

% Generate pulses
pulseTimes = cumsum( repmat(interval, [1, nBlocks]) );
pulseTimes = pulseTimes + (rand(1, nBlocks*nPulsesPerBlock) - 0.5) .* repmat(randomness, [1, nBlocks]);
pulseTimes = round(pulseTimes,4);
% Generate voltages
pulseVoltages = repmat(voltages, [1, nBlocks]);

 
SendCustomPulseTrain(1, pulseTimes, pulseVoltages); % Uploads our train in slot 1 of 2

ProgramPulsePalParam(4, 14, 1); % Sets output channel 1 to use custom train 1
ProgramPulsePalParam(4, 4, durations(1)/1000); % Sets output channel 1 to use 250?s pulses
TriggerPulsePal(4);

pause(pulseTimes(end)+timeOffset);

end