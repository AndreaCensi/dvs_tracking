function x = findTransformation(x0,T1,T2)

T1 = T1';

fun = @(x0,T1,T2) sum(sqrt(sum( (T1 - ( quatrotate(x0(1:4,1)',T2)' + repmat(x0(5:7,1),1,size(T2,1)))).^2 )));

options = optimset('Algorithm', 'levenberg-marquardt', 'MaxFunEvals', 10000, 'MaxIter', 10000);

x = lsqnonlin(@(x0) fun(x0,T1,T2),x0,[],[],options);

