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

RPY = zeros(size(V_r));

[m n] = size(V_r);

for i = 1:m
    RPY(i,:) = rpyAng(rodrigues(V_r(i,:)));
end;

RPY = radtodeg(RPY);

rx = RPY(:,1);
ry = RPY(:,2);
rz = RPY(:,3);


subplot(2,2,1); plot3(T(:,1),T(:,2),T(:,3));
title('Translation [m]');
xlabel('x');
ylabel('y');
zlabel('z');
axis equal;
set(gca,'XDir','reverse');

subplot(2,2,2); plot(time,rx);
title('Pitch');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);

subplot(2,2,3); plot(time,ry);
title('Roll');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);

subplot(2,2,4); plot(time,rz);
title('Yaw');
xlabel('Time [s]');
ylabel('Degrees');
ylim([-200 200]);

h = gcf;
saveas(h,'poseplot_RT','fig');

figure;

plot3(led0(:,1),led0(:,2),time,led1(:,1),led1(:,2),time,led2(:,1),led2(:,2),time,led3(:,1),led3(:,2),time);
title('LED Tracks');
xlabel('x');
ylabel('y');
zlabel('Time [s]');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

h = gcf;
saveas(h,'poseplot_LED_tracks','fig');

figure;

subplot(2,2,1); plot3(led0(:,1),led0(:,2),time);
title('LED 0');
xlabel('x');
ylabel('y');
zlabel('Time [s]');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,2); plot3(led1(:,1),led1(:,2),time);
title('LED 1');
xlabel('x');
ylabel('y');
zlabel('Time [s]');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,3); plot3(led2(:,1),led2(:,2),time);
title('LED 2');
xlabel('x');
ylabel('y');
zlabel('Time [s]');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

subplot(2,2,4); plot3(led3(:,1),led3(:,2),time);
title('LED 3');
xlabel('x');
ylabel('y');
zlabel('Time [s]');
xlim([0 127]);
ylim([0 127]);
axis square;
set(gca,'XDir','reverse');

h = gcf;
saveas(h,'poseplot_LED_sep','fig');

figure;
trajVis(V_r,T,time);
%movie(M,10,60);

