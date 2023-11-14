#include "SA868.h"


SA868::SA868(HardwareSerial *hwser)
{
  this->ATSer = hwser;
};

String SA868::getVersion()
{
  String data;

  this->ATSer->printf("AT+VERSION\r\n");
  if (this->waitResponse(data, "\r\n", 1000))
  {
    String version = data.substring(0, data.indexOf("\r\n"));
    return version;
  }
  else
  {
    return "-";
  }
};

String SA868::getHardware()
{
  String data;

  this->ATSer->printf("AT+MODEL\r\n");
  if (this->waitResponse(data, "\r\n", 1000))
  {
    String version = data.substring(0, data.indexOf("\r\n"));
    return version;
  }
  else
  {
    return "-";
  }
};

bool SA868::waitResponse(String &data, String rsp, uint32_t timeout)
{
  uint32_t startMillis = millis();
  data.clear();
  do
  {
    while (this->ATSer->available() > 0)
    {
      int8_t ch = this->ATSer->read();
      data += static_cast<char>(ch);
      if (rsp.length() && data.endsWith(rsp))
      {
        return true;
      }
    }
  } while (millis() - startMillis < timeout);
  return false;
}