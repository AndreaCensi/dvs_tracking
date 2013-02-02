function poseAccuracyComp(dvsData,optitrackData,timeOffsetOpti)

Pd_raw = importdata(dvsData);
Po = importdata(optitrackData);

%eliminate duplicates in dvs data
Pd = unique(Pd_raw,'rows','stable');

%dvs data
Td = Pd(:,1:3);
V_r = Pd(:,4:6);

timeD = Pd(:,15);
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

%optitrack data
To = Po(:,[2 1 3]);
To(:,2) = -To(:,2);

Qo = Po(:,[7 4 5 6]);   %format of quat input is [x y z w]

timeO = Po(:,8);
timeO = timeO - timeO(1) + timeOffsetOpti;

% eliminate dvs data which is not covered by optitrack
startO = timeO(1);
endO = timeO(end);

i_interval = timeD > startO & timeD < endO;

Td = Td(i_interval,:);
timeD = timeD(i_interval);

rollD = rollD(i_interval);
pitchD = pitchD(i_interval);
yawD = yawD(i_interval);

% interpolate optitrack data
To_i(:,1) = interp1(timeO,To(:,1),timeD);
To_i(:,2) = interp1(timeO,To(:,2),timeD);
To_i(:,3) = interp1(timeO,To(:,3),timeD);

%determine rotation and translation to align both ref frames

Q0 = ones(4,1);
t0 = zeros(3,1);

x0 = [Q0 ; t0];

x = findTransformation(x0,Td,To_i);

Q = x(1:4,1)';
t = x(5:7,1);

% align optitrack to dvs referece frame
[m n] = size(To_i);
To_i = quatrotate(Q,To_i) + repmat(t',m,1);

Qo_aligned = quatmultiply(Q,Qo);

[yawO,pitchO,rollO] = quat2angle(Qo_aligned);

rollO = radtodeg(rollO);
pitchO = radtodeg(pitchO);
yawO = radtodeg(yawO);

%determine error
distance_V = (Td - To_i)';
norm = sqrt(sum(distance_V.^2));

boxplot(norm);
title('DVS pose estimation error');
ylabel('Distance [m]','Rotation',90);

figure;

%plotting
style = '-o';

subplot(2,2,1); plot(timeD,Td(:,1),style,timeD,To_i(:,1),style);
title('Translation x [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,2); plot(timeD,Td(:,2),style,timeD,To_i(:,2),style);
title('Translation y [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,3); plot(timeD,Td(:,3),style,timeD,To_i(:,3),style);
title('Translation z [m]');
xlabel('Time [s]');
ylabel('x');

figure;

subplot(2,2,1); plot(timeD,yawD,style,timeO,yawO,style);
title('Yaw');
xlabel('Time [s]');
ylabel('Degree');

subplot(2,2,2); plot(timeD,pitchD,style,timeO,pitchO,style);
title('Pitch');
xlabel('Time [s]');
ylabel('Degree');

subplot(2,2,3); plot(timeD,rollD,style,timeO,rollO,style);
title('Roll');
xlabel('Time [s]');
ylabel('Degree');
