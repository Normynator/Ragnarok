# Made with the help of:
#   https://github.com/Delfioh/SPRiggan
#   http://mist.in/gratia/ro/spr/SprFileFormat.html

import struct

from PIL import Image


def trans(b):
    if True:  # Transparent: True/False
        return b
    return True


def image(frame, palette, w, h):
    img = Image.new('RGBA', (w, h), "white")
    img.format = 'BMP'
    pixels = img.load()
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            raw_px = frame[j * img.size[0] + i]
            if trans(raw_px != 0x00):
                pixels[i, j] = (
                    byte2int(palette['r'][raw_px]),
                    byte2int(palette['g'][raw_px]),
                    byte2int(palette['b'][raw_px]))
            else:
                pixels[i, j] = (255, 255, 255, 0)

    return img


def parse_frame_data512(data):
    return data


def parse_frame_data513(data):
    ret_data = {}
    q = 0
    j = 0
    while j < data.__len__():
        if data[j] == 0x00:
            for i in range(data[j + 1]):
                ret_data[q] = 0x00
                q += 1
            j += 1
        else:
            ret_data[q] = data[j]
            q += 1
        j += 1

    return ret_data


def render_spr512(data, frame_count):
    a, z = 6, 8
    frame = {}
    frame_width = {}
    frame_height = {}
    for i in range(byte2int(frame_count)):
        a += 2
        z += 2
        width = data[a:z]
        print('Width: ' + byte2str(width) + ' | ' + str(width))

        a += 2
        z += 2
        height = data[a:z]
        print('Height: ' + byte2str(height) + ' | ' + str(height))

        length = byte2int(width) * byte2int(height)
        print('Length of frame {}: {}'.format(i, length))

        z += length
        temp_frame_data = data[a + 2:z]  # compressed
        a += length
        print(temp_frame_data)

        frame[i] = parse_frame_data512(temp_frame_data)
        frame_width[i] = byte2int(width)
        frame_height[i] = byte2int(height)

        print('a:', a)
        print('z:', z)
        print('pixels: ', frame[i].__len__())

    z += 1024
    p = data[a + 2:z]
    print(p)
    ret = struct.unpack('1024c', p)
    print(ret)

    c = 0
    palette = dict.fromkeys(['r', 'g', 'b'])
    palette['r'] = dict()
    palette['g'] = dict()
    palette['b'] = dict()

    for i in range(256):
        palette['r'][i] = ret[c + 0]
        palette['g'][i] = ret[c + 1]
        palette['b'][i] = ret[c + 2]
        c += 4

    return frame, palette, frame_width, frame_height


def render_spr513(data, frame_count):

    a, z = 6, 8
    frame = {}
    frame_width = {}
    frame_height = {}
    for i in range(byte2int(frame_count)):
        a += 2
        z += 2
        width = data[a:z]
        print('Width: ' + byte2str(width) + ' | ' + str(width))

        a += 2
        z += 2
        height = data[a:z]
        print('Height: ' + byte2str(height) + ' | ' + str(height))

        a += 2
        z += 2
        length = byte2int(data[a:z])
        print('Length of frame {}: {}'.format(i, length))

        z += length
        temp_frame_data = data[a + 2:z]  # compressed
        a += length
        print(temp_frame_data)

        frame[i] = parse_frame_data513(temp_frame_data)
        frame_width[i] = byte2int(width)
        frame_height[i] = byte2int(height)

        print('a:', a)
        print('z:', z)
        print('pixels: ', frame[i].__len__())

    z += 1024
    p = data[a + 2:z]
    print(p)
    ret = struct.unpack('1024c', p)
    print(ret)

    c = 0
    palette = dict.fromkeys(['r', 'g', 'b'])
    palette['r'] = dict()
    palette['g'] = dict()
    palette['b'] = dict()

    for i in range(256):
        palette['r'][i] = ret[c + 0]
        palette['g'][i] = ret[c + 1]
        palette['b'][i] = ret[c + 2]
        c += 4

    return frame, palette, frame_width, frame_height


def byte2str(input_):
    return str(int.from_bytes(input_, byteorder='little'))


def byte2int(input_):
    return int.from_bytes(input_, byteorder='little')


def render_spr(filename):
    with open(filename, 'rb') as f:
        data = f.read()

    print('Data: ' + str(data))

    spr_header = data[:2]
    print('Sprite Header: ' + str(spr_header))

    spr_version = byte2int(data[2:4])
    print('Version: ' + str(spr_version))

    frame_count = data[4:6]
    print('Frame count 1: ' + byte2str(frame_count))

    frame_count2 = data[6:8]
    print('Frame count 2: ' + byte2str(frame_count2))

    images = {}
    if spr_version == 513:
        frame, palette, frame_width, frame_height = render_spr513(data, frame_count)

    if spr_version == 512:
        frame, palette, frame_width, frame_height = render_spr512(data, frame_count)

    for i in range(frame.__len__()):
        images[i] = image(frame[i], palette, frame_width[i], frame_height[i])

    return images



def main():
    print('Starting ...')
    #filename = 'kopf_1.spr'
    filename = 'cursors.spr'
    #frames, palette, frames_width, frames_height = render_spr(filename)
    render_spr(filename)[0].show()

    #for i in range(frames.__len__()):
        #image(frames[i], frames_width[i], frames_height[i], palette)
    #image(frames[0], frames_width[0], frames_height[0], palette)
    #print(palette['r'])
    #print(palette['g'])
    #print(palette['b'])
    #s = ""
    #i = 0
    #for p in frames[0]:
    #    s += str(frames[0][p]) + ' '
    #    if i % 22 == 0 and i != 0:
    #        s += '\n'
    #    i += 1
    #print(s)


if __name__ == '__main__':
    main()
