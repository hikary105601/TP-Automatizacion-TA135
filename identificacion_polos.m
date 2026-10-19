close all;
if ~isfile('identificacion2.mat')
    error('No se encuentra el archivo datos_identificacion.mat. Obtenga datos de Simulink');
end

data = load('identificacion2.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

y_n   = matriz_datos(1,:); % ángulo barra actual
u_n   = matriz_datos(2,:); % ángulo comandado a servo actual

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras = size(matriz_datos,2); % 500 muestras
tiempo = (0:num_muestras - 1)' * Ts; 

% 2. Verificación adquisicion datos
figure('Name', 'Validación de Transitorios - Segundo Orden', 'Color', 'w');
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Respuesta a escalón - Sistema de Segundo Orden');
legend('y_n', 'u_n');
grid on;
hold off;

% 3. Resolución por Cuadrados Mínimos (Modelo de Segundo Orden)
% Ecuación en diferencias: y_n = c_{y1}*y_{n-1} + c_{y2}*y_{n-2} + c_u*u_n  -->  Y = X * alpha
Y    = y_n(:, 3:end)'; % ignoro primeras dos muestras hasta n=0
y_n1 = y_n(:, 2:end-1)'; % instante -1
y_n2 = y_n(:, 1:end-2)';% instante -2
u_n_recortado  = u_n(:, 3:end)'; 
X = [y_n1, y_n2, u_n_recortado];               

alpha = (X' * X) \ (X' * Y); % (X^T * X)^-1 * X^T * Y

cy1 = alpha(1); % -(p1+p2)/(p1*p2)
cy2 = alpha(2); % -1/(p1*p2)
cu  = alpha(3); % k/(p1*p2)

% Los polos discretos son las raíces del polinomio característico: z^2 - cy1*z - cy2 = 0
polos_discretos = roots([1, -cy1, -cy2]);

% Conversión a tiempo continuo mediante s = ln(z) / Ts
polos_continuos = log(polos_discretos) / Ts;

fprintf('Coeficientes estimados:\n');
fprintf('cy1 = %.4f, cy2 = %.4f, cu = %.4f\n\n', cy1, cy2, cu);

fprintf('Polos discretos en Z:\n');
disp(polos_discretos);

fprintf('Polos continuos en S:\n');
disp(polos_continuos);

%======== Comparo planta real con su transferencia ========
s = tf('s');

p1 = polos_continuos(1);
p2 = polos_continuos(2);

k_planta = (cu*p1*p2)/(1-cy1-cy2);
Planta = k_planta/((s-p1)*(s-p2));
sys = tf(Planta); % sistema servo-barra

[y_simulado, ~] = lsim(sys, u_n, tiempo');

figure('Name', 'Validación del Modelo Continuo Identificado', 'Color', 'w');
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); 
plot(tiempo, y_simulado, '--', 'LineWidth', 1.8, 'Color', [0 0.5 0]); % Curva simulada en verde punteado
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: Sistema Real vs Modelo Identificado');
legend('y_n (Real)', 'u_n (Entrada)', 'y_{simulado}', 'Location', 'Best');
grid on;
hold off;

% == Planta Completa ==
sys_completo = sys * (9.81/s^2); % agrego transferencia del carrito

sys_pade = sys_completo*((1-0.04*s)/(1+0.04*s)); % agrego retardo

[y_sin_retardo, ~] = lsim(sys_completo, u_n, tiempo');
[y_pade, ~] = lsim(sys_pade, u_n, tiempo');

figure('Name', 'Respuesta de sistema completo', 'Color', 'w');
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); hold on;
plot(tiempo, y_sin_retardo, 'LineWidth', 1.5, 'Color', 'r');
plot(tiempo, y_pade, '--', 'LineWidth', 1.8, 'Color', 'g');
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: Sistema Completo vs Con Retardo');
legend('u (entrada)','y sin retardo', 'y pade');
grid on;
hold off;

% ==== Control digital ====
% Ts = 0.02 --> omega_s = 314 rad/s
% Ogata pg 171 respuesta de sistemas de segundo orden
% la frecuencia de gain crossover es del mismo orden que la frecuencia natural
rp1 = real(p1);
ip1 = imag(p1);
omega_natural = sqrt(rp1^2 + ip1^2);
Controlador = ((s+11)^2)/(s*(s+200));
L = sys*Controlador;
%figure;bode(L);title('L antes deajustar k');
k = db2mag(29.1); % ganancia en omega_natural=11
L = L*k;
figure;bode(L);title('L');

C_z = c2d(Controlador, Ts, 'tustin');

S=1/(1+L);
T=1-S; 
PS=minreal(sys*S);
CS=minreal(Controlador*S);

%figure; bode(S,T);
%si hacemos bode(S) leemos peak de 0.381dB en 80.4rad/seg
%calculamos el sm como:
sm=1/(db2mag(80.4));