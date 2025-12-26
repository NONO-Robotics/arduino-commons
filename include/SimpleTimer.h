#include <Arduino.h>
#include <functional>

/**
 * @brief Clase para ejecutar una función (closure/lambda) periódicamente
 * dentro del loop principal, sin usar hilos de FreeRTOS.
 */
class SimpleTimer {
public:
    using TaskFunction = std::function<void()>;

    /**
     * @brief Constructor del temporizador.
     * @param msInterval El tiempo en milisegundos para el intervalo de ejecución.
     * @param func La función (o lambda) que se ejecutará periódicamente.
     */
    SimpleTimer(uint32_t msInterval, TaskFunction func)
        : interval_ms(msInterval), taskFunc(func), last_execution_ms(0) {
        // Inicializa el tiempo de la última ejecución al crear el objeto.
    }

    /**
     * @brief Este método debe ser llamado en el loop() principal de Arduino.
     * Verifica si ha pasado el tiempo necesario y, de ser así, ejecuta la función.
     */
    void update() {
        uint32_t current_ms = millis();
        
        // La condición usa resta para manejar correctamente el desbordamiento de millis() (rollover).
        if (current_ms - last_execution_ms >= interval_ms) {
            
            // 1. Actualizar el tiempo de la última ejecución ANTES de correr la función.
            //    Esto asegura que el intervalo se base en el tiempo real, no en el tiempo
            //    que toma la ejecución del código.
            last_execution_ms = current_ms; 
            
            // 2. Ejecutar el closure/lambda
            if (taskFunc) {
                taskFunc();
            }
        }
    }

private:
    uint32_t interval_ms;
    TaskFunction taskFunc;
    uint32_t last_execution_ms; // Almacena el tiempo de la última ejecución exitosa
};