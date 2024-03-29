#include <pebble.h>
#include "watchface.h"

static Window *window;
static TextLayer *text_date_layer;
static TextLayer *text_time_layer;

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    // Need to be static because they're used by the system later.
    static char time_text[] = "00:00";
    static char date_text[] = "Xxxxxxxxx 00";

    char *time_format;

    if (!tick_time) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Update the tick time from local time");
      time_t now = time(NULL);
      //now += 100000;  __darwin_time_t
      tick_time = localtime(&now);
    }
    tick_time->tm_hour = 0;
    
    time_t nowTime = time(NULL);

    //long nowLong = nowTime.__darwin_time_t;
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "local time %l", nowLong);
    
    int	tm_sec = tick_time->tm_sec;
    int	tm_min = tick_time->tm_min;
    int	tm_hour = tick_time->tm_hour;
    int	tm_mday = tick_time->tm_mday;
    int	tm_mon = tick_time->tm_mon;
    int	tm_year = tick_time->tm_year;
    int	tm_wday = tick_time->tm_wday;
    int	tm_yday = tick_time->tm_yday;
    int tm_isdst = tick_time->tm_isdst;

    

    //long tm_gmtoff = tick_time->tm_gmtoff;
    //char *tm_zone = tick_time->tm_zone;

    APP_LOG(
            APP_LOG_LEVEL_DEBUG,
            "tick_time hour %d  min %d",
            tm_hour,
            tm_min
            );
    
    
  // TODO: Only update the date when it's changed.
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date_text);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}

void watchface_deinit(void) {
  tick_timer_service_unsubscribe();
}

void watchface_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  text_date_layer = text_layer_create(GRect(8, 68, 144-8, 168-68));
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  text_time_layer = text_layer_create(GRect(7, 92, 144-7, 168-92));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
}
