#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH   130
#define HEIGHT   43
#define OCTAVES   6
#define SCALE    20.0

typedef struct
{
    double x;
    double y;
} Vector2;

typedef struct
{
    Vector2 gradients[ HEIGHT ][ WIDTH ];
} GradientMap;

double lerp( double a, double b, double t )
{
    return a * ( 1 - t ) + ( b * t );
}

double smoothstep( double x )
{
    return 6 * pow( x , 5 ) - 15 * pow( x, 4 ) + 10 * pow( x, 3 );
}

double dot( Vector2 v1, Vector2 v2 )
{
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2 random_gradient( int x, int y )
{
    double angle = ( double ) rand() / RAND_MAX * 2 * M_PI;
    Vector2 result = { cos( angle ), sin( angle ) };
    return result;
}

GradientMap generate_gradient_map( )
{
    GradientMap result;
    for ( int y = 0 ; y < HEIGHT ; y++ )
    {
        for ( int x = 0 ; x < WIDTH ; x++ )
        {
            result.gradients[y][x] = random_gradient( x, y );
        }
    }

    return result;
}

double noise( Vector2 point, GradientMap gradient_map )
{
    double total = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    for ( int i = 0 ; i < OCTAVES ; i++ )
    {
        Vector2 sample_point = {
            point.x * frequency / SCALE,
            point.y * frequency / SCALE
        };

        int x0 = (int)floor( sample_point.x );
        int x1 = x0 + 1;
        int y0 = (int)floor( sample_point.y );
        int y1 = y0 + 1;
        Vector2 gradient00 = gradient_map.gradients[y0][x0];
        Vector2 gradient10 = gradient_map.gradients[y0][x1];
        Vector2 gradient01 = gradient_map.gradients[y1][x0];
        Vector2 gradient11 = gradient_map.gradients[y1][x1];
        double tx = sample_point.x - x0;
        double ty = sample_point.y - y0;
        double u = smoothstep( tx );
        double v = smoothstep( ty );
        double dot00 = dot( gradient00, (Vector2){ tx, ty } );
        double dot10 = dot( gradient10, (Vector2){ tx-1, ty } );
        double dot01 = dot( gradient01, (Vector2){ tx, ty-1 } );
        double dot11 = dot( gradient11, (Vector2){ tx-1, ty-1 } );
        double value = lerp( lerp( dot00, dot10, u ), lerp( dot01, dot11, u ), v );

        total += value * amplitude;
        frequency *= 2.0;
        amplitude /= 2.0;
    }

    return total;
}

char terrain_type( double value )
{
    if ( value < -0.3) return ' ';
    else if ( value < 0.2 ) return '.';
    else return '^';
}

void generate_map( )
{
    GradientMap gradient_map = generate_gradient_map( );

    for ( int y = 0 ; y < HEIGHT ; y++ )
    {
        for ( int x = 0 ; x < WIDTH ; x++ )
        {
            Vector2 point = { x, y };
            double value = noise( point, gradient_map );
            printf("%c", terrain_type( value ) );
        }
        printf("\n");
    }
}

int main( )
{
    srand( time( NULL ) );
    generate_map( );
    return 0;
}

