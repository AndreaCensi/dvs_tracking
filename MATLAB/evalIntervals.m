function res = evalIntervals(filename)

I = importdata(filename);
dvs = I(:,1:2);
ptam = I(:,3:4);
flip = I(:,5:6);

I_dvs = dvs(:,2) - dvs(:,1);
I_ptam = ptam(:,2) - ptam(:,1);
I_flip = flip(:,2) - flip(:,1);

boxplot([I_dvs I_ptam I_flip],'labels',{'Tracking lost DVS ','Tracking lost PTAM ' ,'Flip time'});
title('Tracking downtime');
ylabel('Time [s]','Rotation',90);