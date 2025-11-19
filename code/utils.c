#include "utils.h"

void color(uint16 x,uint16 y,const uint16 color){
    if(x<=0) x=1; if(x>=ips114_width_max-1) x=ips114_width_max-2;
    if(y<=0) y=1; if(y>=ips114_height_max-1) y=ips114_height_max-2;
    ips114_draw_point(x,y,color);
    ips114_draw_point(x-1,y,color);
    ips114_draw_point(x+1,y,color);
    ips114_draw_point(x,y-1,color);
    ips114_draw_point(x,y+1,color);
}