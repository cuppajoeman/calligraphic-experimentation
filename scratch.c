#include <cairo.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.14159265

int basic[20][2] = {
  {50, 600 },
  {400, 200},
  {600, 400},
  {400, 600},
  {50, 200}
};

int thickness = 5;
float theta = PI/8;

void draw_calligraphic_one_polygon(cairo_t *cr, int path[][2], int path_len) {
  float x_shift = cos(theta) * thickness;
  float y_shift = sin(theta) * thickness;

  float x_curr, y_curr, x_next, y_next;
  bool moving_right, moving_left, switched_direction = false;

  int N = path_len;

  int wrapped_array[(2 * N) - 1][2];

  for (int i = 0; i < N; i++) {
    wrapped_array[i][0] = path[i][0];
    wrapped_array[i][1] = path[i][1];
  }

  int count = 0;
  for (int i = 2 ; i <= N ; i++) {
    printf("%d", N-i);
    wrapped_array[N + count][0] = path[N-i][0];
    wrapped_array[N + count][1] = path[N-i][1];
    count += 1;
  }

  for (int i = 0; i < ( (2 * N) - 1) ; i++) {
    printf("x: %d, y %d\n", wrapped_array[i][0],  wrapped_array[i][1]);
  }

  bool first_iteration = true;
  for (int i = 0; i < ( (2 * N) - 1) - 1; i++) {
    x_curr = wrapped_array[i][0];
    y_curr = wrapped_array[i][1];

    x_next = wrapped_array[i+1][0];
    y_next = wrapped_array[i+1][1];

    printf("x_curr: %f y_curr: %f \n", x_curr, y_curr);
    printf("x_next: %f y_next: %f \n", x_next, y_next);

    if (x_curr < x_next) {
      printf("moving right\n");
      if (!first_iteration) {
        if (moving_left) {
          switched_direction = true;
        } else {
          switched_direction = false;
        }
      }

      moving_right = true;
      moving_left = false;

    } else if (x_curr > x_next) {
      printf("moving left\n");

      if (!first_iteration) {
        if (moving_right) {
          switched_direction = true;
        } else {
          switched_direction = false;
        }
      }

      moving_right = false;
      moving_left = true;

    } else {  // x_curr == x_next
      // Then we continue in the same direction?
    }

    printf("switching direction: %s \n", switched_direction ? "true": "false");

    if (first_iteration) {
      if (moving_right) {
        //draw down to up
        printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr - x_shift, y_curr - y_shift, x_curr + x_shift, y_curr + y_shift);
        cairo_line_to(cr, x_curr - x_shift, y_curr - y_shift);
        cairo_line_to(cr, x_curr + x_shift, y_curr + y_shift);
      } else { // moving_left what about neither on the first iteration?
        // draw down to up
        printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr + x_shift, y_curr + y_shift, x_curr - x_shift, y_curr - y_shift);
        cairo_line_to(cr, x_curr + x_shift, y_curr + y_shift);
        cairo_line_to(cr, x_curr - x_shift, y_curr - y_shift);
      }
    }

    if (!first_iteration && switched_direction) {
      if (moving_right) {
        // (prepare to move right) draw a line up
        printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr - x_shift, y_curr - y_shift, x_curr + x_shift, y_curr + y_shift);
        cairo_line_to(cr, x_curr + x_shift, y_curr + y_shift);

      } else { // moving_left
        // draw a line down
        printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr + x_shift, y_curr + y_shift, x_curr - x_shift, y_curr - y_shift);
        cairo_line_to(cr, x_curr - x_shift, y_curr - y_shift);
      }
    }

    if (moving_right) {
      printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr + x_shift, y_curr + y_shift, x_next + x_shift, y_next + y_shift);
      cairo_line_to(cr, x_next + x_shift, y_next + y_shift);
    } else { // moving_left what about neither on the first iteration?
        // draw down to up
      printf("drawing from (%f, %f) -> (%f, %f)\n", x_curr - x_shift, y_curr - y_shift, x_next - x_shift, y_next - y_shift);
      cairo_line_to(cr, x_next - x_shift, y_next - y_shift);
    }
    first_iteration = false;
    printf("========== END OF ITERATION ==========\n");
  }
  cairo_close_path(cr);
  cairo_fill(cr);
  //cairo_stroke(cr);
}


int main (int argc, char *argv[]) {
  cairo_surface_t *surface =
  cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 800);
  cairo_t *cr =
  cairo_create (surface);

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);

  draw_calligraphic_one_polygon(cr, basic, 5);

  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, "x.png");
  cairo_surface_destroy (surface);
  return 0;
}

