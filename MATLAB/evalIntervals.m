function res = evalIntervals(filename)

I = importdata(filename);
dvs = I(:,1:2);
ptam = I(:,3:4);
flip = I(:,5:6);

I_dvs = dvs(:,2) - dvs(:,1);
I_ptam = ptam(:,2) - ptam(:,1);
I_flip = flip(:,2) - flip(:,1);

mean_dvs_start = mean(dvs(:,1) - flip(:,1));
mean_ptam_start = mean(ptam(:,1) - flip(:,1));

mean_dvs_I = mean(I_dvs);
mean_ptam_I = mean(I_ptam);
mean_flip_I = mean(I_flip);

%plot
subplot(2,1,1);
boxplot([I_dvs I_ptam I_flip],'labels',{'Tracking lost DVS ','Tracking lost PTAM ' ,'Flip time'});
title('Tracking downtime');
ylabel('Time [s]','Rotation',90);

subplot(2,1,2);

time_data = [0 mean_flip_I;
    mean_ptam_start mean_ptam_I;
    mean_dvs_start mean_dvs_I];

h = barh(time_data,'stack','b');
title('Mean tracking downtime comparison');
xlabel('Time [s]');
set(h(1), 'facecolor', 'none', 'EdgeColor', 'none'); % disable the color of the first column (init time)
set(gca, 'YTickLabel', {'Flip duration', 'PTAM', 'DVS'}); % change the y axis tick to your name of the process
