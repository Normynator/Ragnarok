from PIL import Image
from actr import actr
from sprr import sprr
from actr.act import Act

__ACTIONS = {'IDLE': 0,
             'WALKING': 1,
             'SIT': 2,
             'PICKUP': 3,
             'READYFIGHT': 4,
             'ATTACK': 5,
             'HURT': 6,
             'UNK1': 7,
             'DIE': 8,
             'UNK2': 9,
             'ATTACK2': 10,
             'ATTACK3': 11,
             'SKILL': 12
            }


class DataObj(object):
    act_filename = ''   # String
    spr_filename = ''   # String
    sprite = None       # Sprite
    action = Act()      # Action


def build_animation(body_filename, head_filename, hg_filename_list, action, direction):
    # -1 => all frames
    __build_image(body_filename, head_filename, hg_filename_list, action, direction, frame_nr=-1)


def build_image(body_filename, head_filename, hg_filename_list, action, direction, frame_nr=0):
    __build_image(body_filename, head_filename, hg_filename_list, action, direction, frame_nr)


def __create_animation(frames):
    img = Image.new('RGBA', (100*frames.__len__(), 120))
    i = 0
    for frame in frames:
        img.paste(frame, (100*i, 0), frame)
        i += 1
    img.save('tmp/animation.png')


def __build_image(body_filename, head_filename, hg_filename_list, action, direction, frame_nr):
    body = DataObj()
    body.act_filename, body.spr_filename = body_filename + '.act', body_filename + '.spr'
    body.sprite, body.action = sprr.render_spr(body.spr_filename), actr.parse_act(body.act_filename)

    head = DataObj()
    head.act_filename, head.spr_filename = head_filename + '.act', head_filename + '.spr'
    head.sprite, head.action = sprr.render_spr(head.spr_filename), actr.parse_act(head.act_filename)

    hg_list = [DataObj(), DataObj(), DataObj()]

    i = 0
    for hg in hg_filename_list:
        if hg is not None:
            hg_list[i].act_filename, hg_list[i].spr_filename = hg + '.act', hg + '.spr'
            hg_list[i].sprite, hg_list[i].action = sprr.render_spr(hg_list[i].spr_filename), actr.parse_act(hg_list[i].act_filename)
        else:
            hg_list[i] = None
        i += 1

    __render_images(body, head, hg_list, action, direction, frame_nr)


def __render_images(body, head, hg_list, action, direction, frame_nr):

    if direction > 7:
        raise ValueError('Direction max size is 7')

    action_ = __ACTIONS[action]
    animation_nr = 8 * action_ + direction

    if frame_nr < 0:  # If frame_nr is -1 -> render all frames
        frames = []
        for frame in body.action.animations[animation_nr].frames:
            img = Image.new('RGBA', (100, 120))
            frames.append(__processing(img, body, head, hg_list, animation_nr, frame.id))
        __create_animation(frames)

    else:
        img = Image.new('RGBA', (400, 400))
        __processing(img, body, head, hg_list, animation_nr, frame_nr).save('tmp/tmp' + str(frame_nr) + '.png')


def __render_obj(img, render_obj, animation_nr, frame_nr, pos):
    obj = render_obj.action.animations[animation_nr].frames[frame_nr]
    for obj_subframe in obj.subframes:
        image_nr = obj_subframe.image
        if image_nr > -1:
            obj_img = render_obj.sprite[image_nr]
            if pos is None:
                ref_x, ref_y = obj.extra_x, obj.extra_y
            else:
                ref_x, ref_y = pos
            hg_pos_x = 50 - obj_img.size[0] / 2 + obj_subframe.offset_x + ref_x - obj.extra_x
            hg_pos_y = 100 - obj_img.size[1] / 2 + obj_subframe.offset_y + ref_y - obj.extra_y

            if obj_subframe.mirror:
                obj_img = obj_img.transpose(Image.FLIP_LEFT_RIGHT)
            img.paste(obj_img, (int(hg_pos_x), int(hg_pos_y)), obj_img)

    return obj.extra_x, obj.extra_y


def __processing(img, body, head, hg_list, animation_nr, frame_nr):

    # Render BODY
    ref_pos = __render_obj(img, body, animation_nr, frame_nr, None)

    # Render HEAD
    __render_obj(img, head, animation_nr, frame_nr, ref_pos)

    # Render HEADGEARS
    for hg in hg_list:
        if hg is not None:
            __render_obj(img, hg, animation_nr, frame_nr, ref_pos)

    return img


def main():
    # build_image('wizard', 'kopf_1', ['Black_Valkyrie_Helm', None, None], 'WALKING', 1)
    build_image('wizard', 'kopf_1', ['Black_Valkyrie_Helm', None, None],
                'IDLE', 7)


if __name__ == '__main__':
    main()

# TODO: Support mirror
# TODO: add cache system
# TODO: decide what happens if no extra info is provided


# Thanks to RoChargenPHP
# Vx/     Vy/       lindex/ Vis_mirror/     C4color/    f2scale/        Vangle/ Vspr_type/  Vwidth/ Vheight
# offsetX offsetY   Bild    Richtung        Farbe       MaßStab X & Y   Angle   unkown      GrößeX  GrößeY

# layer->pos = offsetX offsetY
# pos = extraX extraY

# offsetX+bodyextraX-extraX
