#include "Logger.h"


// ── Instancia global — constructor vacío, sin efectos secundarios ─────────────
Logger logger;

#ifdef USE_ROS_LOGGER
static rcl_publisher_t ros_log_publisher;
static std_msgs__msg__String ros_log_msg;
static bool ros_publisher_ready = false;
static char ros_msg_buffer[256]; // Buffer fijo para el mensaje
#endif

// ── begin() — llamar en setup() ───────────────────────────────────────────────
void Logger::begin(unsigned long baud, LogLevel level, LogOutput output)
{
    this->baud   = baud;
    this->level  = level;
    this->output = output;

    switch (output) {
        case OUTPUT_SERIAL:
            Serial.begin(baud);
            while (!Serial && millis() < 5000);
            break;

        case OUTPUT_SERIAL2:
            Serial2.begin(baud, SERIAL_8N1, 16, 17);
            while (!Serial2 && millis() < 5000);
            break;

        case OUTPUT_ROS:
            // No abre serial. Llamar initRosPublisher() después del nodo.
            break;
    }

    initialized = true;
}

// ── Configuración ─────────────────────────────────────────────────────────────
void Logger::setLevel(LogLevel level)    { this->level  = level; }
void Logger::setOutput(LogOutput output) { this->output = output; }

// ── Output interno ────────────────────────────────────────────────────────────
void Logger::printToOutput(const String& prefix, const String& msg)
{
    if (!initialized) return;  // silencioso si no se llamó begin()

    String full = prefix + msg;

    switch (output) {
        case OUTPUT_SERIAL:  Serial.println(full);  break;
        case OUTPUT_SERIAL2: Serial2.println(full); break;
        case OUTPUT_ROS:
#ifdef USE_ROS_LOGGER
            if (ros_publisher_ready) {
                // Copia al buffer fijo sin usar malloc
                size_t len = full.length();
                if (len > 255) len = 255;
                memcpy(ros_msg_buffer, full.c_str(), len);
                ros_msg_buffer[len] = '\0';
                
                ros_log_msg.data.data = ros_msg_buffer;
                ros_log_msg.data.size = len;
                ros_log_msg.data.capacity = 256;
                
                rcl_publish(&ros_log_publisher, &ros_log_msg, NULL);
            }
#endif
            break;
    }
}

// ── Log principal ─────────────────────────────────────────────────────────────
void Logger::log(LogLevel level, String msg)
{
    if (level < this->level) return;

    String prefix;
    switch (level) {
        case TRACE: prefix = "[TRACE] "; break;
        case DEBUG: prefix = "[DEBUG] "; break;
        case INFO:  prefix = "[INFO]  "; break;
        case WARN:  prefix = "[WARN]  "; break;
        case ERROR: prefix = "[ERROR] "; break;
        case FATAL: prefix = "[FATAL] "; break;
        case OFF:   return;
    }

    printToOutput(prefix, msg);
}

// ── Helpers de nivel ──────────────────────────────────────────────────────────
void Logger::trace(String msg) { log(TRACE, msg); }
void Logger::debug(String msg) { log(DEBUG, msg); }
void Logger::info(String msg)  { log(INFO,  msg); }
void Logger::warn(String msg)  { log(WARN,  msg); }
void Logger::error(String msg) { log(ERROR, msg); }
void Logger::fatal(String msg) { log(FATAL, msg); }

void Logger::debugPlot(String varName, float value) {
    if (isOff() || !isDebug()) return;
    printToOutput(">", varName + ":" + String(value));
}

// ── Checks de nivel ───────────────────────────────────────────────────────────
bool Logger::isTrace() { return level <= TRACE; }
bool Logger::isDebug() { return level <= DEBUG; }
bool Logger::isInfo()  { return level <= INFO;  }
bool Logger::isWarn()  { return level <= WARN;  }
bool Logger::isError() { return level <= ERROR; }
bool Logger::isFatal() { return level <= FATAL; }
bool Logger::isOff()   { return level == OFF;   }

// ── Init ROS publisher ────────────────────────────────────────────────────────
#ifdef USE_ROS_LOGGER
void Logger::initRosPublisher(rcl_node_t* node, rclc_support_t* support)
{
    if (ros_publisher_ready) return;

    const rosidl_message_type_support_t* type_support =
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

    // Volver a usar el nombre del tópico que prefieras, pero con barra inicial /
    rcl_ret_t ret = rclc_publisher_init_default(
        &ros_log_publisher, node, type_support, "/microrosout"
    );

    ros_publisher_ready = (ret == RCL_RET_OK);
}
#endif
