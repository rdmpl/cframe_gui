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

  return 0;
};
int onShapeEvent2(void *window, Shape *shape, int event) {
  printf("%s called. evt = %d.\n", __FUNCTION__, event);
  return 0;
};
int onShapeEvent3(void *window, Shape *shape, int event) {
  printf("%s called. evt = %d.\n", __FUNCTION__, event);
  return 0;
};
int onWindowEvent(void *win, Shape *shape, int event) {
  Window *window = (Window *)win;
  printf("%s called. evt = %d, base = %d.\n", __FUNCTION__, event,
         window->baseMap);
}
Shape item1[] = {{E_SHAPE_RECANGLE, {0, 0}, {40}, 5, &onShapeEvent1, &t[0]},
                 {E_SHAPE_RECANGLE, {0, 0}, {20}, 6, &onShapeEvent1, &t[1]},
                 {E_SHAPE_RECANGLE, {0, 0}, {70}, 6, &onShapeEvent1, &t[2]},
                 {E_SHAPE_RECANGLE, {0, 20}, {40}, 15, &onShapeEvent1, &t[3]},
                 {E_SHAPE_CIRCLE, {10, 10}, 10, 0, &onShapeEvent1, &t[4]}};
Window win1 = {
    .itemCnt = 5,
    .maxItemCnt = 10,
    .focusEnable = 1,
    .focusID = 0,
    .item = item1,
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