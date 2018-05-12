import struct


def byte2str(input_, signed=False):
    return str(int.from_bytes(input_, byteorder='little', signed=signed))


def byte2int(input_, signed=False):
    return int.from_bytes(input_, byteorder='little', signed=signed)


def parse_act(filename):
    act = {}
    with open(filename, 'rb') as f:
        data = f.read()

    print(data)
    offset = 0

    act_header = data[:2]
    act['act_header'] = act_header
    print('Act Header: ' + str(act_header))

    act_sub_version = data[2:3]
    act['act_sub_version'] = byte2int(act_sub_version)
    print('Sub Version: ' + byte2str(act_sub_version))
    act_sub_version = byte2int(act_sub_version)

    act_version = data[3:4]
    act['act_version'] = byte2int(act_version)
    print('Version: ' + byte2str(act_version))

    nanimations = data[4:6]
    print('# Animations: ' + byte2str(nanimations) + ' | ' + str(nanimations))

    unknown = data[6:16]
    # not in act object, cause not needed
    print('Unknown: ' + byte2str(unknown))

    b = 12
    e = 16
    offset += 4
    animations = {}
    animations['count'] = byte2int(nanimations)
    for k in range(byte2int(nanimations)):
        animation = {}

        nframes = data[b + offset:e + offset]
        offset += 4
        print('# Frames: ' + byte2str(nframes) + ' | ' + str(nframes))

        frames = {}
        frames['count'] = byte2int(nframes)
        for j in range(byte2int(nframes)):
            frame = {}

            # unused = data[20:52]
            offset += 32
            # print('Unused: ' + byte2str(unused))

            nsubframes = data[b + offset:e + offset]
            offset += 4
            print('# SubFrames: ' + byte2str(nsubframes))

            subframes = {}
            subframes['count'] = byte2int(nsubframes)
            for i in range(byte2int(nsubframes)):
                subframe = {}

                print('---- Subframe ' + str(i + 1) + ' ----')

                offset_x = data[b + offset:e + offset]
                subframe['offset_x'] = byte2int(offset_x, True)
                offset += 4
                print('Offset X: ' + byte2str(offset_x, True) + ' | ' + str(
                    offset_x))

                offset_y = data[b + offset:e + offset]
                subframe['offset_y'] = byte2int(offset_y, True)
                offset += 4
                print('Offset Y: ' + byte2str(offset_y, True) + ' | ' + str(
                    offset_y))

                image = data[b + offset:e + offset]
                subframe['image'] = byte2int(image, True)
                offset += 4
                print('Image: ' + byte2str(image, True) + ' | ' + str(image))

                direction = data[b + offset:e + offset]
                subframe['direction'] = byte2int(direction)
                offset += 4
                print('Direction: ' + byte2str(direction) + ' | ' + str(
                    direction))

                color = data[b + offset:e + offset]
                subframe['color'] = byte2int(color)
                offset += 4
                print('Color: ' + byte2str(color) + ' | ' + str(color))

                if act_sub_version >= 2:
                    scale_x = data[b + offset:e + offset]
                    subframe['scale_x'] = struct.unpack('f', scale_x)
                    offset += 4
                    print(struct.unpack('f', scale_x))
                    print('Scale X: ' + ' | ' + str(scale_x))

                if act_sub_version >= 4:
                    scale_y = data[b + offset:e + offset]
                    subframe['scale_y'] = struct.unpack('f', scale_y)
                    offset += 4
                    print(struct.unpack('f', scale_y))
                    print('Scale Y:' + ' | ' + str(scale_y))
                else:
                    scale_y = scale_x
                    subframe['scale_y'] = struct.unpack('f', scale_y)
                    print('Scale Y:' + byte2str(scale_y))

                if act_sub_version >= 2:
                    rotation = data[b + offset:e + offset]
                    subframe['rotation'] = byte2int(rotation)
                    offset += 4
                    print('Rotation: ' + byte2str(rotation))

                    jump = data[b + offset:e + offset]
                    # currently not needed in
                    offset += 4
                    print('Jump: ' + byte2str(jump) + ' | ' + str(jump))

                    if not jump:
                        offset += 12

                if act_sub_version >= 5:
                    size_x = data[b + offset:e + offset]
                    subframe[size_x] = byte2int(size_x)
                    offset += 4
                    print('Size X: ' + byte2str(size_x))

                    size_y = data[b + offset:e + offset]
                    subframe[size_y] = byte2int(size_y)
                    offset += 4
                    print('Size Y: ' + byte2str(size_y))

                subframes['subframe' + str(i)] = subframe

            frame['subframes'] = subframes

            sound = data[b + offset:e + offset]
            frame['sound'] = byte2int(sound, True)
            offset += 4
            print('Sound: ' + byte2str(sound, True))

            if act_sub_version >= 2:

                extra_info = data[b + offset:e + offset]
                frame['extra_info'] = byte2int(extra_info, True)
                offset += 4
                print('Extra Info: ' + byte2str(extra_info, True))

                if byte2int(extra_info, True) < 0:
                    raise ValueError('Extra Info cant be negative')

                for _ in range(byte2int(extra_info)):
                    offset += 4

                    extra_x = data[b + offset:e + offset]
                    frame['extra_x'] = byte2int(extra_x, True)
                    offset += 4
                    print('Extra X: ' + byte2str(extra_x, True))

                    extra_y = data[b + offset:e + offset]
                    frame['extra_y'] = byte2int(extra_y, True)
                    offset += 4
                    print('Extra Y: ' + byte2str(extra_y, True))

            sound_file = data[b + offset:e + offset]
            frame['sound_file'] = byte2int(sound_file)
            offset += 4
            print('Sound File: ' + byte2str(sound_file))

            for _ in range(byte2int(sound_file)):
                offset += 40

            frames['frame' + str(j)] = frame

        animation['frames'] = frames
        animations['animation' + str(k)] = animation

    act['animations'] = animations

    # animation speed not needed currently
    # for _ in range(byte2int(nframes)):
    #    offset += 4

    return act


def main():
    # filename = 'Black_Valkyrie_Helm.act'
    filename = 'kopf_1.act'
    print(parse_act(filename))


if __name__ == '__main__':
    main()
