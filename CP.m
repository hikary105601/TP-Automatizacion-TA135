close all;

% Traigo transferencia de la planta completa
%           72.768                        9.8
%  ------------------------ . ----------------------------
%    (s^2 + 18.29s + 116)     (s+0.692) (s+(19.42-157.1i))
  
s = tf('s');
Psb = (72.768)/(s^2 + 18.29*s + 116);
Pbc = -15.48/((s+0.692)*(s+19.42));

P = Psb*Pbc;

Kp = 5.0;
Ts = 0.02;

% Cargo datos de muestreo
if ~isfile('golpe_p.mat')
    error('No se encuentra el archivo golpe_p.mat. Obtenga datos de Simulink');
end

data = load('golpe_pd.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

p   = matriz_datos(1,:); % posicion (respecto equilibrio en 16cm)

num_muestras = size(matriz_datos,2);
tiempo = (0:num_muestras - 1)' * Ts;

if ~isfile('golpe_p_sim.mat')
    error('No se encuentra el archivo golpe_p_sim.mat. Obtenga datos de Simulink');
end

data = load('golpe_pd_sim.mat');
matriz_datos = squeeze(data.out.ScopeData{1}.Values.Data);

p_sim = matriz_datos(:,1); % posicion (error respecto equilibrio)

% Verificación adquisicion datos
figure;
plot(tiempo, p, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, p_sim, 'LineWidth', 1.5, 'Color', 'b');
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador PD - Impulso');
legend('real','simulada');
grid on;
hold off;



%% Escalón de ref (posición)
if ~isfile('escalon_p.mat')
    error('No se encuentra el archivo escalon_p.mat. Obtenga datos de Simulink');
end

data = load('escalon_pd.mat');
matriz_datos = squeeze(data.out.Prueba{1}.Values.Data);

p   = matriz_datos(1,:); % posicion (error respecto equilibrio)
r   = matriz_datos(2,:); % referencia

num_muestras = size(matriz_datos,2);
tiempo = (0:num_muestras - 1)' * Ts; 

if ~isfile('escalon_p_sim.mat')
    error('No se encuentra el archivo escalon_p_sim.mat. Obtenga datos de Simulink');
end

data = load('escalon_pd_sim.mat');
matriz_datos = squeeze(data.out.ScopeData{1}.Values.Data);

p_sim   = matriz_datos(:,1); % posicion (error respecto equilibrio)

% 2. Verificación adquisicion datos
figure;
plot(tiempo, p, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, p_sim, 'LineWidth', 1.5, 'Color', 'b'); 
xlabel('Tiempo [s]');
ylabel('Magnitud');
title('Controlador PD - Escalón');
legend('real', 'simulada');
grid on;
hold off;