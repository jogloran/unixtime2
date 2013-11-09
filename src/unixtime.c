/*

   How to use a custom non-system font.

 */

#include <pebble.h>
// The auto-generated header file with resource handle definitions
#include "resource_ids.auto.h"

Window* window;

Layer* bg_layer;

TextLayer* text_layer;
TextLayer* subtext_layer;
GFont* large_font, *small_font;

void itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;
    int sign;

    sign = (radix == 10 && value < 0);
    if (sign)   v = -value;
    else    v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    if (sign)
    *sp++ = '-';
    while (tp > tmp)
    *sp++ = *--tp;

    *sp++ = '\0';
}

#define GRECT_INSET(r, dx, dy) GRect((r).origin.x+(dx), (r).origin.y+(dy), (r).size.w - (2*(dx)), (r).size.h - (2*(dy)))

void layer_update_handler(Layer* l, GContext* ctx) {
  graphics_context_set_compositing_mode(ctx, GCompOpAssignInverted);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRECT_INSET(layer_get_frame(l), 10, 10), 32, GCornerTopRight);
}

void handle_second_tick(struct tm* tick_time, TimeUnits units_changed);

void handle_init() {
  window = window_create();
  window_stack_push(window, true /* Animated */);

  large_font = \
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_M_52));
  small_font = \
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_M_24));
    
  bg_layer = layer_create(layer_get_frame(window_get_root_layer(window)));
  layer_set_update_proc(bg_layer, &layer_update_handler);
  layer_add_child(window_get_root_layer(window), bg_layer);

  text_layer = text_layer_create(GRect(0, 32, layer_get_frame(window_get_root_layer(window)).size.w, 70));
  text_layer_set_font(text_layer, large_font);
  text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	subtext_layer = text_layer_create(GRect(0, 32+78, layer_get_frame(window_get_root_layer(window)).size.w, 40));
  text_layer_set_font(subtext_layer, small_font);
  text_layer_set_text_color(subtext_layer, GColorWhite);
	text_layer_set_background_color(subtext_layer, GColorClear);
	text_layer_set_text_alignment(subtext_layer, GTextAlignmentCenter);
	
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(subtext_layer));

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
  struct tm* t = tick_time;
  
  static char buf[8] = "\0";
  int seconds_since_midnight = t->tm_hour * 3600 + t->tm_min * 60 + t->tm_sec;
  itoa(seconds_since_midnight, buf, 10);
  text_layer_set_text(text_layer, buf);
  
  static char buf2[32] = "\0";
  strftime(buf2, sizeof(buf2), "%I.%M.%S %p", t);
  text_layer_set_text(subtext_layer, buf2);
}

void handle_deinit() {

}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

