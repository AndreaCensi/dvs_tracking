function res = evaluateTrackingData()

flip_nr = [3 4 5 7 8];
flip_t_offsets = [0.58 0.23 -0.32 -0.84 0.58];

dvs_trans_err = [];
dvs_roll_err = [];
dvs_pitch_err = [];
dvs_yaw_err = [];

for i = 1:length(flip_nr)
    s_num = num2str(flip_nr(i));
    dvsData = ['pose_log_flip_' s_num '.txt'];
    optiTrackData = ['optitrack_ptam_data_flip' s_num '_drone.txt'];
    offset = flip_t_offsets(i);
    outputPrefix = ['flip_' s_num];
    
    [p_err,ro_err,pi_err,ya_err] = poseAccuracyComp(dvsData,optiTrackData,offset,outputPrefix);
    dvs_trans_err = [dvs_trans_err; p_err];
    dvs_roll_err = [dvs_roll_err;  ro_err];
    dvs_pitch_err = [dvs_pitch_err;  pi_err];
    dvs_yaw_err = [dvs_yaw_err;  ya_err];
end;

mean_dvs_trans_err = mean(dvs_trans_err);
sd_dvs_trans_err = std(dvs_trans_err);

mean_dvs_yaw_err = mean(dvs_yaw_err);
sd_dvs_yaw_err = std(dvs_yaw_err);

mean_dvs_pitch_err = mean(dvs_pitch_err);
sd_dvs_pitch_err = std(dvs_pitch_err);

mean_dvs_roll_err = mean(dvs_roll_err);
sd_dvs_roll_err = std(dvs_roll_err);

res = [mean_dvs_trans_err sd_dvs_trans_err;
    mean_dvs_roll_err sd_dvs_roll_err;
    mean_dvs_pitch_err sd_dvs_pitch_err;
    mean_dvs_yaw_err sd_dvs_yaw_err];

%plot translation error
figure;

boxplot(dvs_trans_err);
title('DVS translation error','FontSize',12,'FontWeight','bold');
ylabel('Distance [m]','Rotation',90);

h = gcf;
saveas(h,'dvs_trans_error_box','fig');

%plot rotation errors
figure;

boxplot([dvs_yaw_err dvs_pitch_err dvs_roll_err],'labels',{'Yaw ','Pitch ' ,'Roll'});
title('DVS rotation error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'dvs_rot_error_box','fig');