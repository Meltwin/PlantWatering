//
// Created by meltwin on 18/12/24.
//

#ifndef APICALLER_HPP
#define APICALLER_HPP

#include <ArduinoJson.hpp>
#include <HTTPClient.h>
#include <sstream>
#include <string>
#include "IO/Pump.hpp"
#include "common.hpp"
#include "datetime.h"

namespace meltwin {

  enum APIErrors {
    NO_ERROR = 0,
    CUSTOM = -1,

    // API Management error
    JSON_ENCODE_ERROR = 100,

    MALFORMED_REQUEST = 200,
    MISSING_ARGUMENTS = 210,

    UNKNOWN_API_PATH = 300,
    INVALID_PATH = 310,

    UNREACHABLE_DB = 400,
    INVALID_DB_CONF_FILE = 410,
    SQL_ERROR = 420,

    INVALID_CREDENTIALS = 500,
    INVALID_TOKEN = 510,
    INVALID_USER_ID = 520
  };

  enum InternalErrors { SUCCESS = 0, FAILED = 1, WRONG_AUTH = 2, WRONG_TOKEN = 3, OTHER = 4 };

  struct Endpoints {
    constchar TIME{"http://worldtimeapi.org/api/ip"};
    constchar LOGIN{"https://meltwin.fr/api/auth/login"};
    constchar SEND_DATA{"https://meltwin.fr/api/plants/record"};
    constchar GET_WATERING_CMD{"https://meltwin.fr/api/plants/get_cmd"};
    constchar WATERING_COMPLETED{"https://meltwin.fr/api/plants/done"};
  };

  struct Payload {

    explicit Payload() {}
    std::string str() { return buffer.str(); }

    template <typename T>
    void add_data(const std::string& param, const T& value) {
      if (!buffer.str().empty())
        buffer << "&";
      buffer << param << "=" << value;
    }

  private:
    std::stringstream buffer;
  };

  struct APICaller {

    inline static InternalErrors authenticate(std::string& token) {
      HTTPClient client;
      setup_post_client(client);

      // Make Payload
      Payload payload;
      payload.add_data("username", "plant01");
      payload.add_data("password", "plt01_access");
      client.begin(Endpoints::LOGIN);
      int code = client.POST(String(payload.str().c_str()));
      if (code <= 0) {
        client.end();
        return InternalErrors::FAILED;
      }

      ArduinoJson::JsonDocument doc;
      ArduinoJson::deserializeJson(doc, client.getString());
      client.end();

      // Process error code
      switch (doc["err_code"].as<int>()) {
      case APIErrors::NO_ERROR:
        token = std::string(doc["token"].as<const char*>());
        return InternalErrors::SUCCESS;
      case APIErrors::INVALID_CREDENTIALS:
        return InternalErrors::WRONG_AUTH;
      default:
        Serial.printf("[Login] Other error: %d\n%s\n", doc["err_code"].as<int>(), doc["err_msg"].as<const char*>());
        return InternalErrors::OTHER;
      }
    }

    inline static InternalErrors sendData(const char* token, unsigned int sensor_id, float value) {
      HTTPClient client;
      setup_post_client(client);

      // Make Payload
      Payload payload;
      payload.add_data("token", token);
      payload.add_data("sensor_id", sensor_id);
      payload.add_data("value", value);

      client.begin(Endpoints::SEND_DATA);
      int code = client.POST(payload.str().c_str());
      if (code <= 0) {
        client.end();
        return InternalErrors::FAILED;
      }

      ArduinoJson::JsonDocument doc;
      ArduinoJson::deserializeJson(doc, client.getString());
      client.end();

      // Process error code
      switch (doc["err_code"].as<int>()) {
      case APIErrors::NO_ERROR:
        return InternalErrors::SUCCESS;
      case APIErrors::INVALID_TOKEN:
        return InternalErrors::WRONG_TOKEN;
      default:
        Serial.printf("[Record] Other error: %d\n%s\n", doc["err_code"].as<int>(), doc["err_msg"].as<const char*>());
        return InternalErrors::OTHER;
      }
    }

    inline static InternalErrors getTime(DateTime& datetime) {
      HTTPClient client;
      client.begin(Endpoints::TIME);
      int code = client.GET();
      if (code <= 0) {
        client.end();
        return InternalErrors::FAILED;
      }

      ArduinoJson::JsonDocument doc;
      ArduinoJson::deserializeJson(doc, client.getString());

      datetime = DateTime::from_iso(doc["datetime"]);
      client.end();
      return InternalErrors::SUCCESS;
    }

    inline static InternalErrors getPumpCmd(const char* token, const size_t pump_id, PumpCmd& cmd) {
      HTTPClient client;
      client.begin(Endpoints::GET_WATERING_CMD);

      Payload payload;
      payload.add_data("token", token);
      payload.add_data("pump_id", pump_id);

      int code = client.POST(payload.str().c_str());
      if (code <= 0) {
        client.end();
        return InternalErrors::FAILED;
      }

      ArduinoJson::JsonDocument doc;
      ArduinoJson::deserializeJson(doc, client.getString());

      cmd.pump_id = doc["pump_id"];
      cmd.time = doc["duration"];
      cmd.pwm = doc["pwm"];

      client.end(); // Process error code
      switch (doc["err_code"].as<int>()) {
      case APIErrors::NO_ERROR:
        return InternalErrors::SUCCESS;
      case APIErrors::INVALID_TOKEN:
        return InternalErrors::WRONG_TOKEN;
      default:
        Serial.printf("[GetCMD] Other error: %d\n%s\n", doc["err_code"].as<int>(), doc["err_msg"].as<const char*>());
        return InternalErrors::OTHER;
      }
    }

    inline static InternalErrors pumpingDone(const char* token, const size_t pump_id) {
      HTTPClient client;
      client.begin(Endpoints::WATERING_COMPLETED);

      Payload payload;
      payload.add_data("token", token);
      payload.add_data("pump_id", pump_id);

      int code = client.POST(payload.str().c_str());
      if (code <= 0) {
        client.end();
        return InternalErrors::FAILED;
      }

      ArduinoJson::JsonDocument doc;
      ArduinoJson::deserializeJson(doc, client.getString());
      client.end();

      // Process error code
      switch (doc["err_code"].as<int>()) {
      case APIErrors::NO_ERROR:
        return InternalErrors::SUCCESS;
      case APIErrors::INVALID_TOKEN:
        return InternalErrors::WRONG_TOKEN;
      default:
        Serial.printf("[PumpDone] Other error: %d\n%s\n", doc["err_code"].as<int>(), doc["err_msg"].as<const char*>());
        return InternalErrors::OTHER;
      }
    }

  private:
    void static setup_post_client(HTTPClient& client) {
      char header[1024];
      client.addHeader("Content-Type", "application/x-www-form-urlencoded");
      client.addHeader("Charset", "ascii");
    }
  };

}; // namespace meltwin

#endif // APICALLER_HPP
