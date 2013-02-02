function cost = cost_function(f,Pose,X,x,cx,cy)

K = [f 0 cx; ...
    0 f cy; ...
    0 0 1];

R = expm(skew_symmetric(Pose(1:3)));
t = Pose(4:6);

xh = cameraProjection(X,K,R,t);
xh = xh(1:2,:);
   
cost = sqrt(sum((x-xh).^2,1))' ;

end