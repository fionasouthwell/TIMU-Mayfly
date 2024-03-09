#ifndef TIMU_LOGGER_H
#define TIMU_LOGGER_H

#include <ModularSensors.h>
#include <helpers.h>

// The default Logger automatically puts the logger to sleep
// This modified version inherits and keeps all functionality EXCEPT it powers off the
// switched voltage regulator before putting the mayfly to sleep
class TIMU_Logger : public Logger {
    using Logger::Logger;

   public:
    void logDataAndPublish(void);
    void logData(void);
};


#endif