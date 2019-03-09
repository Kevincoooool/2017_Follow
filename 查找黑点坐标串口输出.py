import sensor,time,pyb,math,lcd
import m_lcd
from pyb import Pin, Timer, LED, UART
#黑色点阈值
black_threshold = [(0,64)]
#xy平面误差数据
err_x = 0
err_y = 0
#发送数据
uart_buf = bytearray([0x55,0xAA,0x00,0x00,0x00,0x00,0xAA])

#串口三配置
uart = UART(3,115200)
uart.init(115200, bits=8, parity=None, stop=1)

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # or sensor.GRAYSCALE
sensor.set_framesize(sensor.QQVGA2) # Special 128x160 framesize for LCD Shield.
sensor.skip_frames(20)#相机自检几张图片
#sensor.set_auto_whitebal(False)#关闭白平衡
clock = time.clock()#打开时钟

lcd.init()
while(True):
    clock.tick()
    img = sensor.snapshot()

    #寻找blob
    blobs = img.find_blobs(black_threshold)
    if blobs:
        most_pixels = 0
        largest_blob = 0
        for i in range(len(blobs)):
            #目标区域找到的颜色块可能不止一个，找到最大的一个
            if blobs[i].pixels() > most_pixels:
                most_pixels = blobs[i].pixels()
                largest_blob = i
                #位置环用到的变量
                err_x = int(60 - blobs[largest_blob].cy())
                err_y = int(blobs[largest_blob].cx() - 80)
        img.draw_cross(blobs[largest_blob].cx(),blobs[largest_blob].cy())#调试使用
        img.draw_rectangle(blobs[largest_blob].rect())
    else:
       err_x = 0
       err_y = 0
    m_lcd.lcd2()
    lcd.display(img)
    #数组中数据写入
    uart_buf = bytearray([0x55,0xAA,err_x>>8,err_x,err_y>>8,err_y,0xA1])
    print(err_x,err_y)
    #uart.write(uart_buf)
