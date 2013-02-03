function x = findRotationShift(x0, R1,R2)

fun = @(x0,R1,R2) sum(abs( R1-(R2+x0) ));

options = optimset('Algorithm', 'levenberg-marquardt', 'MaxFunEvals', 1000, 'MaxIter', 1000);

x = lsqnonlin(@(x0) fun(x0,R1,R2),x0,[],[],options);