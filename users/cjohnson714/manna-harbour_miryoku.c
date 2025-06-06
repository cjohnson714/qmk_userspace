// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"

// Additional Features double tap guard

enum {
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
    MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        reset_keyboard();
    }
}

#define MIRYOKU_X(LAYER, STRING)                                        \
    void u_td_fn_U_##LAYER(tap_dance_state_t *state, void *user_data) { \
        if (state->count == 2) {                                        \
            default_layer_set((layer_state_t)1 << U_##LAYER);           \
        }                                                               \
    }
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

tap_dance_action_t tap_dance_actions[] = {[U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
                                          MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define MIRYOKU_X(LAYER, STRING) [U_##LAYER] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_##LAYER, MIRYOKU_LAYER_##LAYER),
    MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t *key_overrides[] = {&capsword_key_override};

// thumb combos

#if defined(MIRYOKU_KLUDGE_THUMBCOMBOS)
const uint16_t PROGMEM thumbcombos_base_right[] = {LT(U_SYM, KC_ENT), LT(U_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[]  = {LT(U_NAV, KC_SPC), LT(U_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[]        = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM thumbcombos_mouse[]      = {KC_BTN2, KC_BTN1, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[]      = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[]        = {KC_0, KC_MINS, COMBO_END};
#    if defined(MIRYOKU_LAYERS_FLIP)
const uint16_t PROGMEM thumbcombos_sym[] = {KC_UNDS, KC_LPRN, COMBO_END};
#    else
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
#    endif
const uint16_t PROGMEM thumbcombos_fun[]       = {KC_SPC, KC_TAB, COMBO_END};
combo_t                key_combos[COMBO_COUNT] = {COMBO(thumbcombos_base_right, LT(U_FUN, KC_DEL)),
                                                  COMBO(thumbcombos_base_left, LT(U_MEDIA, KC_ESC)),
                                                  COMBO(thumbcombos_nav, KC_DEL),
                                                  COMBO(thumbcombos_mouse, KC_BTN3),
                                                  COMBO(thumbcombos_media, KC_MUTE),
                                                  COMBO(thumbcombos_num, KC_DOT),
#    if defined(MIRYOKU_LAYERS_FLIP)
                                   COMBO(thumbcombos_sym, KC_RPRN),
#    else
                                   COMBO(thumbcombos_sym, KC_LPRN),
#    endif
                                                  COMBO(thumbcombos_fun, KC_APP)};
#endif

#include "socd_cleaner.h"

socd_cleaner_t socd_v = {{KC_W, KC_S}, SOCD_CLEANER_LAST};
socd_cleaner_t socd_h = {{KC_A, KC_D}, SOCD_CLEANER_LAST};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_socd_cleaner(keycode, record, &socd_v)) {
        return false;
    }
    if (!process_socd_cleaner(keycode, record, &socd_h)) {
        return false;
    }
    // Your macros...
    return true;
}
layer_state_t default_layer_state_set_user(layer_state_t state) {
    socd_cleaner_enabled = IS_LAYER_ON_STATE(state, TAP);
    return state;
}

#ifdef FLOW_TAP_TERM
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        switch (keycode) {
            // QWERTY Layer
            case LGUI_T(KC_A):
            case LALT_T(KC_S):
            case LT(U_BUTTON, KC_Z):
            case ALGR_T(KC_X):
            case LGUI_T(KC_QUOT):
            case LALT_T(KC_L):
            case LT(U_BUTTON, KC_SLSH):
            case ALGR_T(KC_DOT):

            // Gallium Layer
            case LGUI_T(KC_N):
            case LALT_T(KC_R):
            case LT(U_BUTTON, KC_X):
            case ALGR_T(KC_Q):
            case LGUI_T(KC_I):
            case LALT_T(KC_E):
            case LT(U_BUTTON, KC_DOT):
            case ALGR_T(KC_SCLN):
                return FLOW_TAP_TERM;

            // QWERTY Layer
            case LCTL_T(KC_D):
            case LCTL_T(KC_K):

            // Gallium Layer
            case LCTL_T(KC_T):
            case LCTL_T(KC_A):
                return FLOW_TAP_TERM - 25;
        }
    }

    return 0;
}
#endif

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_F):
        case LSFT_T(KC_J):
            return TAPPING_TERM - 45;
        default:
            return TAPPING_TERM;
    }
}
