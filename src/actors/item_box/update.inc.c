#include <actors.h>

/**
 * @brief Updates the item box under the hot air balloon object.
 * It used in Luigi Raceway.
 * (here it's just update the rotation of the item box not the position, for the position it's in object)
 *
 * @param itemBox
 */
void update_actor_item_box_hot_air_balloon(struct ItemBox* itemBox) {
    switch (itemBox->state) {
        case 5:
            itemBox->rot.x += 0xB6;
            itemBox->rot.y -= 0x16C;
            itemBox->rot.z += 0xB6;
            break;
        case 3:
            if (itemBox->someTimer == 0x14) {
                itemBox->state = 5;
                itemBox->flags = -0x4000;
            } else {
                itemBox->someTimer++;
                itemBox->rot.x += 0x444;
                itemBox->rot.y -= 0x2D8;
                itemBox->rot.z += 0x16C;
            }
            break;
    }
}

/**
 * @brief Updates the item box actor.
 *
 * @param itemBox
 */
void update_actor_item_box(struct ItemBox* itemBox) {
    switch (itemBox->state) {
        case 0:
            itemBox->state = 1;
            break;
        case 1:
            if ((itemBox->pos.y - itemBox->origY) < 8.66f) {
                itemBox->pos.y += 0.45f;
            } else {
                itemBox->pos.y = itemBox->origY + 8.66f;
                itemBox->state = 2;
                itemBox->flags = 0xC000;
            }
            break;
        case 2:
            itemBox->rot.x += 0xB6;
            itemBox->rot.y -= 0x16C;
            itemBox->rot.z += 0xB6;
            break;
        case 3:
            if (itemBox->someTimer == 20) {
                itemBox->state = 0;
                itemBox->pos.y = itemBox->resetDistance - 20.0f;
                itemBox->flags = 0xC000;
            } else {
                itemBox->someTimer++;
                itemBox->rot.x += 0x444;
                itemBox->rot.y -= 0x2D8;
                itemBox->rot.z += 0x16C;
            }
            break;
    }
}
