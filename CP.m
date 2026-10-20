close all;

% Traigo transferencia de la planta completa
%           72.768                        9.8
%  ------------------------ . ----------------------------
%    (s^2 + 18.29s + 116)     (s+0.692) (s+(19.42-157.1i))
  
  

s = tf('s');
Psb = (72.768)/(s^2 + 18.29*s + 116);
%Pbc = -15.48/((s+0.692)*(s+(19.42-157.1i)));
Pbc = -15.48/((s+0.692)*(s+19.42));

P = Psb*Pbc;

if ~isfile('golpe_p.mat')
    error('No se encuentra el archivo golpe_p.mat. Obtenga datos de Simulink');
end

data = load('golpe_p.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

d   = matriz_datos(1,:); % distancia desde sonar
e   = matriz_datos(2,:); % posicion (error respecto equilibrio en 16cm)

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras = size(matriz_datos,2);
tiempo = (0:num_muestras - 1)' * Ts; 

% 2. Verificación adquisicion datos
figure;
plot(tiempo, d, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, e, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador Proporcional - Rta a Impulso');
legend('distancia', 'error');
grid on;
hold off;

impulsos = [1.26]; % tiempo de cada golpe
amp_impulsos = [3]; 

u = zeros(size(tiempo)); % impulsos en el tiempo
for k = 1:length(impulsos)
    [~, idx] = min(abs(tiempo - impulsos(k))); %instante más cercano al impulso
    u(idx) = amp_impulsos(k) / Ts; % amplitud normalizada
end


Kp = 5;
G_sim = feedback(-Kp*P, 1);
y_teorica = lsim(G_sim, u, tiempo);

figure;
% Datos experimentales
plot(tiempo, e, 'b', 'LineWidth', 1.5, 'DisplayName', 'posición real'); hold on;

% Respuesta teórica
plot(tiempo, y_teorica, 'r', 'LineWidth', 1.8, 'DisplayName', 'posición teórica');

xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: golpe al carrito e impulso');
legend('Location', 'best');
grid on;
hold off;

%% Escalón de ref (posición)
if ~isfile('escalon_p.mat')
    error('No se encuentra el archivo escalon_p.mat. Obtenga datos de Simulink');
end

data = load('escalon_p.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

p   = matriz_datos(1,:); % posicion (error respecto equilibrio)
r   = matriz_datos(2,:); % referencia
u   = matriz_datos(3,:); % referencia

num_muestras = size(matriz_datos,2);
tiempo = (0:num_muestras - 1)' * Ts; 

% 2. Verificación adquisicion datos
figure;
plot(tiempo, p, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, r, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador Proporcional - Rta a Escalón');
legend('posición', 'referencia');
grid on;
hold off;

y_teorica = lsim(G_sim, u, tiempo);

figure;
% Datos experimentales
plot(tiempo, p, 'b', 'LineWidth', 1.5, 'DisplayName', 'posición real'); hold on;

% Respuesta teórica
plot(tiempo, y_teorica, 'r', 'LineWidth', 1.8, 'DisplayName', 'posición teórica');

plot(tiempo, r, 'g', 'LineWidth', 1.5, 'DisplayName', 'referencia');

xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: golpe al carrito e impulso');
legend('Location', 'best');
grid on;
hold off;