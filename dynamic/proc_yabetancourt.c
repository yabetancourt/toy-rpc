#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    reportable_t parent;

    double a;
    double b;
    double c;
    double root1;
    double root2;
} my_struct_t;

void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        sscanf(buf, "%lf %lf %lf", &d->a, &d->b, &d->c);
    }

    return (void *)d;
}

void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    double a = d->a;
    double b = d->b;
    double c = d->c;
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0)
    {
        d->root1 = (-b + sqrt(discriminant)) / (2 * a);
        d->root2 = (-b - sqrt(discriminant)) / (2 * a);
    }
    else
    {
        d->root1 = NAN;
        d->root2 = NAN;
    }

    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    if (isnan(d->root1) || isnan(d->root2)) {
        snprintf(d->parent.data, 255, "No hay raíces reales para la ecuación cuadrática con coeficientes (%lf, %lf, %lf)\n", d->a, d->b, d->c);
    }
    else {
        snprintf(d->parent.data, 255, "Las raíces reales de la ecuación cuadrática con coeficientes (%lf, %lf, %lf) son %lf y %lf\n", d->a, d->b, d->c, d->root1, d->root2);
    }
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

void clean_up(void *params, void *result, reportable_t *report)
{
    if (report && report->data)
    {
        free(report->data);
    }

    if (params)
    {
        free(params);
    }
}
