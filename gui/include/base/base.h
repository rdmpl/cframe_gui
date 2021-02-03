#ifndef GUI_BASE_H__
#define GUI_BASE_H__
#define TRUE 1
#define FALSE 0
#ifndef NULL
#define NULL ((void *)0)
#endif
enum {
  E_SHAPE_RECANGLE = 0,
  E_SHAPE_CIRCLE = 1,
};
enum {
  LAYOUT_HORIZONTAL = 0x1,
  LAYOUT_VERTICAL = 0x2,
};
enum {
  ALIGN_HOR_LAYOUT = 0x01,
  ALIGN_VER_LAYOUT = 0x02,
  ALIGN_HOR_LEFT = 0x04,
  ALIGN_HOR_MIDDLE = 0x08,
  ALIGN_HOR_RIGHT = 0x10,
  ALIGN_VER_TOP = 0x20,
  ALIGN_VER_MIDDLE = 0x40,
  ALIGN_VER_BOTTOM = 0x80,
};
typedef enum {
  EVT_SHAPE_CREATE,
  EVT_SHAPE_DRAW,
  EVT_SHAPE_FOCUS,
  EVT_SHAPE_DEFOCUS,
  EVT_SHAPE_DESTROY,
} SHAPE_EVENT;
typedef enum {
  EVT_WIN_CREATE,
  EVT_WIN_DRAW,
  EVT_WIN_FOCUS,
  EVT_WIN_DEFOCUS,
  EVT_WIN_CONTENT_UPDATE,  // 请求重新绑定内容
  EVT_WIN_DESTROY,
} WIN_EVENT;
typedef struct Point_t {
  int x;
  int y;
} Point;  // 基准点坐标
typedef struct Shape_t {
  int type;    // 形状类型
  Point base;  // 基准点
  int height;
  union {
    int width;
    int r;
  };
  int (*onShapeEvent)(void *window, struct Shape_t *shape, int evt);
  void *privData;  // 每个数据段都有自己的私有数据
} Shape;
typedef struct {
  Shape shape;  // 形状信息,窗口的形状
  int align;    // 窗口内各个形状的对齐方式
  int baseMap;  // 容器编号0映射到的内容索引
  int itemCnt;  // 容器元素个数
  int maxItemCnt;  // 最大元素个数，当这个值大于itemCnt的时候，可能会滚动
  int focusID;      // 当前焦点ID
  int focusEnable;  // 是否聚焦
  Shape *item;      // 容器里面包含的元素集合
  int (*onWindowEvent)(void *window, Shape *itembase, int evt);
} Window;  // 上下文环境
///
int isInContent(Shape *cur, Shape *content);
int winOpen(Window *win);
int winClose(Window *win);
int winItemUp(Window *win);
int winItemDown(Window *win);
#endif