#include "base.h"

#include <stdio.h>

#define callShapeEvent(win, op, i)                       \
  do {                                                   \
    if (win->item && win->item[i].onShapeEvent) {        \
      win->item[i].onShapeEvent(win, &win->item[i], op); \
    }                                                    \
  } while (0)
static void getBorder(Shape *cur, int *left, int *right, int *top,
                      int *bottom) {
  if (cur->type == E_SHAPE_RECANGLE) {
    *left = cur->base.x;
    *right = cur->base.x + cur->width;
    *top = cur->base.y;
    *bottom = cur->base.y + cur->height;
  } else {
    *left = cur->base.x - cur->r;
    *right = cur->base.x + cur->r;
    *top = cur->base.y - cur->r;
    *bottom = cur->base.y + cur->r;
  }
}
int isInContent(Shape *cur, Shape *content) {
  int curLeft, curRight, curTop, curButtom;
  int conLeft, conRight, conTop, conButtom;
  if (cur == NULL || content == NULL) {
    return FALSE;
  }
  getBorder(cur, &curLeft, &curRight, &curTop, &curButtom);
  getBorder(content, &conLeft, &conRight, &conTop, &conButtom);
  if (curLeft < conLeft || curRight > conRight) {
    return FALSE;
  }
  if (curTop < conTop || curButtom > conButtom) {
    return FALSE;
  }
  return TRUE;
}
int winOpen(Window *win) {
  int i = 0;
  for (i = 0; i < win->itemCnt; ++i) {
    callShapeEvent(win, EVT_SHAPE_CREATE, i);
  }
  if (win->focusEnable) {
    callShapeEvent(win, EVT_SHAPE_FOCUS, win->focusID);
  }
  return 0;
}
int winClose(Window *win) {
  int i = 0;
  if (win->focusEnable) {
    callShapeEvent(win, EVT_SHAPE_DEFOCUS, win->focusID);
  }
  for (i = 0; i < win->itemCnt; ++i) {
    callShapeEvent(win, EVT_SHAPE_DESTROY, i);
  }
  return 0;
}
static int winItemMove(Window *win, int step) {
  int newFocus = win->focusID + step;
  int newBase = win->baseMap + step;
  int i = 0;
  if (win->itemCnt >= win->maxItemCnt ||
      (newFocus >= 0 && newFocus < win->itemCnt)) {
    // 不需要重新绑定数据
    if (win->itemCnt == 1) {
      return 0;  // 只有一个元素，移动不了
    }
    newFocus %= win->itemCnt;
    newFocus = (newFocus < 0) ? win->itemCnt + newFocus : newFocus;
    callShapeEvent(win, EVT_SHAPE_DEFOCUS, win->focusID);
    win->focusID = newFocus;
    if (win->focusEnable) {
      callShapeEvent(win, EVT_SHAPE_FOCUS, newFocus);  // 新焦点已被获取到
    }
    return 0;
  }
  callShapeEvent(win, EVT_SHAPE_DEFOCUS, win->focusID);
  if (newBase < 0) {
    win->focusID = win->itemCnt - 1;  // 退到最大值
  }
  if (newBase >= win->maxItemCnt - win->itemCnt) {
    win->focusID = 0;
  }
  newBase %= (win->maxItemCnt - win->itemCnt);
  newBase = newBase < 0 ? win->maxItemCnt - win->itemCnt + newBase : newBase;
  win->baseMap = newBase;
  if (win->onWindowEvent) {
    win->onWindowEvent(win, win->item, EVT_WIN_CONTENT_UPDATE);
  }
  callShapeEvent(win, EVT_SHAPE_FOCUS, win->focusID);
  return 0;
}
int winItemUp(Window *win) { return winItemMove(win, -1); }
int winItemDown(Window *win) { return winItemMove(win, 1); }