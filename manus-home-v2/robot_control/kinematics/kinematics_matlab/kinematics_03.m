
global L_1
global L_2
global L_3
global L_4
global d
% % Define joint angle lengths
L_1 = 14;
L_2 = 16;
L_3 = 14;
L_4 = 16;
d = 3;

n = 5;
theta_1_vector = linspace(pi/2, (3/4)*pi, n)
theta_3_vector = linspace(pi/4, pi/2, n)

XE_vector = zeros(1,n);
YE_vector = zeros(1,n);

for i = 1:n
    [XE,YE] = forwardKinematics(theta_1_vector(i),theta_3_vector(i));
    XE_vector(i) = XE;
    YE_vector(i) = YE;
end

XE_vector
YE_vector


theta_1_vector2 = zeros(1,n);
theta_3_vector2 = zeros(1,n);
for i = 1:n
    [theta_1,theta_3] = inverseKinematics(XE_vector(i),YE_vector(i));
    theta_1_vector2(i) = theta_1;
    theta_3_vector2(i) = theta_3;
end

theta_1_vector2
theta_3_vector2

% Test if the theta values match
epsilon = 0.0001
theta_1_correct = theta_1_vector - theta_1_vector2 < epsilon
theta_3_correct = theta_3_vector - theta_3_vector2 < epsilon

% Defines the forward kinematics
function [XE,YE] = forwardKinematics(theta_1,theta_3)
    global L_1
    global L_2
    global L_3
    global L_4
    global d


    % Define forward kinematics
    x_b = L_1*cos(theta_1) - d;
    y_b = L_1*sin(theta_1);
    x_d = L_3*cos(theta_3) + d;
    y_d = L_3*sin(theta_3);

    h = sqrt((y_d - y_b)^2 + (x_d - x_b)^2);
    

    delta = acos((L_2^2 + h^2 - L_4^2) / (2*L_2*h));
    %delta = acos(h/(2*L_2));
    gamma = atan2( (y_d - y_b) , (x_d - x_b) );

    theta_2 = delta + gamma;

    % Define the X and Y coordinates of the end-effector as a function of the joint angles (?1?,?3).
    XE = x_b + L_2*cos(theta_2);
    YE = y_b + L_2*sin(theta_2);
end


% Defines the inverse kinematics
function [theta_1,theta_3] = inverseKinematics(XE, YE)
    global L_1;
    global L_2;
    global L_3;
    global L_4;
    global d;

    theta_1 =  atan2(YE,XE+d) + acos((-L_2^2 + L_1^2 + (XE+d)^2 + YE^2) / (2*L_1*sqrt((XE+d)^2 + YE^2)) );
	theta_3 = atan2(YE,XE-d) - acos(((XE-d)^2 + YE^2 + L_3^2 - L_4^2) / (2*L_3*sqrt((XE - d)^2 + YE^2)));
end

