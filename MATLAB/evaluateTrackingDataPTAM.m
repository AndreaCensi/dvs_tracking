function res = evaluateTrackingDataPTAM()

flip_nr = [3 4 5 7 8];
flip_t_offsets_ptam = [0.46 0.16 -0.48 -0.7 0.43];
flip_t_offsets_opti = [0.58 0.23 -0.32 -0.84 0.58];

ptam_trans_err = [];
ptam_roll_err = [];
ptam_pitch_err = [];
ptam_yaw_err = [];

for i = 1:length(flip_nr)
    s_num = num2str(flip_nr(i));
    ptamData = ['optitrack_ptam_data_flip' s_num '_ptam.txt'];
    optiTrackData = ['optitrack_ptam_data_flip' s_num '_drone.txt'];
    offset_opti = flip_t_offsets_opti(i);
    offset_ptam = flip_t_offsets_ptam(i);
    outputPrefix = ['ptam_flip_' s_num];
    
    [p_err,ro_err,pi_err,ya_err] = poseAccuracyPTAM(ptamData,optiTrackData,offset_ptam,offset_opti,outputPrefix);
    ptam_trans_err = [ptam_trans_err; p_err];
    ptam_roll_err = [ptam_roll_err;  ro_err];
    ptam_pitch_err = [ptam_pitch_err;  pi_err];
    ptam_yaw_err = [ptam_yaw_err;  ya_err];
end;

mean_ptam_trans_err = mean(ptam_trans_err);
sd_ptam_trans_err = std(ptam_trans_err);

mean_ptam_yaw_err = mean(ptam_yaw_err);
sd_ptam_yaw_err = std(ptam_yaw_err);

mean_ptam_pitch_err = mean(ptam_pitch_err);
sd_ptam_pitch_err = std(ptam_pitch_err);

mean_ptam_roll_err = mean(ptam_roll_err);
sd_ptam_roll_err = std(ptam_roll_err);

res = [mean_ptam_trans_err sd_ptam_trans_err;
    mean_ptam_roll_err sd_ptam_roll_err;
    mean_ptam_pitch_err sd_ptam_pitch_err;
    mean_ptam_yaw_err sd_ptam_yaw_err];

%plot translation error
figure;

boxplot(ptam_trans_err);
title('PTAM translation error','FontSize',12,'FontWeight','bold');
ylabel('Distance [m]','Rotation',90);

h = gcf;
saveas(h,'ptam_trans_error_box','fig');

%plot rotation errors
figure;

boxplot([ptam_yaw_err ptam_pitch_err ptam_roll_err],'labels',{'Yaw ','Pitch ' ,'Roll'});
title('PTAM rotation error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'ptam_rot_error_box','fig');