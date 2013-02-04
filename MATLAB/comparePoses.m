function ret = comparePoses(dvsData,ptamData,optitrackData,offsetPtam,offsetOpti)

Pd_raw = importdata(dvsData);
Pp = importdata(ptamData);
Po = importdata(optitrackData);

%eliminate transformation duplicates in dvs data (ignore timestamp)
[Pd,ia,ic] = unique(Pd_raw(:,1:6),'rows','stable');
Pd = [Pd Pd_raw(ia,15)];

non_f = Pd(:,1) < 1000;
Pd = Pd(non_f,:);

%dvs data
Td = Pd(:,1:3);
V_r = Pd(:,4:6);

timeD = Pd(:,7);
timeD = timeD - timeD(1);

RPY = zeros(size(V_r));

[m n] = size(V_r);

for i = 1:m
    RPY(i,:) = rpyAng(rodrigues(V_r(i,:)));
end;

RPY = radtodeg(RPY);

rollD = RPY(:,1);
pitchD = RPY(:,2);
yawD = RPY(:,3);

%parrot data
Tp = Pp(:,1:3);
Qp = Pp(:,4:7);

timeP = Pp(:,8);
timeP = timeP - timeP(1) + offsetPtam;

[yawP,pitchP,rollP] = quat2angle(Qp);

yawP = radtodeg(yawP);
rollP = radtodeg(pitchP);
pitchP = radtodeg(rollP);

%optitrack data
To = Po(:,1:3);
Qo = Po(:,4:7);

timeO = Po(:,8);
timeO = timeO - timeO(1) + offsetOpti;

[yawO,pitchO,rollO] = quat2angle(Qo);

rollO = radtodeg(pitchO);
pitchO = radtodeg(rollO);
yawO = radtodeg(yawO);

%plotting
style = '-';

subplot(2,2,1); plot(timeP,Tp(:,1),style,timeD,Td(:,1),style,timeO,To(:,2),style);
title('Translation x [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,2); plot(timeP,Tp(:,2),style,timeD,Td(:,2),style,timeO,-To(:,1),style);
title('Translation y [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,3); plot(timeP,Tp(:,3),style,timeD,Td(:,3),style,timeO,To(:,3),style);
title('Translation z [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,4); plot(timeP,rollP,style,timeD,rollD,style,timeO,rollO,style);
%plot(timeO,rollO,style);
title('Roll');
xlabel('Time [s]');
ylabel('Degree');
