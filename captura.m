arduino = serialport("COM3", 115200);
configureTerminator(arduino, "LF");
flush(arduino); % Limpiamos cualquier basura acumulada en el puerto al iniciar

fprintf('Iniciando captura y sincronización de datos binarios...\n');

num_muestras = 400;
matriz_datos = zeros(num_muestras, 3);

i = 1;
while i <= num_muestras
    % Buscamos activamente la cabecera "abcd" byte a byte para evitar desajustes
    b1 = read(arduino, 1, "uint8");
    if b1 == 'a'
        b2 = read(arduino, 1, "uint8");
        if b2 == 'b'
            b3 = read(arduino, 1, "uint8");
            if b3 == 'c'
                b4 = read(arduino, 1, "uint8");
                if b4 == 'd'
                    % ¡Cabecera encontrada y sincronizada! 
                    % Leemos de forma segura los 3 floats [Y2, Y1, U1]
                    packet = read(arduino, 3, "single"); 
                    matriz_datos(i, :) = packet;
                    i = i + 1;
                end
            end
        end
    end
end

clear arduino;

writematrix(matriz_datos, 'serial_data.txt');
fprintf('Captura finalizada y guardada en serial_data.txt con éxito.\n');