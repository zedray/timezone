#include <pebble.h>
#include "watchface.h"

int main(void) {
  watchface_init();
  app_event_loop();
  watchface_deinit();
}

struct tm getTime(struct tm *tick_time) {

  //if (!tick_time) {
    time_t now = time(NULL);
    now = now + 1 * 60; // 1 hour in seconds.
    //now = now;
    tick_time = localtime(&now);
  //}
  return *tick_time;
}