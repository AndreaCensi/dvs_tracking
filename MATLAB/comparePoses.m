function ret = comparePoses(dvsData,parrotData)

Pd = importdata(dvsData);
Pp = importdata(parrotData);

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

yawD = RPY(:,3);

%parrot data
Tp = Pp(:,1:3);
Q = Pp(:,4:7);

timeP = Pp(:,8);
timeP = timeP - timeP(1);

[yawP,pitch,roll] = quat2angle(Q);

yawP = radtodeg(yawP);


subplot(2,2,1); plot(timeP,Tp(:,1),timeD,Td(:,1));
title('Translation x [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,2); plot(timeP,Tp(:,2),timeD,Td(:,2));
title('Translation y [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,3); plot(timeP,Tp(:,3),timeD,Td(:,3));
title('Translation z [m]');
xlabel('Time [s]');
ylabel('x');

subplot(2,2,4); plot(timeP,yawP,timeD,yawD);
title('Yaw');
xlabel('Time [s]');
ylabel('Degree');
