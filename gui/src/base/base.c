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
  return curLeft >= conLeft && curRight <= conRight && curTop >= conTop &&
         curButtom <= conButtom;
}
static inline int calItemX(int align, int winX, int winW, int itemW) {
  if (align & ALIGN_HOR_LEFT) {
    return winX;
  } else if (align & ALIGN_HOR_MIDDLE) {
    return winX + winW / 2 - itemW / 2;
  }
  return winX + winW - itemW;
}
static inline int calItemY(int align, int winY, int winH, int itemH) {
  if (align & ALIGN_VER_TOP) {
    return winY;
  } else if (align & ALIGN_VER_MIDDLE) {
    return winY + winH / 2 - itemH / 2;
  }
  return winY + winH - itemH;
}

static int calItemPosHor(Window *win) {
  int i = 0;
  win->item[0].base.x = win->shape.base.x;
  win->item[0].base.y = calItemY(win->align, win->shape.base.y,
                                 win->shape.height, win->item[0].height);
  for (i = 1; i < win->itemCnt; ++i) {
    win->item[i].base.x = win->item[i - 1].base.x + win->item[i - 1].width;
    win->item[i].base.y = calItemY(win->align, win->shape.base.y,
                                   win->shape.height, win->item[i].height);
  }
  return 0;
}
static int calItemPosVer(Window *win) {
  int i = 0;
  win->item[0].base.y = win->shape.base.y;
  win->item[0].base.x = calItemX(win->align, win->shape.base.x,
                                 win->shape.width, win->item[i].width);
  for (i = 1; i < win->itemCnt; ++i) {
    win->item[i].base.x = calItemX(win->align, win->shape.base.x,
                                   win->shape.width, win->item[i].width);
    win->item[i].base.y = win->item[i - 1].base.y + win->item[i - 1].height;
  }
  return 0;
}
static int calItemPos(Window *win) {
  if (win->itemCnt == 0) {
    return -1;
  }
  if (win->align & ALIGN_HOR_LAYOUT) {
    calItemPosHor(win);
  } else if (win->align & ALIGN_VER_LAYOUT) {
    calItemPosVer(win);
  }
  return 0;
};
int winOpen(Window *win) {
  int i = 0;
  for (i = 0; i < win->itemCnt; ++i) {
    callShapeEvent(win, EVT_SHAPE_CREATE, i);
  }
  calItemPos(win);
  for (i = 0; i < win->itemCnt; ++i) {
    callShapeEvent(win, EVT_SHAPE_DRAW, i);
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