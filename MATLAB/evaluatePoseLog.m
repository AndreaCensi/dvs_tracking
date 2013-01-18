function ret = evaluatePoseLog(filename)

P = importdata(filename);
P_t = P(:,1:3);
P_r = P(:,4:6);

plot3(P_t(:,1),P_t(:,2),P_t(:,3));
figure();
plot3(P_r(:,1),P_r(:,2),P_r(:,3));
