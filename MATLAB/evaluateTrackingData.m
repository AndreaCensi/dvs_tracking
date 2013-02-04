function res = evaluateTrackingData()

flip_nr = [3 4 5 7 8];
flip_t_offsets = [0.58 0.23 -0.32 -0.84 0.58];

pose_err = [];
roll_err = [];
pitch_err = [];
yaw_err = [];

for i = 1:length(flip_nr)
    s_num = num2str(flip_nr(i));
    dvsData = ['pose_log_flip_' s_num '.txt'];
    optiTrackData = ['optitrack_ptam_data_flip' s_num '_drone.txt'];
    offset = flip_t_offsets(i);
    outputPrefix = ['flip_' s_num];
    
    [p_err,ro_err,pi_err,ya_err] = poseAccuracyComp(dvsData,optiTrackData,offset,outputPrefix);
    pose_err = [pose_err; p_err];
    roll_err = [roll_err;  ro_err];
    pitch_err = [pitch_err;  pi_err];
    yaw_err = [yaw_err;  ya_err];
end;

mean_pose_err = mean(pose_err);
sd_pose_err = std(pose_err);

mean_yaw_err = mean(yaw_err);
sd_yaw_err = std(yaw_err);

mean_pitch_err = mean(pitch_err);
sd_pitch_err = std(pitch_err);

mean_roll_err = mean(roll_err);
sd_roll_err = std(roll_err);

res = [mean_pose_err sd_pose_err;
    mean_roll_err sd_roll_err;
    mean_pitch_err sd_pitch_err;
    mean_yaw_err sd_yaw_err];

%plot translation error
figure;

boxplot(pose_err);
title('DVS pose estimation error','FontSize',12,'FontWeight','bold');
ylabel('Distance [m]','Rotation',90);

h = gcf;
saveas(h,'pose_error_box','fig');

%plot rotation errors
figure;

boxplot([yaw_err pitch_err roll_err],'labels',{'Yaw ','Pitch ' ,'Roll'});
title('DVS rotation error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'rot_error_box','fig');