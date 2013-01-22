function ret = evaluatePoseLog(filename)

P = importdata(filename);

%indices = find(abs(P(:,3)) < 0.01 );    %remove misdetections
%P(indices,:) = []; 

T = P(:,1:3);
V_r = P(:,4:6);
led0 = P(:,7:8);
led1 = P(:,9:10);
led2 = P(:,11:12);
led3 = P(:,13:14);

time = P(:,15);

V_r = radtodeg(V_r);

rx = V_r(:,1);
ry = V_r(:,2);
rz = V_r(:,3);


subplot(2,2,1); plot3(T(:,1),-T(:,2),T(:,3));
title('Translation [m]');
xlabel('x');
ylabel('y');
zlabel('z');
axis equal;

subplot(2,2,2); plot(time,rx);
title('Pitch');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);

subplot(2,2,3); plot(time,ry);
title('Yaw');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);

subplot(2,2,4); plot(time,rz);
title('Roll');
xlabel('Time [s]');
ylabel('Degree');
ylim([-200 200]);

figure;

plot(led0(:,1),led0(:,2),led1(:,1),led1(:,2),led2(:,1),led2(:,2),led3(:,1),led3(:,2));
title('LED 0');
xlabel('x');
ylabel('y');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

figure;

subplot(2,2,1); plot(led0(:,1),led0(:,2));
title('LED Tracks');
xlabel('x');
ylabel('y');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,2); plot(led1(:,1),led1(:,2));
title('LED 1');
xlabel('x');
ylabel('y');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,3); plot(led2(:,1),led2(:,2));
title('LED 2');
xlabel('x');
ylabel('y');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,4); plot(led3(:,1),led3(:,2));
title('LED 3');
xlabel('x');
ylabel('y');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

