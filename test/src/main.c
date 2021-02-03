#include <stdio.h>

#include "base.h"

typedef struct Test_t {
  int i;
  int j;
} Test;
Test t[] = {{0, 1}, {1, 3}, {2, 8}, {3, 9}, {4, 10}};
int onShapeEvent1(void *win, Shape *shape, int event) {
  Test *t = (Test *)shape->privData;
  Window *window = (Window *)win;
  printf("%s called. evt = %d, focus = %d, t.i = %d.\n", __FUNCTION__, event,
         window->focusID, t->i);
  if (event == EVT_SHAPE_DRAW) {
    printf("draw: x = %d, y = %d, w = %d, z = %d.\n", shape->base.x,
           shape->base.y, shape->width, shape->height);
  }
  return 0;
};
int onWindowEvent(void *win, Shape *shape, int event) {
  Window *window = (Window *)win;
  printf("%s called. evt = %d, base = %d.\n", __FUNCTION__, event,
         window->baseMap);
}
Shape item1[] = {{E_SHAPE_RECANGLE, {0, 0}, 40, 5, &onShapeEvent1, &t[0]},
                 {E_SHAPE_RECANGLE, {0, 0}, 20, 6, &onShapeEvent1, &t[1]},
#if 0
                 {E_SHAPE_RECANGLE, {0, 0}, 70, 6, &onShapeEvent1, &t[2]},
#endif
                 {E_SHAPE_RECANGLE, {0, 0}, 40, 15, &onShapeEvent1, &t[3]},
                 {E_SHAPE_CIRCLE, {10, 10}, 10, 0, &onShapeEvent1, &t[4]}};
Window win1 = {
    .item = item1,
    .itemCnt = sizeof(item1) / sizeof(item1[0]),
    .maxItemCnt = 10,
    .focusEnable = TRUE,
    .focusID = 0,
    .align = ALIGN_VER_LAYOUT | ALIGN_HOR_MIDDLE,
    .onWindowEvent = onWindowEvent,
};

int main() {
  int i = 0;
  winOpen(&win1);
  while (1) {
    printf("please Enter op:\n");
    scanf("%d", &i);
    if (i == 0) {
      winItemUp(&win1);
    } else if (i != -1) {
      winItemDown(&win1);
    } else {
      winClose(&win1);
      break;
    }
  }
  return 0;
}