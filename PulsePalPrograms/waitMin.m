function waitMin(timeToWait)

%wait specified minutes.
% MV2019

timeToWait = (timeToWait * 60) + 1;
timeSlots = 1:600:timeToWait;
if timeToWait > max(timeSlots)
    timeSlots(length(timeSlots)+1) = timeToWait;
end
timeSlots = diff(timeSlots);
disp('Waiting...');
k = 0;
for ii = 1:length(timeSlots)
    pause(timeSlots(ii));
    k = k + timeSlots(ii);
    fprintf("   %i min of %i \n",k/60,timeToWait);
end

end