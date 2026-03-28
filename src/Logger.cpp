#include "Logger.h"

// ── Instancia global — constructor vacío, sin efectos secundarios ─────────────
Logger logger;

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
        // Nivel según rcl_interfaces/msg/Log
        switch (this->level) {
            case TRACE: ros_log_msg.level = rcl_interfaces__msg__Log__DEBUG; break;
            case DEBUG: ros_log_msg.level = rcl_interfaces__msg__Log__DEBUG; break;
            case INFO:  ros_log_msg.level = rcl_interfaces__msg__Log__INFO;  break;
            case WARN:  ros_log_msg.level = rcl_interfaces__msg__Log__WARN;  break;
            case ERROR: ros_log_msg.level = rcl_interfaces__msg__Log__ERROR; break;
            case FATAL: ros_log_msg.level = rcl_interfaces__msg__Log__FATAL; break;
            default:    ros_log_msg.level = rcl_interfaces__msg__Log__INFO;  break;
        }

        ros_log_msg.msg.data     = (char*)msg.c_str();
        ros_log_msg.msg.size     = msg.length();
        ros_log_msg.msg.capacity = msg.length() + 1;

        // Nombre del nodo como "name"
        ros_log_msg.name.data     = (char*)"esp32";
        ros_log_msg.name.size     = 5;
        ros_log_msg.name.capacity = 6;

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
    const rosidl_message_type_support_t* type_support =
        ROSIDL_GET_MSG_TYPE_SUPPORT(rcl_interfaces, msg, Log);

    rcl_ret_t ret = rclc_publisher_init_default(
        &ros_log_publisher,
        node,
        type_support,
        "rosout"   // <-- tópico correcto
    );

    ros_publisher_ready = (ret == RCL_RET_OK);
}
#endif