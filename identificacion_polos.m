close all;
if ~isfile('identificacion2.mat')
    error('No se encuentra el archivo datos_identificacion.mat. Obtenga datos de Simulink');
end

data = load('identificacion2.mat');
matriz_datos = squeeze(data.out.Prueba2{1}.Values.Data);

y_n   = matriz_datos(1,:); % ángulo barra actual
y_n_1 = matriz_datos(2,:); % y_{n-1} angulo previo
y_n_2 = matriz_datos(3,:); % y_{n-2}
u_n   = matriz_datos(4,:); % ángulo comandado a servo actual

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras = size(matriz_datos,2); % cantidad de filas (500)
tiempo = (0:num_muestras - 1)' * Ts; 

% 2. Visualización de los transitorios (Gráfica de verificación)
figure('Name', 'Validación de Transitorios - Segundo Orden', 'Color', 'w');
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, y_n_1, 'LineWidth', 1.5, 'Color', 'g'); 
plot(tiempo, y_n_2, 'LineWidth', 1.5, 'Color', 'm'); 
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Respuesta a escalón - Sistema de Segundo Orden');
legend('y_n', 'y_{n-1}', 'y_{n-2}', 'u_n');
grid on;
hold off;

% 3. Resolución por Cuadrados Mínimos (Modelo de Segundo Orden)
% Ecuación en diferencias: y_n = c_{y1}*y_{n-1} + c_{y2}*y_{n-2} + c_u*u_n  -->  Y = X * alpha
Y = y_n(:);                
X = [y_n_1(:), y_n_2(:), u_n(:)];               

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

%======== Ahora sí, comparo planta ========
s = tf('s');

p1 = polos_continuos(1);
p2 = polos_continuos(2);

k_planta = (cu*p1*p2)/(1-cy1-cy2);
Planta = k_planta/((s-p1)*(s-p2));
sys = tf(Planta);

[y_simulado, ~] = lsim(sys, u_n, tiempo);

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

% ==== Control digital ====
% Ts = 0.02 --> omega_s = 314 rad/s
% Ogata pg 171 respuesta de sistemas de segundo orden
% la frecuencia de gain crossover es del mismo orden que la frecuencia natural
k = db2mag(28.9);
Controlador = (k*(s+11)^2)/(s*(s+200));
L = sys*Controlador;
figure;bode(L);

C_z = c2d(Controlador, Ts, 'tustin');

S=1/(1+L);
T=1-S; 
PS=minreal(sys*S);
CS=minreal(Controlador*S);

%figure; bode(S,T);
%si hacemos bode(S) leemos peak de 0.381dB en 80.4rad/seg
%calculamos el sm como:
sm=1/(db2mag(80.4));