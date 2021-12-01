#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

//workaround from error
#define MAX 40

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point a;
    Point b;
} Rectangle;

typedef struct {
    Point center;
    int radius;
} Circle;

typedef enum  {
    circle,
    rectangle
} shapeType;

typedef struct {
    union {
        Rectangle rectangle;
        Circle circle;
    } object;
    shapeType type;
} Shape;

typedef struct {
    int size;
    Shape objects[MAX];
} Drawing;

//graphics constants
const char vertical = '|';
const char horizontal = '-';
const char corner = '+';
const char blank = ' ';
const char border = '~';
const char circleCenter = 'X';
const char circleOut = '*';

//Point
void printPoint (Point *p);
void readPoint (Point *p);
Point initPoint();

//Rectangle
void printRectangle (Rectangle *r);
void readRectangle (Rectangle*r);
Rectangle initRectangle();

//Circle
void printCircle (Circle *c);
void readCircle (Circle *c);
Circle initCircle ();

//Drawing
Drawing initDrawing ();
void printDrawing (Drawing *d);
void addShape (Drawing *d, Shape *s);
void delShape (Drawing *d, int index);
void readDrawing (Drawing *d);
double totalArea (Drawing *d);

//shape
void printShape (Shape *s);

//graphics
void drawRectangle(int height, int width, char cv[height][width], Rectangle *r);
void drawCircle (int height, int width, char cv[height][width], Circle *c);
void drawDrawing (Drawing *d, int height, int width);

//graphics helper functions
void drawRectangleCornerCheck(Point *a, int height, int width, char cv[height][width]);

int main() {
    printf(">> Drawing struct test\n\n");

    Drawing test = initDrawing();

    printf(">> Read test: \n\n");
    readDrawing(&test);

    printf(">> Print test: \n");
    printDrawing(&test);

    printf("\n>> Area calculation test: \n");
    printf("\nTotal area of drawing: %lf", totalArea(&test));

    printf("\n\n>> Press any key to continue.");
    getch();
    system("cls");

    printf(">> Drawing test:\n\n");
    drawDrawing(&test, 20, 100);

    getch();
    return 0;
}


void printPoint (Point *p) {
    printf("[%d/%d]\n", p->x, p->y);
}
void readPoint (Point *p) {
    printf("Enter x: ");
    scanf("%d", &(p->x));

    printf("Enter y: ");
    scanf("%d", &(p->y));
}
Point initPoint() {
    Point x = {0, 0};
    return x;
}

void printRectangle (Rectangle *r) {
    printf("Point 1: ");
    printPoint(&(r->a));
    printf("Point 2: ");
    printPoint(&(r->b));
}
void readRectangle (Rectangle *r) {
    printf("Point a (top right): \n");
    readPoint(&(r->a));

    printf("Point b (bottom left): \n");
    readPoint(&(r->b));
}
Rectangle initRectangle () {
    Rectangle rekt = {initPoint(), initPoint()};
    return rekt;
}

void printCircle (Circle *c) {
    printf("Center: ");
    printPoint(&(c->center));

    printf("Radius: %d", c->radius);
}
void readCircle (Circle *c) {
    printf("Center: \n");
    readPoint(&(c->center));

    printf("Enter radius: ");
    scanf("%d", &(c->radius));
}
Circle initCircle () {
    Circle cr = {initPoint(), 0};
    return cr;
}

void printShape (Shape *s) {
    switch (s->type) {
        case circle:
            printCircle(&(s->object.circle));
            break;
        case rectangle:
            printRectangle(&(s->object.rectangle));
            break;
    }
}

