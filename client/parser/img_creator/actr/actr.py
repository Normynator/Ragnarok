import struct
from actr.animation import Animation
from actr.frame import Frame
from actr.subframe import Subframe
from actr.act import Act


def dprint(msg):
    pass


def byte2str(input_, signed=False):
    return str(int.from_bytes(input_, byteorder='little', signed=signed))


def byte2int(input_, signed=False):
    return int.from_bytes(input_, byteorder='little', signed=signed)


def parse_act(filename):
    act = Act()
    with open(filename, 'rb') as f:
        data = f.read()

    dprint(data)
    offset = 0

    act_header = data[:2]
    act.header = act_header
    dprint('Act Header: ' + str(act_header))

    act_sub_version = data[2:3]
    act.sub_version = byte2int(act_sub_version)
    dprint('Sub Version: ' + byte2str(act_sub_version))
    act_sub_version = byte2int(act_sub_version)

    act_version = data[3:4]
    act.version = byte2int(act_version)
    dprint('Version: ' + byte2str(act_version))

    nanimations = data[4:6]
    dprint('# Animations: ' + byte2str(nanimations) + ' | ' + str(nanimations))

    unknown = data[6:16]
    # not in act object, cause not needed
    dprint('Unknown: ' + byte2str(unknown))

    b = 12
    e = 16
    offset += 4
    animations = []
    # temp not needed
    # animations['count'] = byte2int(nanimations)
    for k in range(byte2int(nanimations)):
        animation = Animation()

        nframes = data[b + offset:e + offset]
        offset += 4
        dprint('# Frames: ' + byte2str(nframes) + ' | ' + str(nframes))

        frames = []
        # temp not needed
        # frames['count'] = byte2int(nframes)
        for j in range(byte2int(nframes)):
            frame = Frame()
            frame.id = j
            # unused = data[20:52]
            offset += 32
            # dprint('Unused: ' + byte2str(unused))

            nsubframes = data[b + offset:e + offset]
            offset += 4
            dprint('# SubFrames: ' + byte2str(nsubframes))

            subframes = []
            # temp not needed
            # subframes['count'] = byte2int(nsubframes)
            for i in range(byte2int(nsubframes)):
                subframe = Subframe()

                dprint('---- Subframe ' + str(i + 1) + ' ----')

                offset_x = data[b + offset:e + offset]
                subframe.offset_x = byte2int(offset_x, True)
                offset += 4
                dprint('Offset X: ' + byte2str(offset_x, True) + ' | ' + str(
                    offset_x))

                offset_y = data[b + offset:e + offset]
                subframe.offset_y = byte2int(offset_y, True)
                offset += 4
                dprint('Offset Y: ' + byte2str(offset_y, True) + ' | ' + str(
                    offset_y))

                image = data[b + offset:e + offset]
                subframe.image = byte2int(image, True)
                offset += 4
                dprint('Image: ' + byte2str(image, True) + ' | ' + str(image))

                direction = data[b + offset:e + offset]
                subframe.mirror = byte2int(direction)
                offset += 4
                dprint('Mirror: ' + byte2str(direction) + ' | ' + str(
                    direction))

                color = data[b + offset:e + offset]
                subframe.color = byte2int(color)
                offset += 4
                dprint('Color: ' + byte2str(color) + ' | ' + str(color))

                if act_sub_version >= 2:
                    scale_x = data[b + offset:e + offset]
                    subframe.scale_x = struct.unpack('f', scale_x)
                    offset += 4
                    dprint(struct.unpack('f', scale_x))
                    dprint('Scale X: ' + ' | ' + str(scale_x))

                if act_sub_version >= 4:
                    scale_y = data[b + offset:e + offset]
                    subframe.scale_y = struct.unpack('f', scale_y)
                    offset += 4
                    dprint(struct.unpack('f', scale_y))
                    dprint('Scale Y:' + ' | ' + str(scale_y))
                else:
                    scale_y = scale_x
                    subframe.scale_y = struct.unpack('f', scale_y)
                    dprint('Scale Y:' + byte2str(scale_y))

                if act_sub_version >= 2:
                    rotation = data[b + offset:e + offset]
                    subframe.rotation = byte2int(rotation)
                    offset += 4
                    dprint('Rotation: ' + byte2str(rotation))

                    jump = data[b + offset:e + offset]
                    # currently not needed in
                    offset += 4
                    dprint('Jump: ' + byte2str(jump) + ' | ' + str(jump))

                    if not jump:
                        offset += 12

                if act_sub_version >= 5:
                    size_x = data[b + offset:e + offset]
                    subframe.size_x = byte2int(size_x)
                    offset += 4
                    dprint('Size X: ' + byte2str(size_x))

                    size_y = data[b + offset:e + offset]
                    subframe.size_y = byte2int(size_y)
                    offset += 4
                    dprint('Size Y: ' + byte2str(size_y))

                subframes.append(subframe)

            frame.subframes = subframes

            sound = data[b + offset:e + offset]
            frame.sound = byte2int(sound, True)
            offset += 4
            dprint('Sound: ' + byte2str(sound, True))

            if act_sub_version >= 2:

                extra_info = data[b + offset:e + offset]
                frame.extra_info = byte2int(extra_info, True)
                offset += 4
                dprint('Extra Info: ' + byte2str(extra_info, True))

                if byte2int(extra_info, True) < 0:
                    raise ValueError('Extra Info cant be negative')

                for _ in range(byte2int(extra_info)):
                    offset += 4

                    extra_x = data[b + offset:e + offset]
                    frame.extra_x = byte2int(extra_x, True)
                    offset += 4
                    dprint('Extra X: ' + byte2str(extra_x, True))

                    extra_y = data[b + offset:e + offset]
                    frame.extra_y = byte2int(extra_y, True)
                    offset += 4
                    dprint('Extra Y: ' + byte2str(extra_y, True))

            sound_file = data[b + offset:e + offset]
            frame.sound_file = byte2int(sound_file)
            offset += 4
            dprint('Sound File: ' + byte2str(sound_file))

            for _ in range(byte2int(sound_file)):
                offset += 40

            frames.append(frame)

        animation.frames = frames
        animations.append(animation)

    act.animations = animations
    # dprint(animations.__len__())

    # animation speed not needed currently
    # for _ in range(byte2int(nframes)):
    #    offset += 4
    return act


def main():
    # filename = 'Black_Valkyrie_Helm.act'
    filename = 'wizard.act'
    dprint(parse_act(filename))


if __name__ == '__main__':
    main()
