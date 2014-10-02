#include <pebble.h>
#include "watchface.h"

int main(void) {
  watchface_init();
  app_event_loop();
  watchface_deinit();
}
