

fig3d = figure; axis equal;

X = OBJ_points.vertices';

colors = ['r' 'g' 'b' 'y' 'm' 'c' 'k'];
for i = 1:size(X,2);
   hold on; scatter3(X(1,i),X(2,i),X(3,i), [colors(mod(i,length(colors))+1) 'o']); 
   text(X(1,i)+0.3,X(2,i)+0.3,X(3,i)+0.3, num2str(i));
   hold off;
end

%%

figure;
Img=imread('../me_cv.jpg');
imshow(Img);

x = ginput(size(X,2));
x = x';
save('test_osobow_me_jpg_repere_points.mat','x');

%%
%anchor_point_idx = 1:size(X,2);
anchor_point_idx = [1 3 14 15 13 16 5 6 7 12 23 24];

f = 450;
cx = floor(size(Img,2)/2);
cy = floor(size(Img,1)/2);
K = [f 0 cx; ...
    0 f cy; ...
    0 0 1];

Rt = exterior(K,X(:,anchor_point_idx),x(:,anchor_point_idx),'fiore');
Rt = lowe(K,X(:,anchor_point_idx),x(:,anchor_point_idx),Rt);
xt = cameraProjection(X,K,Rt(:,1:3),Rt(:,4));
figure;
imshow(Img);
hold on; 
for i=1:size(x,2)
    text(x(1,i),x(2,i),num2str(i),'color','r'); 
end
hold off;

figure;
imshow(Img);
hold on; 
for i=1:size(xt,2)
    text(xt(1,i),xt(2,i),num2str(i),'color','m'); 
end
hold off;

cost = sqrt(sum((x(1:2,anchor_point_idx) - xt(1:2,anchor_point_idx)).^2,1))' ;

res = sum(cost.^2)
iter = 0;
res1 = 0;
res2 = 10;
r_skew_sym  = logm(Rt(1:3,1:3));
r = [ r_skew_sym(3,2); r_skew_sym(1,3); r_skew_sym(2,1) ];
Pose = [r; Rt(:,4)]; 
min_options = optimset('LevenbergMarquardt', 'on', 'MaxFunEvals', 1000, 'MaxIter', 1000);

while(iter < 250 && abs(res1-res2)>eps)
    iter = iter + 1
%[f,res1] = lsqnonlin(@(var) cost_function(var,Pose,X(:,anchor_point_idx),x(:,anchor_point_idx),cx,cy), f, [], [], min_options);
[Pose,res2] = lsqnonlin(@(var) cost_function(f,var,X(:,anchor_point_idx),x(:,anchor_point_idx),cx,cy), Pose, [], [], min_options);
end

res2

K = [f 0 cx; ...
     0 f cy; ...
     0 0 1  ];

R = expm(skew_symmetric(Pose(1:3)));
t = Pose(4:6);
xt = cameraProjection(X,K,R,t);
reprojFig = figure;
figure(reprojFig);
imshow(Img);

%feature_point_idx = 1:size(X,2);
feature_point_idx = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17];

hold on; 
for i=1:length(feature_point_idx)
    text(xt(1,feature_point_idx(i))+4,xt(2,feature_point_idx(i))+4,num2str(feature_point_idx(i)),'color','y');
    scatter(xt(1,feature_point_idx(i)),xt(2,feature_point_idx(i)),'yo'); 
end
hold off;
%%
figure(fig3d)
hold on;
C = -R'*t;
    M = R' + [C,C,C];
    plot3([C(1) M(1,3)], [C(2) M(2,3)], [C(3) M(3,3)], 'color', 'b', 'LineWidth', 2);
    plot3([C(1) M(1,1)], [C(2) M(2,1)], [C(3) M(3,1)], 'color', 'r', 'LineWidth', 2);
    plot3([C(1) M(1,2)], [C(2) M(2,2)], [C(3) M(3,2)], 'color', 'g', 'LineWidth', 2);
hold off;

%%

X = OBJ_mask.vertices';

xh = cameraProjection(X,K,R,t);
figure(reprojFig);
hold on;
scatter(xh(1,anchor_point_idx),xh(2,anchor_point_idx),'g+');
hold off;

%%

feature_point_idx = 1:24;
dists = sqrt(sum((x(:,feature_point_idx) - xt(1:2,feature_point_idx)).^2,1))';
squared_error = (dists.^2);

mask_height = (max(x(2,feature_point_idx))-min(x(2,feature_point_idx)))
mean_squared_error = sum(squared_error)/size(dists,1)
rmse = sqrt(mean_squared_error)
nrmse = rmse / mask_height

[v,i] = sort(dists);
 