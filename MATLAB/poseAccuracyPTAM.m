function [pose_err,roll_err,pitch_err,yaw_err] = poseAccuracyPTAM(ptamData,optitrackData,timeOffsetPTAM,timeOffsetOpti,outputPrefixName)

Pp = importdata(ptamData);
Po = importdata(optitrackData);

%% Data extraction
%dvs data
Tp = Pp(:,1:3);
Qp = Pp(:,[7 4 5 6]);   %format of quat input is [x y z w]

timeP = Pp(:,8);
timeP = timeP - timeP(1) + timeOffsetPTAM;

%optitrack data
To = Po(:,[2 1 3]);
To(:,2) = -To(:,2);

Qo = Po(:,[7 4 5 6]);   %format of quat input is [x y z w]

timeO = Po(:,8);
timeO = timeO - timeO(1) + timeOffsetOpti;

% eliminate ptam data which is not covered by optitrack
startO = timeO(1);
endO = timeO(end);

i_interval = timeP > startO & timeP < endO;

Tp = Tp(i_interval,:);
timeP = timeP(i_interval);
Qp = Qp(i_interval,:);

% interpolate optitrack translation data
To_i(:,1) = interp1(timeO,To(:,1),timeP);
To_i(:,2) = interp1(timeO,To(:,2),timeP);
To_i(:,3) = interp1(timeO,To(:,3),timeP);

%% Transformation to DVS reference frame
%determine rotation and translation to align both ref frames

Q0 = ones(4,1);
t0 = zeros(3,1);

x0 = [Q0 ; t0];

x = findTransformation(x0,Tp,To_i);

Q = x(1:4,1)';
t = x(5:7,1);

% align optitrack to drone referece frame
[m n] = size(To_i);
To_i = quatrotate(Q,To_i) + repmat(t',m,1); %interpolated data

[m n] = size(To);
To_r = quatrotate(Q,To) + repmat(t',m,1);   %raw optitrack data

Qo_aligned = quatmultiply(Q,Qo);

[yawO,pitchO,rollO] = quat2angle(Qo_aligned);

rollO = radtodeg(rollO);
pitchO = radtodeg(pitchO);
yawO = radtodeg(yawO);

[yawP,pitchP,rollP] = quat2angle(Qp);

rollP = -radtodeg(rollP);
pitchP = -radtodeg(pitchP);
yawP = radtodeg(yawP);

%determine translation error
distance_V = (Tp - To_i)';
pose_err = (sqrt(sum(distance_V.^2)))';

%interpolate optitrack rotation data
rollO_i = interp1(timeO,rollO,timeP);
pitchO_i = interp1(timeO,pitchO,timeP);
yawO_i = interp1(timeO,yawO,timeP);

% Align axis rotations
x0 = 0;
shift = findRotationShift(x0,rollP,rollO_i);
rollO_i = rollO_i + shift;
rollO = rollO + shift;

shift = findRotationShift(x0,pitchP,pitchO_i);
pitchO_i = pitchO_i + shift;
pitchO = pitchO + shift;

shift = findRotationShift(x0,yawP,yawO_i);
yawO_i = yawO_i + shift;
yawO = yawO + shift;

%determine rotation error
yaw_err = (abs(yawO_i - yawP));
pitch_err = (abs(pitchO_i - pitchP));
roll_err = (abs(rollO_i - rollP));

%% Plotting
% translation plot
%figure;

style = '-';

subplot(2,2,1); plot(timeP,Tp(:,1),style,timeO,To_r(:,1),style);
title('Translation x-axis','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Distance [m]');
legend('DVS','OptiTrack');

subplot(2,2,2); plot(timeP,Tp(:,2),style,timeO,To_r(:,2),style);
title('Translation y-axis','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Distance [m]');
legend('DVS','OptiTrack');

subplot(2,2,3); plot(timeP,Tp(:,3),style,timeO,To_r(:,3),style);
title('Translation z-axis','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Distance [m]');
legend('DVS','OptiTrack');

h = gcf;
saveas(h,[outputPrefixName '_translation'],'fig');

% rotation plot
%figure;

subplot(2,2,1); plot(timeP,yawP,style,timeO,yawO,style);
title('Yaw','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);
legend('DVS','OptiTrack');

subplot(2,2,2); plot(timeP,pitchP,style,timeO,pitchO,style);
title('Pitch','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);
legend('DVS','OptiTrack');

subplot(2,2,3); plot(timeP,rollP,style,timeO,rollO,style);
title('Roll','FontSize',12,'FontWeight','bold');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);
legend('DVS','OptiTrack');

h = gcf;
saveas(h,[outputPrefixName '_rotation'],'fig');
