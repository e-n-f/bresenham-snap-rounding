#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <set>

struct seg {
    int x0;
    int y0;
    int x1;
    int y1;
    int xx, yy;

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

        restart();
    }

    void restart() {
        xx = x0;
        yy = y0;

        dx = abs(x1 - x0);
        sx = (x0 < x1) ? 1 : -1;

        dy = abs(y1 - y0);
        sy = (y0 < y1) ? 1 : -1;

        err = ((dx > dy) ? dx : -dy) / 2;
    }

    void out(const char *op, std::vector<std::set<int> > pixels, int id) {
        printf("%d %d %s ", xx, yy, op);
        pixels[yy * 100 + xx].insert(id);
    }

    void run(std::vector<std::set<int> > pixels, int id, bool check) {
        out("moveto", pixels, id);
        while (xx != x1 || yy != y1) {
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
                    yy += sy;
                    out("lineto", pixels, id);
                    xx += sx;
                    out("lineto", pixels, id);
                } else {
                    xx += sx;
                    out("lineto", pixels, id);
                    yy += sy;
                    out("lineto", pixels, id);
                }
            } else if (across) {
                xx += sx;
                out("lineto", pixels, id);
            } else if (down) {
                yy += sy;
                out("lineto", pixels, id);
            }
        }
        printf("stroke\n");
    }
};

int main() {
    srand(1);
    std::vector<seg> segs;
    std::vector<std::set<int> > pixels;
    pixels.resize(100 * 100);

    printf("0 setlinewidth\n");

    for (size_t i = 0; i < 50; i++) {
        int x0 = rand() % 100;
        int y0 = rand() % 100;
        int x1 = rand() % 100;
        int y1 = rand() % 100;

        segs.push_back(seg(x0, y0, x1, y1));
        printf("%d %d moveto %d %d lineto stroke\n", segs[i].x0, segs[i].y0, segs[i].x1, segs[i].y1);
        segs[i].run(pixels, i, false);
    }

    for (size_t i = 0; i < 50; i++) {
        segs[i].restart();
        segs[i].run(pixels, i, true);
    }
}
