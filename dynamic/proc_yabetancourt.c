#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <complex.h>

typedef struct
{
    reportable_t parent;

    double a;
    double b;
    double c;
    complex double root1;
    complex double root2;
    bool error;
    char error_message[255];
} my_struct_t;

void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        int parsed_values = sscanf(buf, "%lf %lf %lf", &d->a, &d->b, &d->c);
        if (parsed_values != 3)
        {
            d->error = true;
            snprintf(d->error_message, 255, "Error al analizar los coeficientes. Asegúrese de ingresar 3 valores separados por espacios.\n");
        }
        else
        {
            d->error = false;
        }
    }

    return (void *)d;
}

void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    if (d->error)
    {
        return data;
    }

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
        d->root1 = (-b + csqrt(discriminant)) / (2 * a);
        d->root2 = (-b - csqrt(discriminant)) / (2 * a);
    }

    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    if (d->error)
    {
        snprintf(d->parent.data, 255, "%s", d->error_message);
    }
    else if (cimag(d->root1) == 0 && cimag(d->root2) == 0)
    {
        snprintf(d->parent.data, 255, "Las raíces reales de la ecuación cuadrática con coeficientes (%lf, %lf, %lf) son %lf y %lf\n", d->a, d->b, d->c, creal(d->root1), creal(d->root2));
    }
    else
    {
        snprintf(d->parent.data, 255, "Las raíces complejas de la ecuación cuadrática con coeficientes (%lf, %lf, %lf) son %lf + %lfi y %lf + %lfi\n", d->a, d->b, d->c, creal(d->root1), cimag(d->root1), creal(d->root2), cimag(d->root2));
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
