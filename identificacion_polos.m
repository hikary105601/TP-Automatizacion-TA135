if ~isfile('serial_data.txt')
    error('No se encuentra el archivo serial_data.txt. Ejecute primero captura.m');
end

matriz_datos = readmatrix('serial_data.txt');

Y_vector = matriz_datos(:, 1); % y_{n+1}
y_n      = matriz_datos(:, 2); % y_n
u_n      = matriz_datos(:, 3); % u_n   


% Gráfico
Ts = 0.02; % Tiempo de muestreo (MICROS_ENVIO en segundos -> 0.02s)
tiempo = (0:length(Y_vector)-1) * Ts;

figure('Name', 'Validación de Transitorios del Experimento', 'Color', 'w');
plot(tiempo, Y_vector, 'LineWidth', 1.5, 'Color', 'r'); hold on;
plot(tiempo, y_n, 'LineWidth', 1.5, 'Color', 'g'); hold on;
plot(tiempo, u_n, 'LineWidth', 1.5, 'Color', 'b'); hold on;
xlabel('Tiempo [s]');
ylabel('Ángulo [°]');
title('Respuesta a escalón');
legend('y_{n+1}', 'y_n', 'u_n');
grid on;
hold off;

% 3. Resolución por Cuadrados Mínimos (Clase 4)
% Modelo: y_{n+1} = c_y * y_n + c_u * u_n  -->  Y = X * alpha
Y = Y_vector;                  
X = [y_n, u_n];                

% Solución óptima: alpha = (X^T * X)^-1 * X^T * Y
alpha = (X' * X) \ (X' * Y);

cy = alpha(1);                 % Coeficiente de la salida anterior (polo discreto)
cu = alpha(2);                 % Coeficiente de la acción de control


% 4. Obtención de Polos y Conversión a Continuo
polo_discreto = cy;
polo_continuo = log(polo_discreto) / Ts;

fprintf('\n--- RESULTADOS DE IDENTIFICACIÓN ---\n');
fprintf('Polo discreto en Z: %.4f\n', polo_discreto);
fprintf('Polo continuo en S: %.4f\n', polo_continuo);