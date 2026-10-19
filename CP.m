close all;

% Traigo transferencia de la planta completa
%           713.86
%  ------------------------
%  s^2 (s^2 + 18.29s + 116)

s = tf('s');
P = 713.86/(s*(s^2 + 18.29*s + 116)*(s+10));

if ~isfile('impulso_p.mat')
    error('No se encuentra el archivo impulso_p.mat. Obtenga datos de Simulink');
end

data = load('impulso_p.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

d   = matriz_datos(1,:); % distancia desde sonar
e   = matriz_datos(2,:); % error respecto equilibrio en 16cm

Ts = 0.02; % muestreo de 50 Hz (20 ms)
num_muestras = size(matriz_datos,2); % 500 muestras
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

impulsos = [0.9 2.76 4.52 5.8 7.62]; % tiempo de cada golpe
amp_impulsos = [1, 1, 1, 1, 1]; 

u = zeros(size(tiempo)); % impulsos en el tiempo
for k = 1:length(impulsos)
    [~, idx] = min(abs(tiempo - impulsos(k))); %instante más cercano al impulso
    u(idx) = amp_impulsos(k) / Ts; % amplitud normalizada
end


Kp = 5;
G_sim = feedback(Kp*P, 1);
y_teorica = lsim(G_sim, u, tiempo);

figure;
% Datos experimentales
plot(tiempo, e, 'b', 'LineWidth', 1.5, 'DisplayName', 'Error medido (e)'); hold on;

% Respuesta teórica
plot(tiempo, y_teorica, 'k--', 'LineWidth', 1.8, 'DisplayName', 'Rta. teórica');

xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Comparación: Datos Muestreados vs. Respuesta Teórica al Impulso');
legend('Location', 'best');
grid on;
hold off;