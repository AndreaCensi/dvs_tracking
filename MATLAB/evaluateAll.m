function res = evaluateAll()

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

dvs_trans_err = [];
dvs_roll_err = [];
dvs_pitch_err = [];
dvs_yaw_err = [];

for i = 1:length(flip_nr)
    s_num = num2str(flip_nr(i));
    dvsData = ['pose_log_flip_' s_num '.txt'];
    optiTrackData = ['optitrack_ptam_data_flip' s_num '_drone.txt'];
    offset = flip_t_offsets_opti(i);
    outputPrefix = ['flip_' s_num];
    
    [p_err,ro_err,pi_err,ya_err] = poseAccuracyComp(dvsData,optiTrackData,offset,outputPrefix);
    dvs_trans_err = [dvs_trans_err; p_err];
    dvs_roll_err = [dvs_roll_err;  ro_err];
    dvs_pitch_err = [dvs_pitch_err;  pi_err];
    dvs_yaw_err = [dvs_yaw_err;  ya_err];
end;

mean_ptam_trans_err = mean(ptam_trans_err);
sd_ptam_trans_err = std(ptam_trans_err);

mean_ptam_yaw_err = mean(ptam_yaw_err);
sd_ptam_yaw_err = std(ptam_yaw_err);

mean_ptam_pitch_err = mean(ptam_pitch_err);
sd_ptam_pitch_err = std(ptam_pitch_err);

mean_ptam_roll_err = mean(ptam_roll_err);
sd_ptam_roll_err = std(ptam_roll_err);

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
    mean_dvs_yaw_err sd_dvs_yaw_err;
    mean_ptam_trans_err sd_ptam_trans_err;
    mean_ptam_roll_err sd_ptam_roll_err;
    mean_ptam_pitch_err sd_ptam_pitch_err;
    mean_ptam_yaw_err sd_ptam_yaw_err];

%plot translation error
figure('position',[0 0 300 500]);

boxplot([dvs_trans_err; ptam_trans_err],[zeros(length(dvs_trans_err),1); ones(length(ptam_trans_err),1)],'labels',{'DVS','PTAM'});
title('Translation error','FontSize',12,'FontWeight','bold');
ylabel('Distance [m]','Rotation',90);

h = gcf;
saveas(h,'trans_error_box','fig');

%plot rotation errors
figure('position',[0 0 300 500]);

boxplot([dvs_roll_err; ptam_roll_err],[zeros(length(dvs_roll_err),1); ones(length(ptam_roll_err),1)],'labels',{'DVS','PTAM'});
title('Roll error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'roll_error_box','fig');

%plot rotation errors
figure('position',[0 0 300 500]);

boxplot([dvs_pitch_err; ptam_pitch_err],[zeros(length(dvs_pitch_err),1); ones(length(ptam_pitch_err),1)],'labels',{'DVS','PTAM'});
title('Pitch error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'pitch_error_box','fig');

%plot rotation errors
figure('position',[0 0 300 500]);

boxplot([dvs_yaw_err; ptam_yaw_err],[zeros(length(dvs_yaw_err),1); ones(length(ptam_yaw_err),1)],'labels',{'DVS','PTAM'});
title('Yaw error','FontSize',12,'FontWeight','bold');
ylabel('Degree','Rotation',90);

h = gcf;
saveas(h,'yaw_error_box','fig');