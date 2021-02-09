#ifndef ExecuteChannel_h
#define ExecuteChannel_h

#include "Arduino.h"
#include "Device.h"
#include "ArduinoJson.h"


class ExecuteChannel{
    public:
        // constructor
        ExecuteChannel(void);
        // destructor
        ~ExecuteChannel(void);
        // Execute class init with device object
        void init(Device*);
        // time to time mode function
        void timeToTime(char*, DateTime);
        // time duration mode function
        void timeDuration(char*, DateTime);
        // temperature control mode function
        void temperatureControl(char*, DateTime);
        // humidity control mode function
        void humidityControl(char*, DateTime);
        // mirror mode function
        void mirror(char*, DateTime);
        // general execute function
        void execute(char*, DateTime);
    private:
        // device object
        Device *deviceObj;
};

#endif