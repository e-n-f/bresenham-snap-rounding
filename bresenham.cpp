#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct seg {
    int x0;
    int y0;
    int x1;
    int y1;

    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    bool reversed;

    seg(int _x0, int _y0, int _x1, int _y1) {
        if (_y1 >= _y0) {
            x0 = _x0;
            y0 = _y0;
            x1 = _x1;
            y1 = _y1;

            reversed = false;
        } else {
            x0 = _x1;
            y0 = _y1;
            x1 = _x0;
            y1 = _y0;

            reversed = true;
        }

        dx = abs(x1 - x0);
        sx = (x0 < x1) ? 1 : -1;

        dy = abs(y1 - y0);
        sy = (y0 < y1) ? 1 : -1;

        err = ((dx > dy) ? dx : -dy) / 2;
    }

    void out(const char *op) {
        printf("%d %d %s ", x0, y0, op);
    }

    void run() {
        out("moveto");
        while (x0 != x1 || y0 != y1) {
            int e2 = err;
            bool across = false, down = false;

            if (e2 > -dx) {
                err -= dy;
                across = true;
            }

            if (e2 < dy) {
                err += dx;
                down = true;
            }

            if (across && down) {
                if (dx > dy) {
                    y0 += sy;
                    out("lineto");
                    x0 += sx;
                    out("lineto");
                } else {
                    x0 += sx;
                    out("lineto");
                    y0 += sy;
                    out("lineto");
                }
            } else if (across) {
                x0 += sx;
                out("lineto");
            } else if (down) {
                y0 += sy;
                out("lineto");
            }
        }
        printf("stroke\n");
    }
};

int main() {
    srand(1);

    for (size_t i = 0; i < 50; i++) {
        int x0 = rand() % 100;
        int y0 = rand() % 100;
        int x1 = rand() % 100;
        int y1 = rand() % 100;

        seg s(x0, y0, x1, y1);

        printf("0 setlinewidth\n");
        printf("%d %d moveto %d %d lineto stroke\n", x0, y0, x1, y1);

        s.run();
    }
}
