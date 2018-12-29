#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#pragma pack(1)   /* 取消字节对齐 */

/* bmp图片文件头信息封装 */
typedef struct {
    /* 位图文件头 */
    u8  file_type[2];   /* 位图文件类型:'BM'->0x4d42 */
    u32 file_size;          /* 整个文件大小 */
    u16 reserved1;          /* 保留 */
    u16 reserved2;          /* 保留 */
    u32 offset;             /* 文件头到位图数据之间的偏移量 */

    /* 位图信息头 */
    u32 head_size;          /* 位图信息头长度 */
    u32 width;              /* 位图宽度 */
    u32 height;             /* 位图高度 */
    u16 bit_planes;         /* 位图位面数 */
    u16 bits_per_pixel;     /* 每个像素的位数 */
    u32 compression;        /* 压缩说明 */
    u32 image_size;         /* 位图数据大小 */
    u32 h_res;              /* 水平分辨率 */
    u32 v_res;              /* 垂直分辨率 */
    u32 color_palette;      /* 位图使用的颜色索引数 */
    u32 vip_color;          /* 重要的颜色索引数目 */
}bmp_header_t;

#pragma pack()  /* 恢复字节对齐 */

void init_bmp_header(bmp_header_t *header, int w, int h);

int main(int argc, char **argv)
{
    int fd;

    /* 检查参数 */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./a.out + bmp_file\n");
        return -1;
    }

    /* 文件设备fb */
    fd = open("/dev/fb0", O_RDONLY);
    if (fd < 0)
    {
        perror("open bmp");
        return -2;
    }

    /* 获取显示器屏幕信息 */
    struct fb_fix_screeninfo fix_info;
    struct fb_var_screeninfo var_info;
    int ret, line_length;
    int x, y;

    ret = ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
    if (ret < 0)
    {
        perror("get fix info");
        close(fd);
        return -3;
    }
    ret = ioctl(fd, FBIOGET_VSCREENINFO, &var_info);
    if (ret < 0)
    {
        perror("get var info");
        close(fd);
        return -4;
    }
    line_length = fix_info.line_length;
    /* 水平像素个数：一行的字节数/一个像素占的字节数 */
    x = line_length / (var_info.bits_per_pixel / 8);
    y = var_info.yres;

    /* 设置bmp文件头信息 */
    bmp_header_t header;
    init_bmp_header(&header, x, y);

    /* 保存显存数据为bmp图片 */
    int *addr = mmap(NULL, y*line_length, PROT_READ, MAP_SHARED, fd, 0);
    if (NULL == addr)
    {
        perror("mmap");
        close(fd);
        return -5;
    }
    FILE *fl = fopen(argv[1], "w");
    if (NULL == fl)
    {
        fprintf(stderr, "open %s failed\n", argv[1]);
        close(fd);
        return -6;
    }
    ret = fwrite(&header, sizeof(header), 1, fl); /* 写bmp头 */
    if (ferror(fl))
    {
        fprintf(stderr, "write header error!\n");
        close(fd);
        fclose(fl);
        return -7;
    }
    /* 写rgb数据 */
    int i, j;
    for (i = 0; i < y; i++)
    {
        for (j = 0; j < x; j++)
        {
            /* addr为int型，取它地址则偏移4个字节 */
            fwrite(&addr[i*x+j], 4, 1, fl);
        }
    }

    close(fd);
    fclose(fl);
    return 0;
}

void init_bmp_header(bmp_header_t *header, int w, int h)
{
    /* 清零头部 */
    memset(header, 0, sizeof(*header));

    /* 填写bmp信息 */
    strncpy(header->file_type, "BM", 2);
    header->file_size = (w*h*4) + 54;
    header->offset = 54;

    header->head_size = 40;
    header->width = w;
    header->height = h;
    header->bit_planes = 1;
    header->bits_per_pixel = 32;
    header->image_size = w*h*4;
}