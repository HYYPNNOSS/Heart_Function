#include <mlx.h>
#include <unistd.h>
#include <math.h>
//math function: "y=x^{\frac{2}{3}}+0.9*\sqrt{3.3\ -\ x^{2}}*\sin(a*\pi*x)\"
typedef struct    s_data {
    void    *img;
    char    *addr;
    int        bits_per_pixel;
    int        line_length;
    int        endian;
}                t_data;

typedef struct s_nadi
{
    void *mlx;
    void *mlx_win;
    t_data *img;
}   t_nadi;

void    my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void draw(void *mlx, void *mlx_win, t_data *img, double a)
{
    double x;
    double tmp_x;
    double y;
    double yy;
    double yyy;
    double yyyy;

    mlx_clear_window(mlx, mlx_win);
    x = 0;
    while (x < 1820)
    {
        y = 0;
        while (y < 1080)
        {
            my_mlx_pixel_put(img, x, y, 0);
            y++;
        }
        x++;
    }

    x = 250;
    while (x < 1000)
    {
        tmp_x = map(x, 500, 1000, 1.8164, -1.8164);
        y = 0;
        yy = cbrt(pow(tmp_x, 2));
        yyy = 0.9 * sqrt(3.3 - pow(tmp_x, 2));
        yyyy = sin(a * M_PI * tmp_x);
        y = yy + yyy * yyyy;
        my_mlx_pixel_put(img, x, map(y, 1.8164, -1.8164, 500, 1000), 0x002b1d69);
        x= x + 0.0001 ;
    }
    mlx_put_image_to_window(mlx, mlx_win, img->img, 0, 0);
}

int mouse_hook(int keycode, t_nadi *nadi)
{
    static  double  a = 0;
	if (keycode == 126)
		a++;
    else if (keycode == 125)
		a--;
    draw(nadi->mlx, nadi->mlx_win, nadi->img, a);
    return (1);
}

int    main(void)
{
    void    *mlx;
    void    *mlx_win;
    t_data  img;
	double  a = 0;
    t_nadi  nadi;
    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1820, 1080, "HYPN05!");
    img.img = mlx_new_image(mlx, 1820, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                &img.endian);
    nadi.img = &img;
    nadi.mlx = mlx;
    nadi.mlx_win = mlx_win;
    draw(mlx, mlx_win, &img, 0);
	mlx_hook(mlx_win, 2, 0L, mouse_hook , &nadi);
    mlx_loop(mlx);
}
