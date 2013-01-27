function ret = evaluateRosbagPose(filename)

P = importdata(filename);

T = P(:,1:3);
Q = P(:,4:7);

time = P(:,8);
time = time - time(1);

[yaw,pitch,roll] = quat2angle(Q);

yaw = radtodeg(yaw);
pitch = radtodeg(pitch);
roll = radtodeg(roll);

subplot(2,2,1); plot3(T(:,1),T(:,2),T(:,3));
title('Translation [m]');
xlabel('x');
ylabel('y');
zlabel('z');
axis equal;

subplot(2,2,2); plot(time,pitch);
title('Pitch');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);

subplot(2,2,3); plot(time,roll);
title('Roll');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);

subplot(2,2,4); plot(time,yaw);
title('Yaw');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);