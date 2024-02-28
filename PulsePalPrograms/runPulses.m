function runPulses(voltages,durations,nPulses,interval,randomness)

% run PulsePal pulses specified by inputs. Accepts vector with pulses
% combination, leaving a time delay between diferent programs of 1 second.
% MV 2019

timeOffset = 1; % time between protocols
for ii = 1:length(voltages)
    fprintf("Pulse %i of %i \n",ii,length(voltages));
    fprintf("    Voltage      : %i V \n", voltages(ii));
    fprintf("    Duration     : %i ms \n", durations(ii));
    fprintf("    # of pulses  : %i pulses \n", nPulses(ii));
    fprintf("    interval     : %i sec \n", nPulses(ii));
    
    % Generate pulses
    pulseTimes = [1:interval(ii):nPulses*interval(ii)];
    pulseTimes = pulseTimes + (rand(1,length(pulseTimes))-.5)*randomness(ii);
    pulseTimes = round(pulseTimes,4); % .1 milliseconds resolution    
    pulseVoltages = ones(1,length(pulseTimes)) * voltages(ii);
    
    fprintf("    Prot duration: %1.2f s \n", pulseTimes(end));
    SendCustomPulseTrain(1, pulseTimes, pulseVoltages); % Uploads our train in slot 1 of 2
%     ProgramPulsePalParam(1, 14, 1); % Sets output channel 1 to use custom train 1
%     ProgramPulsePalParam(1, 4, durations(ii)/1000); % Sets output channel 1 to use 250?s pulses
%     TriggerPulsePal('1');
    
    ProgramPulsePalParam(1, 14, 1); % Sets output channel 1 to use custom train 1
    ProgramPulsePalParam(1, 4, durations(ii)/1000); % Sets output channel 1 to use 250?s pulses
    
    ProgramPulsePalParam(4, 14, 1); % Sets output channel 1 to use custom train 1
    ProgramPulsePalParam(4, 4, durations(ii)/1000); % Sets output channel 1 to use 250?s pulses
    
    ProgramPulsePalParam(2, 14, 1); % Sets output channel 1 to use custom train 1
    ProgramPulsePalParam(2, 4, durations(ii)/1000); % Sets output channel 1 to use 250?s pulses
    TriggerPulsePal([1 2 4]);
    
    pause(pulseTimes(end)+timeOffset);
    fprintf("End of pulse %i of %i \n",ii,length(voltages));
end 

end