Drawing initDrawing () {
    Drawing ret = {.size = 0};
    return ret;
}
void printDrawing (Drawing *d) {
    for (int i = 0; i < d->size; i++) {
        printf("\n---- Shape %d:\n", i);
        printShape(&(d->objects[i]));
    }
}
void addShape (Drawing *d, Shape *s) {
    d->size += 1;
    d -> objects [ d -> size - 1 ] = * s;
}
void delShape (Drawing *d, int index) {
    d -> size -= 1;
    for (int i = d -> size; i > index; i--) {
        d->objects[i - 1] = d->objects[i];
    }
}
void readDrawing (Drawing *d) {
    int amount = 0;
    printf("Enter amount of shapes you want to add: ");
    scanf("%d", &amount);
    printf("\n");

    for (int i = d -> size; i < amount; i++) {
        printf("[%d]\tCircle (0) or Rectangle (1)? (Enter number) : ", i);
        int typeInt = 0;
        scanf("%d", &typeInt);
        printf("\n---- Input Prompt Shape %d --- \n", i);

        switch (typeInt) {
            case 0:
                d -> objects[i] = (Shape) {.type = circle, .object.circle = initCircle()};
                readCircle(&(d -> objects[i].object.circle));
                break;
            case 1:
                d -> objects[i] = (Shape) {.type = rectangle, .object.rectangle = initRectangle()};
                readRectangle(&(d -> objects[i].object.rectangle));
                break;
            default:
                printf("\nInvalid Integer for type selection: %d\n", typeInt);
                exit(1);
        }

        printf("---- End Input Prompt Shape %d --- \n\n", i);
    }

    d -> size += amount;
}
double totalArea (Drawing *d) {
    double area = 0;

    int w;
    int l;

    for (int i = 0; i < d->size; i++) {
        switch (d->objects[i].type) {
            case circle:
                area += (double) d->objects[i].object.circle.radius * M_PI;
                break;
            case rectangle:
                l = d->objects[i].object.rectangle.b.y - d->objects[i].object.rectangle.a.y;
                w = d->objects[i].object.rectangle.b.x - d->objects[i].object.rectangle.a.x;
                area += (double) w * (double) l;
                break;
        }
    }

    return area;
}

void drawDrawing (Drawing *d, int height, int width) {
    char canvas[height][width+1];

    //clear
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            canvas[y][x] = blank;
        }
        canvas[y][width] = '\0';
    }

    //map all objects
    for (int i = 0; i<d->size; i++) {
        switch (d->objects[i].type) {
            case circle:
                drawCircle(height, width + 1, canvas, &(d->objects[i].object.circle));
                break;
            case rectangle:
                drawRectangle(height, width+1, canvas, &(d->objects[i].object.rectangle));
                break;
        }
    }

    printf("Drawing visualized (%dx%d):\n\n", height, width);
    for (int i = height - 1; i > -1; i--) {
        printf("%c%s%c\n", border, canvas[i], border);
    }
}
void drawRectangle(int height, int width, char cv[height][width], Rectangle *r) {
    Point bottomLeft = r->b;
    Point topRight = r->a;
    Point topLeft = {.y = topRight.y, .x = topRight.x - (topRight.x - bottomLeft.x)};
    Point bottomRight = {.x = topRight.x, .y = topRight.y - (topRight.y - bottomLeft.y)};

    int x = bottomLeft.x;
    int y = bottomLeft.y;
    int w = topRight.x - bottomLeft.x;
    int h = topRight.y - bottomLeft.y;
    for (int i = 0; i < w; i++) {
        if (x < width - i - 1 && y < height && x >= -i && y >= 0) {
            cv[y][x + i] = horizontal;
        }
        if (x < width - w + i - 1 && y < height - h && x >= i - w && y >= -h) {
            cv[y + h][x + w - i] = horizontal;
        }
    }
    for (int i = 0; i < h; i++) {
        if (x < width - 1 && y < height - i && x >= 0 && y >= -i) {
            cv[y + i][x] = vertical;
        }
        if (x < width - w - 1 && y < height + i - h && x >= - w && y >= i - h) {
            cv[y + h - i][x + w] = vertical;
        }
    }

    //draw corners
    drawRectangleCornerCheck(&topLeft, height, width, cv);
    drawRectangleCornerCheck(&topRight, height, width, cv);
    drawRectangleCornerCheck(&bottomLeft, height, width, cv);
    drawRectangleCornerCheck(&bottomRight, height, width, cv);
}
void drawRectangleCornerCheck(Point *a, int height, int width, char cv[height][width]) {
    if (a->x < width - 1 && a->y < height && a->y >= 0 && a->x >= 0) {
        cv[a->y][a->x] = corner;
    }
}
void drawCircle (int height, int width, char cv[height][width], Circle *c) {

    //out markings
    int r = c->radius;
    int cX = c->center.x;
    int cY = c->center.y;
    double ratio = 6/3;
    for (int x = floor(cX - r * ratio); x < ceil(cX + r * ratio); x++) {
        for (int y = cY - r; y < cY + r; y++) {
            if (x < width - 1 && y < height && x >= 0 && y >= 0) {
                double dist = sqrt(pow((cX - x) / ratio, 2) + pow(cY - y, 2));
                if (dist < r - 1 / ratio && r - 1 <= dist) {
                    cv[y][x] = circleOut;
                }
            }
        }
    }

    //center
    if (c->center.x < width - 1 && c->center.y < height && c->center.x >= 0 && c->center.y >= 0) {
        cv[c->center.y][c->center.x] = circleCenter;
    }
}