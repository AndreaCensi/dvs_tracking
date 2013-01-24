%Rot: rodrigues rotation
%Trans: translation vector
%time: timestamp
function M = trajVis(Rot, Trans, time)

P = zeros(length(time),4,2);

L1 = [-0.1 -0.1 0 1]';
L2 = [-0.1 0.1 0 1]';
L3 = [0.1 -0.1 0 1]';
L4 = [0.1 0.1 0 1]';

P1 =  zeros(length(time),4);
P2 =  zeros(length(time),4);
P3 =  zeros(length(time),4);
P4 =  zeros(length(time),4);

for i = 1:length(time)
    R = rodrigues(Rot(i,:));
    T = Trans(i,:)';
    RT = [R T ; 0 0 0 1];
    
    P1(i,:) = RT*L1;
    P2(i,:) = RT*L2;
    P3(i,:) = RT*L3;
    P4(i,:) = RT*L4;
end;

%plot3(P1(:,1),P1(:,2),P1(:,3),P2(:,1),P2(:,2),P2(:,3),P3(:,1),P3(:,2),P3(:,3),P4(:,1),P4(:,2),P4(:,3));

lastTS = -2;
for i = 1:length(time)
    %plot3(P1(1:i,1),P1(1:i,2),P1(1:i,3),P2(1:i,1),P2(1:i,2),P2(1:i,3),P3(1:i,1),P3(1:i,2),P3(1:i,3),P4(1:i,1),P4(1:i,2),P4(1:i,3));
    if time(i)-lastTS > 0.1
        hold on;
        Quad = [P1(i,:);P2(i,:);P4(i,:);P3(i,:);P1(i,:)];
        plot3(Quad(:,1),Quad(:,2),Quad(:,3));
        lastTS = time(i);
    end;
    %M(i) = getframe;
    %clf();
end;

title('Trajectory [m]');
xlim([-1 1]);
ylim([-1 1]);
zlim([0 2]);
xlabel('x');
ylabel('y');
zlabel('z');
set(gca,'XDir','reverse');


h = gcf;
saveas(h,'poseplot_traj','fig');