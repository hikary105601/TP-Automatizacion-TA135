close all;
%% ======== Planta servo-barra ========
if ~isfile('servo-barra.mat')
    error('No se encuentra el archivo servo-barra.mat. Obtenga datos de Simulink');
end

data = load('servo-barra.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

y_n   = matriz_datos(1,:); % ángulo barra actual
u_n   = matriz_datos(2,:); % ángulo comandado a servo actual

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras = size(matriz_datos,2); % 500 muestras
tiempo = (0:num_muestras - 1)' * Ts; 

% 2. Verificación adquisicion datos
figure('Name', 'Validación de datos', 'Color', 'w');
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Respuesta a escalón');
legend('ang_barra', 'ang_servo');
grid on;
hold off;

% 3. Resolución por Cuadrados Mínimos (Modelo de Segundo Orden)
% Ecuación en diferencias: y_n = c_{y1}*y_{n-1} + c_{y2}*y_{n-2} + c_u*u_n  -->  Y = X * alpha
Y    = y_n(:, 3:end)'; % ignoro primeras dos muestras hasta n=0
y_n1 = y_n(:, 2:end-1)'; % instante -1
y_n2 = y_n(:, 1:end-2)';% instante -2
u_n2  = u_n(:, 3:end)'; 
X = [y_n1, y_n2, u_n2];               

alpha = (X' * X) \ (X' * Y); % (X^T * X)^-1 * X^T * Y

cy1 = alpha(1); % -(p1+p2)/(p1*p2)
cy2 = alpha(2); % -1/(p1*p2)
cu  = alpha(3); % k/(p1*p2)

% Los polos discretos son las raíces del polinomio característico: z^2 - cy1*z - cy2 = 0
polos_discretos = roots([1, -cy1, -cy2]);

% Conversión a tiempo continuo mediante s = ln(z) / Ts
polos_continuos = log(polos_discretos) / Ts;

fprintf('Coeficientes estimados para Psb:\n');
fprintf('cy1 = %.4f, cy2 = %.4f, cu = %.4f\n\n', cy1, cy2, cu);

fprintf('Polos discretos en Z de Psb:\n');
disp(polos_discretos);

fprintf('Polos continuos en S de Psb:\n');
disp(polos_continuos);

%======== Comparo planta real con su transferencia ========
s = tf('s');

p1 = polos_continuos(1);
p2 = polos_continuos(2);

k_planta = (cu*p1*p2)/(1-cy1-cy2);
Planta = k_planta/((s-p1)*(s-p2));
Psb = tf(Planta); % sistema servo-barra

[y_simulado, ~] = lsim(Psb, u_n, tiempo');

figure('Name', 'Validación del Modelo Continuo Identificado', 'Color', 'w');
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); 
plot(tiempo, y_simulado, '--', 'LineWidth', 1.8, 'Color', [0 0.5 0]); % Curva simulada en verde punteado
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: Sistema Real vs Modelo Identificado');
legend('ang_{barra}', 'ang_{servo}', 'y_{simulado}', 'Location', 'Best');
grid on;
hold off;

%% ======== Planta barra-carrito ========
if ~isfile('barra-carrito.mat')
    error('No se encuentra el archivo barra-carrito.mat. Obtenga datos de Simulink');
end

data = load('test_bc.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

y_bc = matriz_datos(1,:); % posición carrito (referencia-distancia)
u_bc = matriz_datos(2,:); % ángulo barra

y_bc = y_bc(1,200:end-135);
u_bc = u_bc(1,200:end-135);

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras_bc = size(y_bc(:));
tiempo_bc = (0:num_muestras_bc - 1)' * Ts; 

% 2. Verificación adquisicion datos
figure('Name', 'Validación datos', 'Color', 'w');
plot(tiempo_bc, y_bc, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo_bc, u_bc, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Respuesta a escalón');
legend('posición', 'ang_{barra}');
grid on;
hold off;

% 3. Resolución por Cuadrados Mínimos
% Ecuación en diferencias feed forward: y_k = y_{k-1}*c1 + y_{k-2}*c2 + u_{k-2}*cu
Y    = y_bc(:, 3:end)'; % ignoro primeras dos muestras
y_n1 = y_bc(:, 2:end-1)'; % instante -1
y_n2 = y_bc(:, 1:end-2)';% instante -2
u_n2= u_bc(:, 1:end-2)'; 
X = [y_n1, y_n2, u_n2];               

alpha = (X' * X) \ (X' * Y); % (X^T * X)^-1 * X^T * Y

cy1 = alpha(1); 
cy2 = alpha(2); 
cu  = alpha(3); 

% Los polos discretos son las raíces del polinomio característico: z^2 - cy1*z - cy2 = 0
polos_discretos = roots([1, -cy1, -cy2]);

% Conversión a tiempo continuo mediante s = ln(z) / Ts
polos_continuos = (polos_discretos-1)/Ts; %log(polos_discretos) / Ts;

fprintf('Coeficientes estimados Pbc:\n');
fprintf('cy1 = %.4f, cy2 = %.4f, cu = %.4f\n\n', cy1, cy2, cu);

fprintf('Polos discretos en Z Pbc:\n');
disp(polos_discretos);

fprintf('Polos continuos en S Pbc:\n');
disp(polos_continuos);

%%======== Comparo planta real con su transferencia ========
 s = tf('s');
 
% uno debería ser 0
 p1 = polos_continuos(1);
 p2 = polos_continuos(2);
 %p2 = 19.42;
 
 k_planta = (cu*p1*p2)/(1-cy1-cy2);
 Pbc = k_planta/((s-p1)*(s-p2));
 %Pbc = k_planta/((s+0.692)*(s+19.42));
 
[y_simulado_bc, ~] = lsim(Pbc, u_bc, tiempo_bc');
figure('Name', 'Validación del Modelo Continuo Identificado', 'Color', 'w');
plot(tiempo_bc, y_bc, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo_bc, u_bc, 'LineWidth', 1.5, 'Color', 'b'); 
plot(tiempo_bc, y_simulado_bc, '--', 'LineWidth', 1.8, 'Color', [0 0.5 0]); % Curva simulada en verde punteado
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: Sistema Real vs Modelo Identificado');
legend('posición real', 'ang_{barra}', 'y_{simulado}', 'Location', 'Best');
grid on;
hold off;
%% == Planta Completa ==
 sys_completo = Psb*Pbc;
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
