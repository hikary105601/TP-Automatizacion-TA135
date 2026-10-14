Ts = 0.02;

A = [0 1,
     -132.8 -20.92];
B = [0,
     82.663];
C = [1 0];
D = 0;

T = 0.02; % 50Hz

A_d = eye(2) + A*T;
B_d = B*T;
C_d = C;
D_d = D;

s1 =  -10.4595 + 4.8381i;
s2 = -10.4595 - 4.8381i;

zL1 = exp(5 * s1 * Ts);
zL2 = exp(3 * s2 * Ts);

L = acker(A_d', C_d', [zL1 zL2])'

%y_est = [1, 0]*x_est;
%x_est = A_d * x_est + L*(y - y_est);