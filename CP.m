close all;
% Este archivo solo grafica en una misma figura las mediciones guardadas en .mat

% Transferencia de la planta completa
%           72.768               -16.82
%  ------------------------ . -------------
%    (s^2 + 18.29s + 116)      s (s+2.572)


Ts = 0.02;

% Respuesta al impulso
data1 = load('impulse_pi.mat');
matriz_datos1 = squeeze(data1.out.Prueba{1}.Values.Data);

p1   = matriz_datos1(:,1); % posicion real

num_muestras1 = size(matriz_datos1,1);
tiempo1 = (0:num_muestras1 - 1)' * Ts;

data1 = load('impulse_p_sim.mat');
matriz_datos1 = squeeze(data1.out.ScopeData{1}.Values.Data);

p_sim1 = matriz_datos1(:,1); % posicion simulación

figure;
plot(tiempo1, p1, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo1, p_sim1, 'LineWidth', 1.5, 'Color', 'b');
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador P - Impulso');
legend('real','simulada');
grid on;
hold off;

%% Escalón de ref (posición)
data2 = load('step_p.mat');
matriz_datos2 = squeeze(data2.out.Prueba{1}.Values.Data);

p2   = matriz_datos2(:,1); % posicion real

num_muestras2 = size(matriz_datos2,1);
tiempo2 = (0:num_muestras2 - 1)' * Ts; 

data2 = load('step_p_sim.mat');
matriz_datos2 = squeeze(data2.out.ScopeData{1}.Values.Data);

p_sim2   = matriz_datos2(1:end,1); % posicion simulación

figure;
plot(tiempo1, p1, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo1, p_sim1, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador P - Escalón');
legend('real', 'simulada');
grid on;
hold off;