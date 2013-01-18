function ret = evaluatePoseLog(filename)

P = importdata(filename);
T = P(:,1:3)./100;
V_r = P(:,4:6);

V_r = radtodeg(V_r);

yaw = V_r(:,1);
pitch = V_r(:,2);
roll = V_r(:,3);

subplot(2,2,1); plot3(T(:,1),T(:,2),T(:,3));
title('Translation [m]');
xlabel('x');
ylabel('y');
zlabel('z');
axis equal;

subplot(2,2,2); plot(yaw);
title('Yaw');
xlabel('Eventpacket');
ylabel('Degree');
ylim([-190 190]);

subplot(2,2,3); plot(pitch);
title('Pitch');
xlabel('Eventpacket');
ylabel('Degree');
ylim([-190 190]);

subplot(2,2,4); plot(roll);
title('Roll');
xlabel('Eventpacket');
ylabel('Degree');
ylim([-190 190]);
