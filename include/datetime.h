/** ===========================================================================
 *      Automated Plant Watering System
 *          By Meltwin - 2024 (c) MIT Licence
 *  ============================================================================
 * This code provide an automated way of watering your plants. You can select
 * your activation pump duration as well as your watering period.
 */

#ifndef MELTWIN_DATETIME
#define MELTWIN_DATETIME

#include <Preferences.h>
#include <cstring>
#include <sstream>
#include "hardware_configs.h"

namespace meltwin {
  struct DateTime {
    static constexpr const char* ISO_NULL{"0000-00-00T00:00:00.000000+00:00"};
    static constexpr const char* YEAR_KEY{"yr"};
    static constexpr const char* MONTH_KEY{"mth"};
    static constexpr const char* DAY_KEY{"dy"};
    static constexpr const char* HOUR_KEY{"hr"};
    static constexpr const char* MINUTES_KEY{"min"};
    static constexpr const char* SECONDS_KEY{"s"};
    static constexpr const char* USEC_KEY{"us"};

    // Time structure
    const char* prefix = "";
    uint16_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    uint32_t usecs = 0;

    DateTime() {}
    DateTime(uint16_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t mi, uint8_t s, uint32_t us, const char* p = "") :
        year(y), month(m), day(d), hour(h), minutes(mi), seconds(s), usecs(us), prefix(p) {}

    // Parse time from ISO-8601-1 datetime format: "YYYY-MM-DDThh:mm:ss.ssssss+hh:mm"
    static DateTime from_iso(const char* iso_string) {
      DateTime datetime;
      char buffer[6];
      const char* p = (const char*)iso_string;

      // Get year
      strncpy(buffer, p, 4);
      buffer[4] = '\0';
      datetime.year = strtol(buffer, NULL, 10);
      p += 5;

      // Get month
      strncpy(buffer, p, 2);
      buffer[2] = '\0';
      datetime.month = strtol(buffer, NULL, 10);
      p += 3;

      // Get day
      strncpy(buffer, p, 2);
      datetime.day = strtol(buffer, NULL, 10);
      p += 3;

      // Get hour
      strncpy(buffer, p, 2);
      datetime.hour = strtol(buffer, NULL, 10);
      p += 3;

      // Get minutes
      strncpy(buffer, p, 2);
      datetime.minutes = strtol(buffer, NULL, 10);
      p += 3;

      // Get seconds
      strncpy(buffer, p, 2);
      datetime.seconds = strtol(buffer, NULL, 10);
      p += 3;

      // Get microseconds
      strncpy(buffer, p, 6);
      buffer[6] = '\0';
      datetime.usecs = strtol(buffer, NULL, 10);

      return datetime;
    }

    static std::string prefixed(const char* prefix, const char* key) {
      std::stringstream ss;
      ss << prefix << key;
      // Serial.printf("Using key \"%s\", and do exist: %s\n", ss.str().c_str(), (preferences.isKey(ss.str().c_str())) ?
      // "true" : "false");
      return ss.str();
    }

    static uint8_t february_length(uint16_t year) {
      if (year % 4 != 0)
        return 28;
      if (year % 100 != 0)
        return 29;
      if (year % 400 != 0)
        return 28;
      return 29;
    }

    inline bool is_long_month(uint8_t month) {
      return (month == 1) || (month == 3) || (month = 5) || (month == 7) || (month == 8) || (month == 10) ||
        (month == 12);
    }

    inline bool is_short_month(uint8_t month) { return (month == 4) || (month == 6) || (month == 9) || (month == 11); }

    static void rjust_insert(std::string& in, const char* what, const size_t where_end, const size_t size_limit) {
      size_t what_len = std::min(std::strlen(what), size_limit);
      for (size_t d_pos = 0; d_pos < what_len; d_pos++)
        in[where_end - d_pos] = what[what_len - d_pos - 1];
    }

    std::string to_iso_string() const {
      std::string iso_str = DateTime::ISO_NULL;

      // Insert date
      rjust_insert(iso_str, std::to_string(this->year).c_str(), 3, 4);
      rjust_insert(iso_str, std::to_string(this->month).c_str(), 6, 2);
      rjust_insert(iso_str, std::to_string(this->day).c_str(), 9, 2);

      // Insert time
      rjust_insert(iso_str, std::to_string(this->hour).c_str(), 12, 2);
      rjust_insert(iso_str, std::to_string(this->minutes).c_str(), 15, 2);
      rjust_insert(iso_str, std::to_string(this->seconds).c_str(), 18, 2);
      rjust_insert(iso_str, std::to_string(this->usecs).c_str(), 25, 6);

      return iso_str;
    }


    DateTime& operator+=(const DateTime& other) {
      // Add USecs
      usecs += other.usecs;
      if (usecs >= 1000000) {
        seconds++;
        usecs -= 1000000;
      }

      // Add seconds
      seconds += other.seconds;
      if (seconds >= 60) {
        minutes++;
        seconds -= 60;
      }

      // Add minutes
      minutes += other.minutes;
      if (minutes >= 60) {
        hour++;
        minutes -= 60;
      }

      // Add hours
      hour += other.hour;
      if (hour >= 24) {
        day++;
        hour -= 24;
      }

      // Add days
      day += other.day;
      uint8_t feb_length = DateTime::february_length(year);
      if ((month == 2 && day > feb_length) || (is_short_month(month) && day > 30) ||
          (is_long_month(month) && day > 31)) {
        month++;
        day = 1;
      }

      // Add months
      month += other.month;
      if (month > 12) {
        year++;
        month = 1;
      }

      // Add years
      year += other.year;

      return *this;
    }
    friend DateTime operator+(DateTime first, const DateTime& second) {
      first += second;
      return first;
    }
    friend bool operator<(const DateTime& first, const DateTime& second) {
      // Year
      if (second.year > first.year)
        return true;
      if (second.year < first.year)
        return false;

      // Month
      if (second.month > first.month)
        return true;
      if (second.month < first.month)
        return false;

      // Day
      if (second.day > first.day)
        return true;
      if (second.day < first.day)
        return false;

      // Hour
      if (second.hour > first.hour)
        return true;
      if (second.hour < first.hour)
        return false;

      // Minutes
      if (second.minutes > first.minutes)
        return true;
      if (second.minutes < first.minutes)
        return false;

      // Seconds
      if (second.seconds > first.seconds)
        return true;
      if (second.seconds < first.seconds)
        return false;

      // Micro secs
      if (second.usecs < first.usecs)
        return false;

      return true;
    }
    friend bool operator>(const DateTime& lhs, const DateTime& rhs) { return rhs < lhs; }
    friend bool operator<=(const DateTime& lhs, const DateTime& rhs) { return !(lhs > rhs); }
    friend bool operator>=(const DateTime& lhs, const DateTime& rhs) { return !(lhs < rhs); }
  };

} // namespace meltwin

#endif
