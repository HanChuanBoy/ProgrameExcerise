#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>


int main(int argc, char **argv)
{
     int fb_fd = 0;
     struct fb_var_screeninfo varinfo;
     struct fb_fix_screeninfo fixinfo;
     long screensize = 0;
     char *fbp = 0;
     int x=0,y=0;
     int location = 0;
     fb_fd = open("/dev/fb0",O_RDWR);
     if(fb_fd < 0) {
        printf("Error : Can not open framebuffer device/n");
        exit(1);
     }
     if (ioctl(fb_fd,FBIOGET_FSCREENINFO,&fixinfo)) {
        printf("Error reading fixed information/n");
        exit(2);
     }
     if (ioctl(fb_fd,FBIOGET_VSCREENINFO,&varinfo)) {
        printf("Error reading variable  information/n");
        exit(2);
     }
     //打印结构体的信息;
     printf("The mem is %d \n",fixinfo.smem_len);
     printf("The line_length is %d \n",fixinfo.line_length);
     printf("The xres  is %d \n",varinfo.xres);
     printf("The yres  is %d \n",varinfo.yres);
     printf("bits_per_pixel is :%d\n",varinfo.bits_per_pixel);  
     //计算显存的大小;
     screensize = varinfo.xres * varinfo.yres * varinfo.bits_per_pixel/8;
     printf("The screensize is %d \n",screensize);
     fbp = (char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd,0);
     if ((int) fbp == -1)
     {
        printf ("Error: failed to map framebuffer device to memory./n");
        exit (4);
     }
     int i,j;
     x = 10;
     y = 10;
     for(i=x,j=y;i<400;i++,j++)
    {
        location = i * (varinfo.bits_per_pixel / 8) + j  *  fixinfo.line_length;
        *(fbp + location) = 0x00;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
        *(fbp + location + 1) = 0xff; /* 绿色的色深*/  
        *(fbp + location + 2) = 0x00; /* 红色的色深*/  
        *(fbp + location + 3) = 0x00;  /* 是否透明*/ 
    }
    munmap (fbp,screensize);
    close (fb_fd);    /*关闭文件*/
    return 0;
}