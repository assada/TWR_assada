#include "SA868.h"

SA868::SA868(HardwareSerial* hwser)
{
  this->ATSer = hwser;
};

String SA868::getVersion() const
{
  String data;

  this->ATSer->printf("AT+VERSION\r\n");
  if (this->waitResponse(data, "\r\n", 1000))
  {
    String version = data.substring(0, data.indexOf("\r\n"));
    return version;
  }

  return "-";
};

String SA868::getHardware() const
{
  String data;

  this->ATSer->printf("AT+MODEL\r\n");
  if (this->waitResponse(data, "\r\n", 1000))
  {
    String version = data.substring(0, data.indexOf("\r\n"));
    return version;
  }

  return "-";
};

unsigned int SA868::pick(uint16_t reg) const
{
  String data;

  this->ATSer->printf("AT+PICK=%u\r\n", reg);
  if (this->waitResponse(data, "\r\n", 1000))
  {
    String stringResponse = data.substring(0, data.indexOf("\r\n"));
    return stringResponse.toInt(); //lol
  }

  return 0;
}

bool SA868::poke(const uint16_t reg, const uint16_t val)
{
  String data;

  this->ATSer->printf("AT+POKE=%u,%u\r\n", reg, val);
  if (this->waitResponse(data, "\r\n", 1000))
  {
    return data == "OK";
  }

  return false;
}

bool SA868::waitResponse(String& data, String rsp, uint32_t timeout) const
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
  }
  while (millis() - startMillis < timeout);
  return false;
}
